#include <Wire.h>
#include <Servo.h>
#include <math.h>
#include "rgb_lcd.h"

Servo myservo;

rgb_lcd lcd;

int buttonState = 0;
int servoState = 0;
const int tempSensor = A1;
int sensorWert;
int temperatur;

const int B = 4275;
const int R0 = 100000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3, INPUT);  //Button auf D3
  pinMode(A0, INPUT); //Poti auf A0
  myservo.attach(A2); //Servo auf A2
  lcd.begin(16,2);// Initialisierung LC-Display

  lcd.print("Embedded 2");
  delay(1000);

}

void loop() {
 
  buttonState = digitalRead(3);
  
  int spannung;

  int a = analogRead(tempSensor);

  float R = 1023.0/a-1.0;
  R = R0*R;

  spannung = analogRead(A0);

  int position;
 
  position = map(spannung, 0, 1023, 0, 179);
 
  myservo.write(position);
 
  //delay(500);


  //Serial.println(buttonState);
  if (buttonState == HIGH) {
    //delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else{
    digitalWrite(LED_BUILTIN, LOW);
  }
  sensorWert = analogRead(tempSensor);
  temperatur = map(sensorWert, 0, 410, -40, 125);

  float temp2= 1.0/(log(R/R0)/B+1/298.15)-273.15;

  Serial.println(temp2);
  //delay(1000);
  Serial.println(temperatur);
  //delay(1000);

  lcd.setCursor(0, 1);

  lcd.print(temp2);
  //delay(500);
}
