/*
Zur Nutzung des LC-Displays am Arduino Due müssen die rgb_lcd.h & rgb_lcd.cpp Dateien der rgb lcd library mit denen auf der Moodle Seite zur Verfügung gestellten ersetzt werden
*/

#define DEBUG
// green board
#if defined(ARDUINO_ARCH_SAM)
#define MAX_VOLTAGE 3300
#define WireMaster Wire1
#define SETRES(x) analogReadResolution(x)
// red board
#elif defined(ARDUINO_ARCH_AVR)
#define ADC_RESOLUTION 10
#define MAX_VOLTAGE 5000
#define WireMaster Wire
#define SETRES(x) while(false){}
// untested
#else
#error "Wrong Hardware"
#endif

const unsigned long ADCMax = (1 << ADC_RESOLUTION) - 1;

#include <Wire.h>
#include <Servo.h>
#include <math.h>
#include "rgb_lcd.h"

//Servo und LC-Display
Servo myservo;
rgb_lcd lcd;



int time1;
int buttonState = 0;
int servoState = 0;
int sensorWert;


const int B = 4275;
const int R0 = 100000;
const int tempSensor = A1;

void setup() {
  
  //Initialisierung der Baud-Rate für die serielle Schnittstelle
  Serial.begin(9600);

  //Belegung der Pins auf dem Shield
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(3, INPUT);  //Button-D3
  pinMode(A0, INPUT); //Poti-A0
  myservo.attach(A2); //Servo-A2


  //Initialisierung LC-Displays
  lcd.begin(16,2, &WireMaster);// Initialisierung LC-Display
  SETRES(ADC_RESOLUTION);

  //Startbildschirm beim Booten des qC
  lcd.print("Embedded 2");
  delay(1000);

  //Interrupt
  attachInterrupt(digitalPinToInterrupt(3), taster, CHANGE);
 

}

void loop() {

  temperature();

  servoSteuerung();

  blink();

  serialTimer();

  //Vergleich der Zeit mit Zeitstempel um Durchlaufrate zu bestimmen
  if(millis() >= time1){
    delay(2950);
  }
  else{
    delay(450);
  }


}

// lässt LED für 10ms aufblinken
void blink(){

  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);

}

// Timer für serielle Schnittstelle
void serialTimer(){

  Serial.print("time since start: "); 
  Serial.print(millis());
  Serial.println("ms.");

}

//ISR bei Tastendruck  
void taster(){
 
//Zeitstempel aus aktueller Zeit + 1min wird erzeugt
  time1 = millis()+60000;
}

void servoSteuerung(){

  int spannung;
  int position;
  
  spannung = analogRead(A0);
  
  position = map(spannung, 0, 1023, 0, 170);
  myservo.write(position);

}

void temperature(){

  sensorWert = analogRead(tempSensor);

  float R = (float) ADCMax/sensorWert-1.0;
 // R = R0*R;

  float temperatur = 1.0/(log(R)/B+1/298.15)-273.15;
  delay(500);
  
  
  lcd.setCursor(0, 1);
  lcd.print(temperatur);
  Serial.print("R ->");
  Serial.println(R);
  Serial.print("sensorWert ->");
  Serial.println(sensorWert);  

}



//lcd.createChar(0, degrees);
