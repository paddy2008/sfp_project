//-----------------------------------------------------------------------------
//      Copyright (c) 2017-2018,
//-----------------------------------------------------------------------------
//
// file name  :       I2cSlaveBuffer.h
//
// description:       I2C Slave Buffer Struct define
//
// Target     :
//
// version    :       V100.001
//
//
//-----------------------------------------------------------------------------
#ifndef I2C_SLAVE_BUFFER_H
#define I2C_SLAVE_BUFFER_H

#define A2_7Bits_ADDR  0x51//0xA2
#define A0_7Bits_ADDR  0x50//0xA0

#define I2C_WRITE_SIZE_ONCE   8


typedef enum I2C_BUFFER_STATUS_ENUM_TAG
{
   I2C_BUFFER_IDLE,
   I2C_BUFFER_WAITING_TO_PROCESS

} I2c_Buffer_Status_e;


typedef enum I2C_NEXT_FLAG_ENUM_TAG
{
	I2C_NEXT_UNKNOWN,
	I2C_NEXT_I2CREGADDRESS,
	I2C_NEXT_DATA,
	I2C_NEXT_DATA_NUM,
	I2C_NEXT_CRC,
	I2C_NEXT_ACK,
	I2C_NEXT_NACK,
	I2C_NEXT_FINISH

} I2c_Next_Flag_e;


struct I2C_BUFFER_Struct
{
	uint8_t         SubAddress;
	uint8_t         A2_RegAddress;
	uint8_t         A0_RegAddress;
	uint8_t         NumOfEntries;
	uint8_t         DataBuff[I2C_WRITE_SIZE_ONCE];

	I2c_Buffer_Status_e   Status;
	I2c_Next_Flag_e       I2cNextFlag;

};

#endif
