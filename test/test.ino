#include <L298N.h>
#include <I2C.h>

#define ENA 3 //PD3
#define IN1 2 //PD2
#define IN2 4 //PD4

#define ENB 6 //PD6
#define IN3 7 //PD7
#define IN4 8 //PD8

L298N motor1(ENA, IN1, IN2);
L298N motor2(ENB, IN3, IN4);
//I2C_Class I2C;

void setup() {
  Serial.begin(9600);
  I2C_begin(8);
}

void loop() {
  Serial.print("\tMotor1Direction: ");
  Serial.print(Motor1Direction);
  Serial.print("\tMotor1Speed: ");
  Serial.print(Motor1Speed);
  Serial.print("\tMotor2Direction: ");
  Serial.print(Motor2Direction);
  Serial.print("\tMotor2Speed: ");
  Serial.println(Motor2Speed);

  switch (Motor1Direction) {
    case mdType_Stop:
      motor1.setSpeed(0);
      motor1.stop();
      break;
    case mdType_Forward:
      motor1.setSpeed(Motor1Speed);
      motor1.forward();
      break;
    case mdType_Backward:
      motor1.setSpeed(Motor1Speed);
      motor1.backward();
      break;
    default:
      motor1.setSpeed(0);
      motor1.stop();
      break;
  }

  switch (Motor2Direction) {
    case mdType_Stop:
      motor2.setSpeed(0);
      motor2.stop();
      break;
    case mdType_Forward:
      motor2.setSpeed(Motor2Speed);
      motor2.forward();
      break;
    case mdType_Backward:
      motor2.setSpeed(Motor2Speed);
      motor2.backward();
      break;
    default:
      motor2.setSpeed(0);
      motor2.stop();
      break;
  }
}

