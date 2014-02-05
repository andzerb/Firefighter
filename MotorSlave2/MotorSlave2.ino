#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

#include <Encoder.h>
#include <Servo.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

boolean Move(int, int, int);
void motors(int, int);

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);
Encoder enc1(22,2);
Encoder enc2(24,3);
Encoder enc3(26,18);
Encoder enc4(28,19);
const float pi = 3.1415926535;
const float mmpercount = 0.08886;
const int threshold = 700;
const boolean xInvert = true;
const boolean yInvert = true;

boolean moving = false;
int wheelPowers[] = {0,0,0,0};
long prevSpeeds[] = {0,0,0,0};
int xreq = 0;
int yreq = 0;
int mSpeed = 200;

void setup(){
   // initialize serial:
  Serial.begin(115200);
  AFMS.begin();
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  for(int i = 2; i <=5; i++){
    
   pinMode(i, INPUT);
    
  }
  enc1.write(0);
  enc2.write(0);
  enc3.write(0);
  enc4.write(0);
  xreq = 500;
  yreq = 500;
  for(int i = 0; i < 4; i++){wheelPowers[i] = 150;}
  delay(1000);
 
}
void loop()
{ 
  delay(100);
  if(!(xreq == 0 && yreq == 0)){        
    long readSpeeds[] ={enc1.read(), enc2.read(), enc3.read(), enc4.read()};
    float ximbalance = (readSpeeds[0]-prevSpeeds[0] + 0.01)/(readSpeeds[3]-prevSpeeds[3] + 0.01);
    float yimbalance = (readSpeeds[1]-prevSpeeds[1] + 0.01)/(readSpeeds[4]-prevSpeeds[2] + 0.01);
    if(wheelPowers[2]*ximbalance <= 255){wheelPowers[2]*= ximbalance;}
    if(wheelPowers[3]*ximbalance <= 255){wheelPowers[3]*= ximbalance;}
    for(int i = 0; i < 4; i++){
     prevSpeeds[i] = readSpeeds[i]; 
    }
    Serial.println(readSpeeds[0]);
    if(readSpeeds[0] >= xreq/mmpercount){
      motor1->run(RELEASE);
      motor1->setSpeed(0);
    }else{
      motor1->run(FORWARD);
      motor1->setSpeed(wheelPowers[0]); 
    }
    if(readSpeeds[1] >= yreq/mmpercount){      
      motor2->run(RELEASE);
      motor2->setSpeed(0);
    }else{
      motor2->run(FORWARD);
      motor2->setSpeed(wheelPowers[1]); 
    }
    if(readSpeeds[2] >= xreq/mmpercount){
      motor3->run(RELEASE);
      motor3->setSpeed(0);
    }else{
      motor3->run(FORWARD);
      motor3->setSpeed(wheelPowers[2]); 
    }
    if(readSpeeds[3] >= yreq/mmpercount){
      motor4->run(RELEASE);
      motor4->setSpeed(0);      
    }else{
      motor4->run(FORWARD);
      motor4->setSpeed(wheelPowers[3]); 
    }
    
    if(readSpeeds[0] >= xreq/mmpercount && readSpeeds[1] >= yreq/mmpercount && readSpeeds[2] >= xreq/mmpercount && readSpeeds[3] >= yreq/mmpercount){
      motor1->run(RELEASE);
      motor2->run(RELEASE);
      motor3->run(RELEASE);
      motor4->run(RELEASE);
      motor1->setSpeed(0);
      motor2->setSpeed(0);
      motor3->setSpeed(0);
      motor4->setSpeed(0);
      xreq = 0; yreq = 0; 
      enc1.write(0);
      enc2.write(0);
      enc3.write(0);
      enc4.write(0);
    }    
  }
  
}
boolean serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      return true;
    } 
  }
}

