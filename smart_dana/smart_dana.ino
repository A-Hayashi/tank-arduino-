#include <pt.h> // ProtoThread Library
#include <clock.h> // Library for the CLOCK_SECOND constant
#include <timer.h> // Libery for the timer
//https://blogs.yahoo.co.jp/dotallcafe/67202857.html

#include "PS_PAD.h"
#include <Servo.h>

#include "SPI.h"
#include "MFRC522.h"

static struct pt pt1, pt2;


#define PT_WAIT(pt, timestamp, usec) PT_WAIT_UNTIL(pt, millis() - *timestamp > usec);*timestamp = millis();



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

  //  pinMode(RC522_SDA, OUTPUT);
  //  digitalWrite(RC522_SDA, HIGH);
  //  pinMode(RC522_RESET, OUTPUT);
  //  digitalWrite(RC522_RESET, LOW);

  PT_INIT(&pt1);
  PT_INIT(&pt2);
}

void loop() {
 protothread1(&pt1);
 protothread2(&pt2);
}


PT_THREAD(protothread1(struct pt *pt)) {
  static unsigned long timestamp = 0;
  static int deg;
  static int lead_sw;
  PT_BEGIN(pt);

  while (1) {
    PT_WAIT(pt, &timestamp, 50);

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
      //PT_WAIT(pt, &timestamp, 2000);
      servo1.detach();
    } else {
      servo1.write(deg);
    }
  }
  PT_END(pt);
}



PT_THREAD(protothread2(struct pt *pt)) {
  static unsigned long timestamp = 0;
  static MFRC522::PICC_Type piccType;
  static String strID;

  PT_BEGIN(pt);
  while (1) {
    // put your main code here, to run repeatedly:
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()){
      PT_WAIT(pt, &timestamp, 10);
      PT_RESTART(pt);
    }
    
    piccType = rfid.PICC_GetType(rfid.uid.sak);
    
    // Check is the PICC of Classic MIFARE type
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
      Serial.println(F("Your tag is not of type MIFARE Classic."));
      PT_WAIT(pt, &timestamp, 10);
      PT_RESTART(pt);
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
    PT_WAIT(pt, &timestamp, 1000);

    if (strID.indexOf("9D:16:DE:73") >= 0) {  //put your own tap card key;
      Serial.println("********************");
      Serial.println("**Authorised acces**");
      Serial.println("********************");
      PT_WAIT(pt, &timestamp, 5000);
      PT_RESTART(pt);
    }
    else {
      Serial.println("****************");
      Serial.println("**Acces denied**");
      Serial.println("****************");
      PT_WAIT(pt, &timestamp, 5000);
      PT_RESTART(pt);
    }
  }
  PT_END(pt);
}



