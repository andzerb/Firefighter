#include <Servo.h>

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
const float pi = 3.1415926535;
const int threshold = 700;
const int stallPins[] = {
  A4,A5,A6,A7};
const int motorPins[] = {
  2,4,3,5};
const int directionPins[] = {
  50,51,53,49};
const int switchPins[] = {
  26,27,28,29}; 
const boolean xInvert = true;
const boolean yInvert = true;

void setup(){
   // initialize serial:
  Serial.begin(115200);
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
  for(int i = 0; i < 4; i++){
    pinMode(directionPins[i],OUTPUT);
  }
  for(int i = 0; i < 4; i++){
    pinMode(motorPins[i],OUTPUT);
  }
  for(int i = 0; i < 4; i++){
    pinMode(stallPins[i],INPUT);
  }
  for(int i = 0; i < 4; i++){
    pinMode(switchPins[i],INPUT);
  }
}
void loop()
{
  Move(60,100);
  delay(100);
  Move(0,0);
  delay(100);
  
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
boolean Move(int Degrees, int power){
  power =  power * 255/100;
  Serial.print(degreetoRad(Degrees));
  Serial.print(" ");
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
  Serial.print(x);
  Serial.print(" ");
  Serial.println(y);
  digitalWrite(directionPins[0], xB);
  digitalWrite(directionPins[1], yB);
  digitalWrite(directionPins[2], xB);
  digitalWrite(directionPins[3], yB);
  analogWrite(motorPins[0], x);
  analogWrite(motorPins[1], x);
  analogWrite(motorPins[2], y);
  analogWrite(motorPins[3], y);

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

int flameSensor(int pin){
  int value = analogRead(pin);
  int voltage = value * 5/1.024;
  return voltage;
}
String wordAfter(String data, int index){
  int index2 = data.indexOf(' ', index+1);
  //Serial.println(index2, DEC);
  String wordAfter = data.substring(index,index2);
  return(wordAfter);
}

int pingSensor(int pingPin){
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  return cm;
}



long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}
long microsecondsToCentimeters(long microseconds)

{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
double degreetoRad(double Degrees){
  return 2* pi *Degrees/360; 
}

