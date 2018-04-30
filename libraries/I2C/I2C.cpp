#include "I2C.h"
#include "Arduino.h"
#include <Wire.h>

static byte cmdBuff[MAX_PACKET_LENGTH];

static byte checkHeader;
static byte cmdIndex;
static byte receiveLength;
static byte receiveDtype;
static byte receiveCtype;
static byte dataBuff[30];

byte Motor1Direction;
byte Motor1Speed;
byte Motor2Direction;
byte Motor2Speed;

static void I2C_Receive();

void I2C_begin(byte adress)
{
	Wire.begin(adress);
	Wire.onReceive(&I2C_Receive);
	Serial.begin(9600);
}

static void I2C_Receive()
{
	Serial.println("ReceiveStart");
	if (Wire.available() > 0)
	{
		int input = Wire.read();
		Serial.println(input);
		
		cmdBuff[cmdIndex++] = (char)input;

		if (cmdIndex < MAX_PACKET_LENGTH)
		{
			if (cmdIndex == 1)
			{
				if (cmdBuff[0] == START1){
					checkHeader = 1;
					Serial.println("START1");
				}
				else
				{
					checkHeader = 0;
					cmdIndex = 0;
				}
			}
			else if (checkHeader == 1)
			{
				if (cmdBuff[1] == START2){
					checkHeader = 2;
					Serial.println("START2");
				}
				else
				{
					checkHeader = 0;
					cmdIndex = 0;
				}
			}

			else if (checkHeader == 2)
			{
				if (cmdIndex == 3)
				{
					receiveDtype = cmdBuff[2];
					Serial.println("receiveDtype:"+receiveDtype);
					dataBuff[cmdIndex - 3] = cmdBuff[cmdIndex - 1];
				}
				else if (receiveDtype != dType_StringMessage)
				{
					if (cmdIndex == 4)
					{
						receiveLength = cmdBuff[3];
						Serial.println("receiveLength:"+receiveLength);
						dataBuff[cmdIndex - 3] = cmdBuff[cmdIndex - 1];
					}
					else if (cmdIndex > 4)
					{
						if (receiveDtype == dType_Control)
						{
							receiveCtype = cmdBuff[4];
							Serial.println("receiveCtype:"+receiveCtype);
							if (receiveCtype == cType_Motor1)
							{
								Motor1Direction = cmdBuff[5];
								Motor1Speed = cmdBuff[6];
								Serial.println("Motor1Direction:"+Motor1Direction);
								Serial.println("Motor1Speed:"+Motor1Speed);
							}
							else if (receiveCtype == cType_Motor2)
							{
								Motor2Direction = cmdBuff[5];
								Motor2Speed = cmdBuff[6];
								Serial.println("Motor2Direction:"+Motor2Direction);
								Serial.println("Motor2Speed:"+Motor2Speed);
							}
						}
						checkHeader = 0;
						cmdIndex = 0;
					}
				}
				else
				{
					checkHeader = 0;
					cmdIndex = 0;
				}
			}
		}
		else
		{
			checkHeader = 0;
			cmdIndex = 0;
		}
	}
	Serial.println("ReceiveEnd");
}


