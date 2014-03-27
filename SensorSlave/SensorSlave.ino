#include <Servo.h>

const int switchPins[4] = {
  2, 3, 4, 5
};
const int pingPins[2][4] = {
  {6, 7, 8, 9},
  {10, 11, 12, 13}  
};
const int flamePins[2] = {
  A0, A1
};
const int lightPins[4] = {
  A2, A3, A4, A5
};
const int servoPin = 8;

const boolean xInvert = true;
const boolean yInvert = true;
const int flameThreshold = 700;

int cycle = 0;
Servo turret;

void setup() {
  Serial.begin(115200);
  turret.attach(servoPin);
  for (int i = 0; i < 4; i++) {
    pinMode(switchPins[i], INPUT);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(pingPins[0][i], INPUT);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(pingPins[1][i], OUTPUT);
  }
  for (int i = 0; i < 2; i++) {
    pinMode(flamePins[i], INPUT);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(lightPins[i], INPUT);
  }
  Serial.println();
}
void loop()
{
  Serial.println(cycle);
  Serial.print("Okay "); // If the brain sees anything different than a fully spelled okay, with a capital O and a space, the line will be rejected.
  // Serious stuff goes here!
  turret.write(0);
  delay(500);
  Serial.print("F1L"); Serial.print(flameSensor(flamePins[0])); // Start with the first flame sensor ("f"), the one to the left ("l"). The orientation is for debug purposes.
  turret.write(180);
  delay(500);
  Serial.print("F2R"); Serial.print(flameSensor(flamePins[0])); // Then print the second flame sensor's reading
  
  for(int i = 0; i < 4; i++){
    Serial.print("S"); Serial.print(i + 1); Serial.print(digitalRead(switchPins[i])); // I don't need orientation on any others because the 1 refers to the top as protocol.
  }
  for(int i = 0; i < 4; i++){
    Serial.print("P"); Serial.print(i + 1); Serial.print(SR04(pingPins[0][i],pingPins[1][i])); // P is for proximity, the two pins are necessary in SR04 type devices.
  }
  for(int i = 0; i < 4; i++){
    Serial.print("L"); Serial.print(i + 1); Serial.print(analogRead(lightPins[i])); // L is for light sensor and units are percentages.
  }
  //End of serious stuff
  Serial.println(); // A nl is the delimiter for a new "packet", so to speak
}
int flameSensor(int pin) {
  int value = analogRead(pin);
  int voltage = value * 5 / 1.024;
  return voltage;
}

int pingSensor(int pingPin) {
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
int SR04 (int outPin, int inPin) {
   // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(outPin, LOW);
  delayMicroseconds(2);
  digitalWrite(outPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(outPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(inPin, HIGH, 50000);

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
