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
  lcd.begin(16,2);// Initialisierung LC-Display

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

  //Abfrage ob Taster gedrückt wurde um delay zu bestimmen
  if(millis() >= time1){
    delay(2990);
  }
  else{
    delay(500);
  }


}

// lässt LED für 10ms aufblinken
void blink(){

  digitalWrite(LED_BUILTIN, HIGH);
  delay(10);
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
  
  position = map(spannung, 0, 1023, 0, 179);
  myservo.write(position);

}

void temperature(){

  sensorWert = analogRead(tempSensor);

  float R = 1023.0/sensorWert-1.0;
  R = R0*R;

  float temperatur = 1.0/(log(R/R0)/B+1/298.15)-273.15;
  delay(500);
  
  Serial.println(temperatur); 
}
