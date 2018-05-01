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
	Wire.onReceive(I2C_Receive);
	Serial.begin(9600);
}

void I2C_Receive()
{
	if (Wire.available() > 0)
	{
		
		int input = Wire.read();	
		cmdBuff[cmdIndex++] = (char)input;

		if (cmdIndex < MAX_PACKET_LENGTH)
		{
			if (cmdIndex == 1)
			{
				if (cmdBuff[0] == START1){
					checkHeader = 1;
//					Serial.print("START1: ");
//					Serial.println(cmdBuff[0]);
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
//					Serial.print("START2: ");
//					Serial.println(cmdBuff[1]);
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
//					Serial.print("receiveDtype: ");
//					Serial.println(receiveDtype);
					
					dataBuff[cmdIndex - 3] = cmdBuff[cmdIndex - 1];
				}
				else if (receiveDtype != dType_StringMessage)
				{
					if (cmdIndex == 4 && cmdIndex)
					{
						receiveLength = cmdBuff[3];
//						Serial.print("receiveLength: ");
//						Serial.println(receiveLength);

						dataBuff[cmdIndex - 3] = cmdBuff[cmdIndex - 1];
					}
					else if (cmdIndex > 4)
					{
						
						if (receiveLength + 3 > cmdIndex){
							dataBuff[cmdIndex - 3] = cmdBuff[cmdIndex - 1];
						}else if (receiveLength + 4 <= cmdIndex){
								
							if (receiveDtype == dType_Control)
							{
								receiveCtype = cmdBuff[4];
//								Serial.print("receiveCtype: ");
//								Serial.println(receiveCtype);
								
								if (receiveCtype == cType_Motor1)
								{
									Motor1Direction = cmdBuff[5];
									Motor1Speed = cmdBuff[6];
								
//									Serial.print("Motor1Direction: ");
//									Serial.println(Motor1Direction);
									
//									Serial.print("Motor1Speed: ");
//									Serial.println(Motor1Speed);
								}
								else if (receiveCtype == cType_Motor2)
								{
									Motor2Direction = cmdBuff[5];
									Motor2Speed = cmdBuff[6];
						
//									Serial.print("Motor2Direction: ");
//									Serial.println(Motor2Direction);
									
//									Serial.print("Motor2Speed: ");
//									Serial.println(Motor2Speed);
								}
							}
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
		}
		else
		{
			checkHeader = 0;
			cmdIndex = 0;
		}

	}
}