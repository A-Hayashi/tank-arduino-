#include "PS_PAD.h"
#include <Servo.h>

#define PS2_DAT        12  //PD12(MISO)
#define PS2_CMD        11  //PD11(MOSI)
#define PS2_CLK        13  //PD13(SCK)
#define PS2_SEL        10  //PD10(SS)

#define LEAD_SW      5  //PD8(LEAD_SW)

#define SERVO_SIGNAL      9  //PD9(SERVO_SIGNAL)


#define RC522_SDA        6  //PD6(SS)
#define RC522_RESET      5  //PD5(RESET)


PS_PAD PAD(PS2_SEL);

Servo servo1;

void setup()
{
  Serial.begin(9600);
  PAD.init();
  pinMode(LEAD_SW, INPUT);

  pinMode(RC522_SDA, OUTPUT);
  digitalWrite(RC522_SDA, HIGH);
  pinMode(RC522_RESET, OUTPUT);
  digitalWrite(RC522_RESET, LOW);
  
  servo1.attach(SERVO_SIGNAL);
}

void loop() {
  PAD.poll();

  int deg = 0;
  int lead_sw = false;

  lead_sw = digitalRead(LEAD_SW);

  deg = PAD.read(PS_PAD::ANALOG_RX);
  Serial.print(lead_sw);
  Serial.print("\t");
  Serial.print(deg);
  Serial.print("\t");
  deg = map(deg, -128, 127, 0, 180);
  Serial.println(deg);


  if (PAD.read(PS_PAD::PAD_CIRCLE)) {
    servo1.attach(SERVO_SIGNAL);
  } else if (PAD.read(PS_PAD::PAD_X)) {
    servo1.detach();
  }

  if (lead_sw == true) {
    servo1.attach(SERVO_SIGNAL);
    servo1.write(180);
    delay(2000);
    servo1.detach();
  } else {
    servo1.write(deg);
  }
}
