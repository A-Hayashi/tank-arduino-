#include <L298N.h>
#include <Wire.h>

//pin definition
#define ENB 9
#define IN3 6
#define IN4 7

//create a motor2 instance
L298N motor2(ENB, IN3, IN4);

void setup() {

  //used for display information
  Serial.begin(9600);

  motor2.setSpeed(80); // an integer between 0 and 255
  Wire.begin(8);
  Wire.onReceive(Receive);
}

void loop() {


}

void Receive() {
  if (Wire.available() > 0)
  {
    byte input = Wire.read();
    Serial.println(input);
    motor2.setSpeed(input);
    motor2.forward();
  }
}

