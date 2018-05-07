//https://blogs.yahoo.co.jp/dotallcafe/67202857.html

#include "PS_PAD.h"
#include <Servo.h>

#include "SPI.h"
#include "MFRC522.h"
#define PS2_DAT        12  //PD12(MISO)
#define PS2_CMD        11  //PD11(MOSI)
#define PS2_CLK        13  //PD13(SCK)
#define PS2_SEL        10  //PD10(SS)

#define LEAD_SW      5  //PD8(LEAD_SW)
#define SERVO_SIGNAL      9  //PD9(SERVO_SIGNAL)

#define RC522_SDA        6  //PD6(SS)
#define RC522_RESET      5  //PD5(RESET)

PS_PAD PAD(PS2_SEL);
MFRC522 rfid(RC522_SDA, RC522_RESET);
Servo servo1;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("I am waiting for card...");

  PAD.init();

  pinMode(LEAD_SW, INPUT);
  servo1.attach(SERVO_SIGNAL);
}

void loop() {
  unsigned long time = micros();
  protothread1();
  protothread2();
  while(micros()-time<300);
}


void protothread1() {
  static int deg;
  static int lead_sw;

  PAD.poll();
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
    servo1.detach();
  } else {
    servo1.write(deg);
  }

}



void protothread2() {
  static MFRC522::PICC_Type piccType;
  static String strID;

  // put your main code here, to run repeatedly:
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  piccType = rfid.PICC_GetType(rfid.uid.sak);

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }
  strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) +
      (i != 3 ? ":" : "");
  }

  strID.toUpperCase();
  Serial.print("Tap card key: ");
  Serial.println(strID);

  if (strID.indexOf("9D:16:DE:73") >= 0) {  //put your own tap card key;
    Serial.println("********************");
    Serial.println("**Authorised acces**");
    Serial.println("********************");
    return;
  }
  else {
    Serial.println("****************");
    Serial.println("**Acces denied**");
    Serial.println("****************");
    return;
  }
}




