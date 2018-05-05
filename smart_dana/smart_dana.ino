#include "PS_PAD.h"
#include <Servo.h>

#define PS2_DAT        12  //PD12(MISO)
#define PS2_CMD        11  //PD11(MOSI)
#define PS2_CLK        13  //PD13(SCK)
#define PS2_SEL        10  //PD10(SS)

PS_PAD PAD(PS2_SEL);

Servo servo1;

void setup()
{
  Serial.begin(9600);
  PAD.init();
  pinMode(8, INPUT);

  servo1.attach(9);
}

void loop() {
  PAD.poll();

  int deg = 0;
  int lead_sw = false;

  lead_sw = digitalRead(8);

  deg = PAD.read(PS_PAD::ANALOG_RX);
  Serial.print(lead_sw);
  Serial.print("\t");
  Serial.print(deg);
  Serial.print("\t");
  deg = map(deg, -128, 127, 0, 180);
  Serial.println(deg);


  if (PAD.read(PS_PAD::PAD_CIRCLE)) {
    servo1.attach(9);
  } else if (PAD.read(PS_PAD::PAD_X)) {
    servo1.detach();
  }

  if (lead_sw == true) {
    servo1.attach(9);
    servo1.write(180);
    delay(2000);
    servo1.detach();
  } else {
    servo1.write(deg);
  }
}
