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
Encoder enc1(2,4);
Encoder enc2(10,11);
Encoder enc3(3,5);
Encoder enc4(12,13);
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
  xreq = 50;
  yreq = 0;
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
    
    
  }else{
    
    
    
    
  }
}/*
void placeholder2(){
   analogWrite(motorPins[0],power1);
    analogWrite(motorPins[2],power3); digitalWrite(directionPins[2],HIGH);
    delay(100);
    read1=wheel1.read();
    read3=wheel3.read();
    imbalance = (read1-prev1 + 0.01)/(read3-prev3 + 0.01);
    if(power3*imbalance <= 255){power3*= imbalance;}
    Serial.print(read1-prev1);Serial.print(", "); Serial.print(read3-prev3);Serial.print(", "); Serial.print(imbalance);Serial.print(", "); Serial.println(power3);
    /*analogWrite(motorPins[2], 150);
    Serial.println(wheel3.read());
    delay(100);
    prev1 = read1;
    prev3 = read3; 
  
  
}
void placehoder(){
  
      while(wheel1.read() < 50/mmpercount){
        digitalWrite(32, HIGH);
          analogWrite(2,155);
        delay(100);
        Serial.println(wheel1.read());
      }
    
  
}
void motorsStalled(boolean *stalled, int pins[], int threshold){
  for(int i = 0; i < 4; i++){
    if(analogRead(pins[i]) >= threshold){
      stalled[i] = true;
    }
    else{
      stalled[i] = false;
    }
  }
}
void motors(int Degrees, int power){
  power =  power * 255/100;
  int x = cos(degreetoRad(Degrees)) * power;
  int y = sin(degreetoRad(Degrees)) * power;
  boolean xB = xInvert; //true if not changed
  boolean yB = yInvert;
  if(x<0){ // invert the directions (make them false) to go backward
    xB *= -1;
  }
  if(y<0){
    yB *= -1;
  }
  digitalWrite(directionPins[0], xB);
  digitalWrite(directionPins[1], yB);
  digitalWrite(directionPins[2], xB);
  digitalWrite(directionPins[3], yB);
  ]  motor1->run(FORWARD);
  motor2->run(FORWARD);
  motor3->run(FORWARD);
  motor4->run(FORWARD);
}
void Drive(int m1, int m2, int m3, int m4){
  analogWrite(motorPins[0], m1 * 255/100);
  analogWrite(motorPins[1], m2 * 255/100);
  analogWrite(motorPins[2], m3 * 255/100);
  analogWrite(motorPins[3], m4 * 255/100);
}

boolean Move(int Degrees, int power, int distance){
   wheel1.write(0);
   wheel2.write(0);
   wheel3.write(0);
   wheel4.write(0);
   boolean done = false;
   int x = cos(degreetoRad(Degrees)) * distance;
   int y = sin(degreetoRad(Degrees)) * distance;
   int Pow1 = cos(degreetoRad(Degrees)) * power;
   int Pow3 = Pow1;
   int Pow2 = sin(degreetoRad(Degrees)) * power;
   int Pow4 = Pow2;
   x*= 1/mmpercount;
   y*= 1/mmpercount;
   int old1 = 0;
   int old2 = 0;
   int old3 = 0;
   int old4 = 0;
   motors(Degrees, power);
   while(done == false){ 
     int new1 = wheel1.read();
     int new2 = wheel2.read();
     int new3 = wheel3.read();
     int new4 = wheel4.read();
     int xImbalance = (new1-old1)/(new3-old3);
     Pow3 = Pow3 * xImbalance;
     int yImbalance = (new2-old2)/(new4-old4);
     Pow4 = Pow4 * yImbalance;
     Drive(Pow1,Pow2,Pow3,Pow4);
     Serial.println(new1);
     Serial.println(new2);
     Serial.println(new3);
     Serial.println(new4);
     if(new1 >= x){analogWrite(motorPins[0],0);Serial.println("1");}
     if(new2 >= y){analogWrite(motorPins[1],0);Serial.println("2");}
     if(new3 >= x){analogWrite(motorPins[2],0);Serial.println("3");}
     if(new4 >= y){analogWrite(motorPins[3],0);Serial.println("4");}
     if(new1 >= x && new2 >= y && new3 >= x && new4 >= y){
       Drive(0,0,0,0);   
       done=true;
      Serial.println("dfasdfasd"); 
     }
     old1 = wheel1.read();
     old2 = wheel2.read();
     old3 = wheel3.read();
     old4 = wheel4.read();
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
String wordAfter(String data, int index){
  int index2 = data.indexOf(' ', index+1);
  //Serial.println(index2, DEC);
  String wordAfter = data.substring(index,index2);
  return(wordAfter);
}
double degreetoRad(double Degrees){
  return 2* pi *Degrees/360; 
}*/

