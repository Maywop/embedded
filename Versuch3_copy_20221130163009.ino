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




// Callback methods prototypes
void t1Callback();
void t2Callback();
/*
//Servo und LC-Display
Servo myservo;
rgb_lcd lcd;*/

Task t1(1000, TASK_FOREVER, &t1Callback);
Task t2(21000, TASK_FOREVER, &t2Callback);

Scheduler runner;
/*
int time1;
int buttonState = 0;
int servoState = 0;
int sensorWert;
volatile int butt = 0;

const int B = 4275;
const int R0 = 100000;
const int tempSensor = A1;
*/
volatile int butt = 0;

void t1Callback(){
  Serial.println("Task1");

if(digitalRead(3)){
  butt = 1;
}
  butt = digitalRead(3);

  Serial.println(butt);
  if (butt == 1){
    runner.addTask(t2);
    t2.enable();
  }
  else if (butt == 0){
    t2.disable();
    runner.deleteTask(t2);
  }
}

void t2Callback(){
  Serial.println("Hallo ich bin t2");
  int zeitstempel= millis() + 20000;


  }
}

void setup() {
  
  //Initialisierung der Baud-Rate für die serielle Schnittstelle
  Serial.begin(9600);

  //Belegung der Pins auf dem Shield
 /* pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3, INPUT);  //Button-D3
  pinMode(A0, INPUT); //Poti-A0
  myservo.attach(A2); //Servo-A2


  //Initialisierung LC-Displays
  //lcd.begin(16,2);// Initialisierung LC-Display

  //Startbildschirm beim Booten des qC
  lcd.print("Embedded 2");*/

  pinMode(3, INPUT);  //Button-D3

  runner.init();
  
  runner.addTask(t1);
 
  t1.enable();
  Serial.println("t1 enabled");
}

void loop() {

  runner.execute();

}

//https://github.com/arkhipenko/TaskScheduler/blob/master/examples/Scheduler_example01/Scheduler_example01.ino