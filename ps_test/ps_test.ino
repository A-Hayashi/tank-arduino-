#include "PS_PAD.h"

#define PS2_DAT        12  //PD12(MISO)
#define PS2_CMD        11  //PD11(MOSI)
#define PS2_CLK        13  //PD13(SCK)
#define PS2_SEL        10  //PD10(SS)

PS_PAD PAD(PS2_SEL);

void setup()
{
  Serial.begin(9600);
  PAD.init();
}

void loop() {
  PAD.poll();
  Serial.println(PAD.read(PS_PAD::ANALOG_RX));
}
