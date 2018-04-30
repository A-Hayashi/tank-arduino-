#include <L298N.h>
#include <I2C.h>

#define ENB 9
#define IN3 6
#define IN4 7

L298N motor2(ENB, IN3, IN4);
//I2C_Class I2C;

void setup() {
  Serial.begin(9600);
  I2C_begin(8);
}

void loop() {
//  I2C_Receive();
//  Serial.print(Motor1Direction);
//  Serial.print(Motor2Direction);
//  Serial.print(Motor1Speed);
//  Serial.print(Motor2Speed);
}
