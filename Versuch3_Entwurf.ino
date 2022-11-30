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
#include <TaskScheduler.h>

// Callbacks deklarieren
void temperatureCallback();
void servoSteuerungCallback();
void blinkCallback();
void serialTimerCallback();

// Task     (LAUFZEIT; WIEDERHOLUNG; CALLBACK)
Task temperature(3000; TASK_FOREVER; &temperatureCallback);
Task servoSteuerung();
Task blink();
Task serialTimer();

Schedular runner;

//
void temperatureCallback(){

  Serial.println("Temperaturmessung");

  sensorWert = analogRead(tempSensor);

  float R = 1023.0/sensorWert-1.0;
  R = R0*R;

  float temperatur = 1.0/(log(R/R0)/B+1/298.15)-273.15;
  delay(500);
  
  Serial.print("Temperatur: ");
  Serial.print(temperatur); 
  Serial.println("°C");
}

void servoSteuerungCallback(){
  
}

void blinkCallback(){
  
}

void serialTimerCallback(){
  
}


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

  //SETUP FÜR SCHEDULAR
  /*
  runner.init();
  
  runner.addTask(TASKNAME)
 
  TASKNAME.enable();
  */
}

void loop() {

runner.execute();

}

https://github.com/arkhipenko/TaskScheduler/blob/master/examples/Scheduler_example01/Scheduler_example01.ino
