#ifndef I2C_h
#define I2C_h

#include "Arduino.h"

#define START1    			0x0A
#define START2   			0x55
#define MAX_PACKET_LENGTH 	200



enum DataType
{
	dType_None = 0,
	dType_StringMessage,
	dType_EndOfType
};


enum CommandType
{
	cType_None = 0,
	cType_EndOfType
};


class I2C_Class
{
public:

/////////////////////////////////////////////////////////////////////////

	void begin(long baud);	
	
	void Receive(void);
		
	void Send_Command(int sendCommand, int sendOption);	
	void Send_Processing(byte _data[], byte _length, byte _crc[]);
	
	unsigned short CRC16_Make(unsigned char *buf, int len); //CRC16-CCITT Format
	boolean CRC16_Check(unsigned char data[], int len, unsigned char crc[]);
	
	byte cmdBuff[MAX_PACKET_LENGTH];

	byte checkHeader;
	byte cmdIndex;
	byte receiveDtype;
	byte receiveLength;
	
	int receiveComplete;

	byte dataBuff[30];
	
};

extern I2C_Class I2C;

#endif   