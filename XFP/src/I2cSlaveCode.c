//-----------------------------------------------------------------------------
//      Copyright (c) 2017-2018,
//-----------------------------------------------------------------------------
//
// file name  :       I2cSlaveCode.c
//
// description:       I2c slave state process
//
// Target     :       EFM8LB12F32ES0
//
// version    :       V100.001
//
//
//-----------------------------------------------------------------------------
#include "include.h"

// �����ݱ�ṹ
SI_SEGMENT_VARIABLE( ddm_Lower,  	extern struct LOWER_MEMORY_128_struct, 				SI_SEG_XDATA );  //A2 lower 128 bytes

// Ԥȡ����
SI_SEGMENT_VARIABLE( PrefetchData_A2, extern uint8_t, SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( PrefetchData_A0, extern uint8_t, SI_SEG_XDATA );

// I2C��������
SI_SEGMENT_VARIABLE( I2c_Buffer, extern struct I2C_BUFFER_Struct, SI_SEG_XDATA );


//-----------------------------------------------------------------------------
// I2c_Slave_Process_Address_Write
//-----------------------------------------------------------------------------
//
// Return Value:  uint16_t
// Parameters:    uint16_t
//
// Configures     I2c_Slave_Process_Address_Write
//
//-----------------------------------------------------------------------------
void I2c_Slave_Process_Address_Write(void)
{
	I2c_Buffer.SubAddress   = I2C0DIN;
	I2c_Buffer.I2cNextFlag  = I2C_NEXT_I2CREGADDRESS;
	I2c_Buffer.NumOfEntries = 0;
}


//-----------------------------------------------------------------------------
// I2c_Slave_Process_Address_Read
//-----------------------------------------------------------------------------
//
// Return Value:  uint16_t
// Parameters:    uint16_t
//
// Configures     I2c_Slave_Process_Address_Read
//
//-----------------------------------------------------------------------------
void I2c_Slave_Process_Address_Read(void)
{
	I2c_Buffer.SubAddress   = I2C0DIN;
	I2c_Buffer.I2cNextFlag  = I2C_NEXT_I2CREGADDRESS;
	I2c_Buffer.NumOfEntries = 0;

	if( I2c_Buffer.SubAddress == A0_7Bits_ADDR ||
		I2c_Buffer.SubAddress == A2_7Bits_ADDR )
	{
		I2c_Slave_Process_Data_Read_ACK();
	}
}


//-----------------------------------------------------------------------------
// I2c_Slave_Process_BusError
//-----------------------------------------------------------------------------
//
// Return Value:  uint16_t
// Parameters:    uint16_t
//
// Configures     I2c_Slave_Process_BusError
//
//-----------------------------------------------------------------------------
void I2c_Slave_Process_BusError(void)
{
	I2c_Buffer.NumOfEntries = 0;
	I2c_Buffer.Status = I2C_BUFFER_IDLE;
}


//-----------------------------------------------------------------------------
// I2c_Slave_Process_Data_Write
//-----------------------------------------------------------------------------
//
// Return Value:  void
// Parameters:    void
//
// Configures     I2c_Slave_Process_Data_Write
//
//-----------------------------------------------------------------------------
void I2c_Slave_Process_Data_Write(void)
{
	uint8_t read_data;

	read_data = I2C0DIN;

	switch ( I2c_Buffer.I2cNextFlag )
	{
		case I2C_NEXT_I2CREGADDRESS:

			I2c_Buffer.I2cNextFlag = I2C_NEXT_DATA;

			if ( A0_7Bits_ADDR == I2c_Buffer.SubAddress )
			{
				I2c_Buffer.A0_RegAddress = read_data;
			}
			else
			{
				I2c_Buffer.A2_RegAddress = read_data;
			}
			Prefetch_Data();
			break;

		case I2C_NEXT_DATA:

			if ( I2c_Buffer.NumOfEntries < I2C_WRITE_SIZE_ONCE )
			{
				I2c_Buffer.DataBuff[I2c_Buffer.NumOfEntries]  = read_data;
			}

			I2c_Buffer.NumOfEntries++;
			break;

		default:
			break;
	}
}



//-----------------------------------------------------------------------------
// I2c_Slave_Process_Data_Read_ACK
//-----------------------------------------------------------------------------
//
// Return Value:  void
// Parameters:    void
//
// Configures     I2c_Slave_Process_Data_Read_ACK
//
//-----------------------------------------------------------------------------
void I2c_Slave_Process_Data_Read_ACK(void)
{
	if ( A0_7Bits_ADDR == I2c_Buffer.SubAddress )
	{
		I2C0DOUT = PrefetchData_A0;
		I2c_Buffer.A0_RegAddress++;
	}
	else
	{
		I2C0DOUT = PrefetchData_A2;
		I2c_Buffer.A2_RegAddress++;
	}

	Prefetch_Data();
}


//-----------------------------------------------------------------------------
// I2c_Slave_Process_Data_Read_NACK
//-----------------------------------------------------------------------------
//
// Return Value:  void
// Parameters:    void
//
// Configures     I2c_Slave_Process_Data_Read_NACK
//
//-----------------------------------------------------------------------------
void I2c_Slave_Process_Data_Read_NACK(void)
{
	I2c_Buffer.Status = I2C_BUFFER_IDLE;
}


//-----------------------------------------------------------------------------
// I2c_Slave_Process_Stop
//-----------------------------------------------------------------------------
//
// Return Value:  void
// Parameters:    void
//
// Configures     I2c_Slave_Process_Stop
//
//-----------------------------------------------------------------------------
void I2c_Slave_Process_Stop(void)
{
	uint8_t i2c_slave_regAddress = 0;
	uint8_t i2c_slave_processed_bytes = 0;

	if ( ( I2c_Buffer.NumOfEntries == 0 ) || ( I2c_Buffer.NumOfEntries > I2C_WRITE_SIZE_ONCE ) )
	{
		// ������յ��ֽڳ���8����ֱ����������������
		I2c_Buffer.NumOfEntries = 0;
		I2c_Buffer.Status = I2C_BUFFER_IDLE;

		Prefetch_Data();
		return;
	}

	// ��Ҫ��������д������
	if ( A0_7Bits_ADDR == I2c_Buffer.SubAddress ) // A0
	{
		 // �ر�I2C���ߣ�����PROCESS_MEMORY����
		I2c_Buffer.Status = I2C_BUFFER_WAITING_TO_PROCESS;
		I2C0CN0 &= ~I2C0CN0_I2C0EN__BMASK;		// clear I2CEN bit
	}
	else // A2
	{
		i2c_slave_regAddress  = I2c_Buffer.A2_RegAddress;

		for ( i2c_slave_processed_bytes = 0; i2c_slave_processed_bytes < I2c_Buffer.NumOfEntries; i2c_slave_processed_bytes++ )
		{
			if ( (i2c_slave_regAddress >= 0x78 ) && ( i2c_slave_regAddress <= 0x7E ))
			{
				( (uint8_t SI_SEG_XDATA *)&ddm_Lower )[i2c_slave_regAddress] = I2c_Buffer.DataBuff[i2c_slave_processed_bytes];
			}
			else if ( (i2c_slave_regAddress == 0x7F ) && SecurityCheck())
			{
				( (uint8_t SI_SEG_XDATA *)&ddm_Lower )[i2c_slave_regAddress] = I2c_Buffer.DataBuff[i2c_slave_processed_bytes];
			}
			i2c_slave_regAddress ++;
		}

		I2c_Buffer.Status = I2C_BUFFER_WAITING_TO_PROCESS;
		I2C0CN0 &= ~I2C0CN0_I2C0EN__BMASK; // clear I2CEN bit
	}
}


