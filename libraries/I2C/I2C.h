#ifndef I2C_h
#define I2C_h

#include "Arduino.h"

#define START1    			0x0A
#define START2   			0x55
#define MAX_PACKET_LENGTH 	200


/*
0:START1
1:START2
2:cType
3:len
4:dType
5-:Option
*/


enum DataType
{
	dType_None = 0,
	dType_Control,
	dType_StringMessage,
	dType_EndOfType
};


enum CommandType
{
	cType_None = 0,
	cType_Motor1,
	cType_Motor2,
	cType_EndOfType
};


enum MotorDirectionType
{
	mdType_Stop = 0,
	mdType_Forward,
	mdType_Backward,
};

extern	void I2C_begin(byte adress);	
extern	void I2C_Receive();
extern	byte Motor1Direction;
extern	byte Motor1Speed;
extern	byte Motor2Direction;
extern	byte Motor2Speed;

#endif   