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
#include <OneButton.h>


Servo myservo;
rgb_lcd lcd;

// Callback methods prototypes
void t1Callback();
void t2Callback();
void t3Callback();
void t4Callback();
void t5Callback();
void t6Callback();
/*
//Servo und LC-Display
Servo myservo;
rgb_lcd lcd;*/

Task t1(1000, TASK_FOREVER, &t1Callback);
Task t2(100, TASK_FOREVER, &t2Callback);
Task t3(1000, TASK_FOREVER, &t3Callback);
Task t4(3000, TASK_FOREVER, &t4Callback);
Task t5(1000, TASK_FOREVER, &t5Callback);
Task t6(100, TASK_FOREVER, &t6Callback);

Scheduler runner;

int time1;
int buttonState = 0;
int servoState = 0;
int sensorWert;
const int B = 4275;
const int R0 = 100000;
const int tempSensor = A1;

volatile int butt = 0;
volatile int zeitstempel;
volatile int spannung;
volatile int position;
volatile int temperatur;
volatile int stunde = 0;
volatile int minute = 0;
volatile int sekunde = 0;
volatile int tag = 0;
volatile int last = 0;
volatile int clicktimer;
volatile int quitlong = 0;
volatile int quitshort = 0;
volatile int uhrtimer;

void t1Callback(){
  digitalWrite(LED_BUILTIN, LOW);

  if (butt == 1){    
    runner.addTask(t2);
    t2.enable();
    }
  else if (butt == 0){
    t2.disable();
    runner.deleteTask(t2);
    }
  if (butt == 2){
    t5.enable();
  }
}

void t2Callback(){

  if ((millis() <= zeitstempel) && (butt != 0)){
    t4.disable();
    t5.disable();
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print("Eingestellte Temperatur:");
    spannung = analogRead(A0);
    position = map(spannung, 0, 1023, 15, 40);
    Serial.println(position);  
    }
  else {
    butt = 0;
    t4.enable();
    t5.enable();
    //Serial.println("t4 enable");
    }

}

void t3Callback(){
  sekunde++;
  if (sekunde > 59){
    sekunde = 0;
    minute++;
    if (minute >59){
      minute = 0;
      stunde++;
      if (stunde > 23){
        stunde =0;
        tag++;
      }
      if (tag > 6){
        tag = 0;
      }
    }
  }
  int time[2] = {0, 0};
  time[1] = stunde;
  time[2] = minute;
  char wochentag[10] = "Testtag";

  


  switch (tag){
    case 0:
    strcpy(wochentag,"Montag");
    break;

    case 1:
    strcpy(wochentag,"Dienstag");
    break;

    case 2:
    strcpy(wochentag,"Mittwoch");
    break;

    case 3:
    strcpy(wochentag,"Donnerstag");
    break;

    case 4:
    strcpy(wochentag,"Freitag");
    break;

    case 5:
    strcpy(wochentag,"Samstag");
    break;

    case 6:
    strcpy(wochentag,"Sonntag");
    break;
  }
  Serial.println(wochentag);
  char str[100];
  
  //lcd.print(str,"%d:%d:%d", stunde, minute, sekunde);
  Serial.println((String)stunde + ":" + minute + ":" + sekunde);
  // Serial.print(stunde);
  //Serial.println(minute);
}

void t4Callback(){
  sensorWert = analogRead(tempSensor);

  float R = 1023.0/sensorWert-1.0;
  R = R0*R;

  float temperatur = 1.0/(log(R/R0)/B+1/298.15)-273.15;
  delay(500);
  
  if (temperatur < position){
    myservo.write(160);
    Serial.println("Ventil auf! Zu kalt.");
   }
  if (((position-1)<=temperatur) && (temperatur<=(position+1))){
    myservo.write(90);
    Serial.println("Temp erreicht");
    }
  if (temperatur > position){
    myservo.write(20);
    Serial.println("Ventil zu! Zu warm.");
    }
}

void t5Callback(){
  
  if(butt == 2){
    //Serial.println("Zeit einstellen");
    t1.disable();
    t2.disable();
    t6.disable();
    if ((uhrtimer+10000)>millis()){
      tag += digitalRead(3);
      if (tag>6){
        tag=0;        
      }
      delay(100);
    }
    if ((uhrtimer+20000)>millis()&& millis()>(uhrtimer+10000)){
      stunde += digitalRead(3);
      if (stunde>23){
        stunde=0;
      }
      delay(100);
    }
    if ((uhrtimer+30000)>millis()&& millis()>(uhrtimer+20000)){
      minute += digitalRead(3);
      if (minute>59){
        minute=0;
      }
      delay(100);
    }
    if (uhrtimer+30000<millis()){
      quitlong=0;
    }
   // printDatum();
  }
  t1.enable();
  t2.enable();
  t6.enable();
}

void t6Callback(){
 /* lcd.print(temperatur);
  delay(4000);
  //lcd.print()*/
  
}

void setup() {
  
  //Initialisierung der Baud-Rate für die serielle Schnittstelle
  Serial.begin(9600);

  //Belegung der Pins auf dem Shield
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3, INPUT);  //Button-D3
  pinMode(A0, INPUT); //Poti-A0
  pinMode(2, INPUT); // Touchsensor
  myservo.attach(A2); //Servo-A2
  //Initialisierung LC-Displays
  lcd.begin(16,2);// Initialisierung LC-Display
  //Startbildschirm beim Booten des qC
  lcd.print("Thermostat");
  
  runner.init();
  
  runner.addTask(t1);
  runner.addTask(t3);
  runner.addTask(t4);
  runner.addTask(t5);
  runner.addTask(t6);

  t1.enable();
  t3.enable();
  t4.enable();
  t5.enable();
  t6.enable();

  attachInterrupt(digitalPinToInterrupt(3), taster, RISING);
}

//ISR Tastervariable
void taster(){
  clicktimer = millis();
  
  if ((last == 0)&&(quitlong==0)){
    uhrtimer = millis();
    last = 1;
  }
  else if (last == 1){
    last = 0;
  }
   // Serial.print("quitlong isr: ");
   // Serial.println(quitlong);
  if (butt == 1){
    Serial.println("1. if");
    butt = 0;
    last = 0;
  }
  if ((butt == 2)&&(quitlong==0)){
    Serial.println("2. if");
    butt = 0;
    last = 0;
  }
 // Serial.print("quitlong end of isr: ");
  //  Serial.println(quitlong);
  zeitstempel = millis() + 10000;
}

void loop() {
  runner.execute();
  if (last == 1){
   // Serial.print("QUITLONG WERT VOR KURZEN CLICK");
   // Serial.println(quitlong);
    if ((digitalRead(3)==0)&&(clicktimer+500)>millis()){
      Serial.println("Short Click");
      butt=1;
      last=0;
      Serial.println("butt loop short click");
      Serial.println(butt);
    }
    if ((digitalRead(3)==0)&&(clicktimer+500)<=millis()){
      Serial.println("Long Click");
      butt=2;
      last=0;
      if (quitlong==1){
        quitlong=0;
      }
      else if (quitlong==0){
        quitlong=1;
        Serial.print("quitlong im loop: ");
    Serial.println(quitlong);
      }
    
      
    Serial.print("quitlong nach loop: ");
    Serial.println(quitlong);
      }
  }
}
/*
  switch (tag){
    case 0:
    wochentag[] = "Montag"
    break;

    case 1:
    wochentag[] = "Dienstag"
    break;

    case 2:
    wochentag[] = "Mittwoch"
    break;

    case 3:
    wochentag[] = "Donnerstag"
    break;

    case 4:
    wochentag[] = "Freitag"
    break;

    case 5:
    wochentag[] = "Samstag"
    break;

    case 6:
     wochentag[] = "Sonntag"
    break;
  }
}
 void printDatum(){
  char temp[50];

  char wo[2] = checkWochentag();
  temp[0] = wo[0];
  temp[1] = wo[1];

  if (stunde < 10){
    Serial.print("0");
  }
  Serial.print(stunde);
  Serial.print(":");
  if (minute < 10){
    Serial.print("0");
  }
  Serial.print(minute);
  Serial.print(":");
  if (sekunde < 10){
    Serial.print("0");
  }
}*/