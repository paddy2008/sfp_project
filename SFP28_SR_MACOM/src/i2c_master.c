//-----------------------------------------------------------------------------------
//      Copyright (c) 2005-2018,
//-----------------------------------------------------------------------------------
//
// file name  :       i2c_master.c
//
// description:       I2C Master Driver
//
// Target     :       EFM8LB1
//
// version    :       V100.001
//
//
//-----------------------------------------------------------------------------------

#include "Include.h"

//-----------------------------------------------------------------------------
// I2C_Master_start
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     Send I2C Start signal
//
//-----------------------------------------------------------------------------
uint8_t I2C_Master_start(void)
{
	// 根据SCL SDA的不同初始状态，将SCL SDA的引脚拉高为1，准备好，并避免出现STOP的状态
	if ( Get_SCL() == 1 )
	{
		if ( Get_SDA() == 0 )
		{
			SCL_L();
			delay_us(Rise_Time_Delay);
			SDA_HIZ();
			delay_us(Rise_Time_Delay);
			SCL_HIZ();
		}
	}
	else
	{
		if ( Get_SDA() == 1 ) // 时钟线为低，数据线为高
		{
			SCL_HIZ();
		}
		else // 时钟线为低，数据线为低
		{
			SDA_HIZ();
			delay_us(Rise_Time_Delay);
			SCL_HIZ();
		}
	}

	delay_us(Rise_Time_Delay);

	if (( Get_SCL() == 1 ) && ( Get_SDA() == 1 ))
	{
		delay_us(I2C_Master_HoldTime);
		SDA_L();
		delay_us(I2C_Master_HoldTime);
		SCL_L();
		delay_us(I2C_Master_Delay);  // 为了保持SCL为低电平的时间约为半周期

		return 0;
	}

	return 1;
}



//-----------------------------------------------------------------------------
// I2C_Master_stop
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     Send I2C Stop signal
//
//-----------------------------------------------------------------------------
uint8_t I2C_Master_stop(void)
{
	// 根据SCL SDA的不同初始状态，将SCL=1 SDA=0，同时避免出现STOP的状态
	if ( Get_SCL() == 0 )
	{
		if ( Get_SDA() == 0 ) // SCL=0 SDA=0
		{
			SCL_HIZ();
		}
		else // SCL=0 SDA=1
		{
			SDA_L();
			delay_us(Rise_Time_Delay);
			SCL_HIZ();
		}
	}
	else
	{
		if ( Get_SDA() == 1 ) // SCL=1 SDA=1
		{
			SCL_L();
			delay_us(Rise_Time_Delay);
			SDA_L();
			delay_us(Rise_Time_Delay);
			SCL_HIZ();
		}
	}

	delay_us(Rise_Time_Delay); // 等待SCL SDA稳定

	if ( ( Get_SCL() == 1 ) && ( Get_SDA() == 0 ) )
	{
		delay_us(I2C_Master_HoldTime);
		SDA_HIZ();
		delay_us(I2C_Master_HoldTime);

		if (( Get_SCL() == 1 ) && ( Get_SDA() == 1 ))
	    {
	    	return 0; // SCL SDA均为高 正常
	    }
	    else
	    {
	       return 1; // SCL SDA无法达到终止状态，报告异常
	    }
	}

	return 1; // SCL SDA无法设定为初始状态，报告异常
}


//-----------------------------------------------------------------------------
// I2C_Master_bit_out
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    send bit
//
// Configures     Send one bit to I2C slave
//
//-----------------------------------------------------------------------------
uint8_t I2C_Master_bit_out( uint8_t bit_out)
{
	uint8_t timercount;

	SDA_OUT(bit_out);                              // 输出一个bit数据到SDA数据线

	delay_us(Rise_Time_Delay);                     // 要求等待时间大于2us

	if ( Get_SDA() != bit_out )                    // 检查SDA是否与目标值一致，不一致则报错
	{
		return 1;
	}

	SCL_HIZ();                                     // 拉高时钟线

	timercount = SCL_Stretch_Timeout;              // 设置超时等待时间，时钟周期：8

	delay_us(Rise_Time_Delay);                     // 要求等待时间大于2us

	while( !Get_SCL() )                            // 检查SCL状态是否为高，不为高则等待，不执行while，时钟周期：3
	{
		delay_us(I2C_Master_HaftCycle);

		if ( timercount-- == 0 )                    // 超过8个I2C总线周期SCL没有恢复，则报错
		{
	         return 1;
		}
	}

	delay_us( I2C_Master_Delay );                  // 延时，控制传送速率

	SCL_L();                                        // 拉低SCL时钟线

	delay_us( I2C_Master_Delay );                   // 延时，控制传送速率

	return 0;                                       // 函数执行完
}


//-----------------------------------------------------------------------------
// I2C_Master_bit_in
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    receive bit
//
// Configures     Receive one bit from I2C slave
//
//-----------------------------------------------------------------------------
uint8_t I2C_Master_bit_in( uint8_t *bit_in)
{
	uint8_t timercount;

	SDA_HIZ();                      	// 将SDA设定为高阻模式

	delay_us(Rise_Time_Delay);      	// 等待SDA线路稳定

	SCL_HIZ();                      	// 拉高时钟线

	timercount = SCL_Stretch_Timeout;   // 设置超时等待时间，时钟周期：8

	delay_us(Rise_Time_Delay);          // 等待SCL线路稳定

	while( !Get_SCL() )                 // 检查SCL状态是否为高，不为高则等待，不执行while，时钟周期：3
	{
		delay_us(I2C_Master_HaftCycle);

		if ( timercount-- == 0 )       // 超过8个I2C总线周期SCL没有恢复，则报错
		{
			return 1;
		}
	}

	*bit_in = Get_SDA();

	delay_us( I2C_Master_Delay );

	SCL_L();

	delay_us( I2C_Master_Delay );

	return 0;
}

//-----------------------------------------------------------------------------
// I2C_Master_byte_out
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    send byte
//
// Configures     Send one byte to I2C slave
//
//-----------------------------------------------------------------------------
uint8_t I2C_Master_byte_out( uint8_t byte_out)
{
	uint8_t   bit_count = 8;
	uint8_t   errorcode;
	uint8_t   ack;

	while (bit_count)
	{
		if ( byte_out & 0x80 )                            // 传送一个bit数据1
		{
			if ( I2C_Master_bit_out(1) )
			{
	            return 1;
			}
		}
		else                                              // 传送一个bit数据0
		{
			if ( I2C_Master_bit_out(0) )
			{
	            return 1;
			}
		}

		byte_out <<= 1;
		bit_count--;
	}

	errorcode = I2C_Master_bit_in( &ack );
	if (( errorcode == 1 ) || ( ack == 1 ))
	{
		return 2;
	}

	return 0;
}


//-----------------------------------------------------------------------------
// I2C_Master_byte_in
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    receive byte
//
// Configures     Receive one byte from I2C slave
//
//-----------------------------------------------------------------------------
uint8_t I2C_Master_byte_in ( uint8_t ack_out, uint8_t *byte_in )
{
	uint8_t bit_count = 8;
	uint8_t bit_in;

	*byte_in  = 0;

	while (bit_count)
	{
		*byte_in = (*byte_in) << 1;

		if ( I2C_Master_bit_in( &bit_in ) ) // 读取一个bit的数据
		{
	         return 1;
		}

		*byte_in = (*byte_in) | bit_in;
		bit_count--;
	}

	if ( I2C_Master_bit_out(ack_out) )
	{
		return 1;
	}

	return 0;
}


//-----------------------------------------------------------------------------
// I2C_Master_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     Initialize I2C Master
//
//-----------------------------------------------------------------------------
uint8_t I2C_Master_Init(void)
{
	SCL_L();
	SDA_L();

	delay_us(Rise_Time_Delay);

	SDA_HIZ();
	SCL_HIZ();

	delay_us(Rise_Time_Delay);                  // 等待时钟上升沿完成

	if (( Get_SCL() == 1 ) && ( Get_SDA() == 1 ))
	{
		return 0;                                // 初始化成功
	}

	return 1;                                    // 管脚状态不对，初始化异常
}


//-----------------------------------------------------------------------------
// I2C_Master_read_byte
//-----------------------------------------------------------------------------
//
// Return Value:  0 : Success / 1 : Error
// Parameters:    None
//
// Configures     send one byte registet adress and read one byte data
//
//-----------------------------------------------------------------------------
uint8_t I2C_Master_read_byte ( uint8_t device, uint8_t address, uint8_t *bufPtr )
{
	// 发送START信号
	if ( I2C_Master_start() )
	{
		I2C_Master_stop();
		return 1;
	}

	// 对设备地址进行SLA+W寻址
	if ( I2C_Master_byte_out( (device & 0xFE) ) )
	{
		I2C_Master_stop();
		return 2;
	}

	// 写入数据写入的子地址
	if ( I2C_Master_byte_out(address) )
	{
	   I2C_Master_stop();
	   return 3;
	}

	// 发送RESTART信号
	if ( I2C_Master_start() )
	{
		I2C_Master_stop();
		return 4;
	}

	// 对设备地址进行SLA+R寻址
	if ( I2C_Master_byte_out( (device | 0x01) ) )
	{
		I2C_Master_stop();
		return 5;
	}

	// 读取一个字节的数据，并返回NACK
	if ( I2C_Master_byte_in(1, bufPtr) )
	{
		I2C_Master_stop();
		return 6;
	}

	// 发送STOP信号
	if ( I2C_Master_stop() )
	{
		return 7;
	}

	return 0;
}



//-----------------------------------------------------------------------------
// I2C_Master_write_byte
//-----------------------------------------------------------------------------
//
// Return Value:  0 : Success / 1 : Error
// Parameters:    None
//
// Configures     send one byte registet adress and write one byte data
//
//-----------------------------------------------------------------------------
uint8_t I2C_Master_write_byte ( uint8_t device, uint8_t address, uint8_t buf )
{
	// 发送START信号
	if ( I2C_Master_start() )
	{
		I2C_Master_stop();
		return 1;
	}

	// 对设备地址进行SLA+W寻址
	if ( I2C_Master_byte_out( (device & 0xFE) ) )
	{
		I2C_Master_stop();
		return 2;
	}

	// 写入数据写入的子地址
	if ( I2C_Master_byte_out(address) )
	{
		I2C_Master_stop();
		return 3;
	}

	// 写入数据
	if ( I2C_Master_byte_out(buf) )
	{
		I2C_Master_stop();
		return 4;
	}

	// 发送STOP信号
	if ( I2C_Master_stop() )
	{
		return 5;
	}

	return 0;
}



//-----------------------------------------------------------------------------
// I2C_Master_readBytes
//-----------------------------------------------------------------------------
//
// Return Value:  0 : Success / 1 : Error
// Parameters:    None
//
// Configures     send one byte registet adress and read (length) bytes data
//
//-----------------------------------------------------------------------------
uint8_t I2C_Master_readBytes( uint8_t device, uint8_t address, uint16_t length, uint8_t *bufPtr )
{
	uint16_t i;

	// 发送START信号
	if ( I2C_Master_start() )
	{
		I2C_Master_stop();
		return 1;
	}

	// 对设备地址进行SLA+W寻址
	if ( I2C_Master_byte_out( (device & 0xFE) ) )
	{
		I2C_Master_stop();
		return 2;
	}

	// 写入数据写入的子地址
	if ( I2C_Master_byte_out(address) )
	{
		I2C_Master_stop();
		return 3;
	}

	// 发送RESTART信号
	if ( I2C_Master_start() )
	{
		I2C_Master_stop();
		return 4;
	}

	// 对设备地址进行SLA+R寻址
	if ( I2C_Master_byte_out( (device | 0x01) ) )
	{
		I2C_Master_stop();
		return 5;
	}


	for ( i = 0; i < length; i++ )
	{
	   if ( I2C_Master_byte_in((i==(length-1))?1:0, &bufPtr[i]) )
	   {
		   I2C_Master_stop();
		   return 6;
	   }
	}

	// 发送STOP信号
	if ( I2C_Master_stop() )
	{
		return 7;
	}

	return 0 ;
}


//-----------------------------------------------------------------------------
// I2C_Master_writeBytes
//-----------------------------------------------------------------------------
//
// Return Value:  0 : Success / 1 : Error
// Parameters:    None
//
// Configures     send one byte registet adress and write (length) bytes data
//
//-----------------------------------------------------------------------------
uint8_t I2C_Master_writeBytes( uint8_t device, uint8_t address, uint16_t length, uint8_t *bufPtr )
{
	uint16_t i;

   // 发送START信号
   if ( I2C_Master_start() )
   {
      I2C_Master_stop();
      return 1;
   }

   // 对设备地址进行SLA+W寻址
   if ( I2C_Master_byte_out( (device & 0xFE) ) )
   {
      I2C_Master_stop();
      return 2;
   }

   // 写入数据写入的子地址
   if ( I2C_Master_byte_out(address) )
   {
      I2C_Master_stop();
      return 3;
   }

   for ( i = 0; i < length; i++ )
   {
      if ( I2C_Master_byte_out(bufPtr[i]) )
      {
         I2C_Master_stop();
         return 4;
      }
   }

   // 发送STOP信号
   if ( I2C_Master_stop() )
   {
       return 5;
   }

   return 0;
}


//-----------------------------------------------------------------------------
// I2C_Master_read_word
//-----------------------------------------------------------------------------
//
// Return Value:  0 : Success / 1 : Error
// Parameters:    None
//
// Configures     send one byte registet adress and read one word data
//
//-----------------------------------------------------------------------------
uint8_t I2C_Master_read_word( uint8_t device, uint8_t address,  uint16_t *bufPtr )
{
	uint8_t buf_h,buf_l;

	// 发送START信号
	if ( I2C_Master_start() )
	{
		I2C_Master_stop();
		return 1;
	}

	// 对设备地址进行SLA+W寻址
	if ( I2C_Master_byte_out( (device & 0xFE) ) )
	{
		I2C_Master_stop();
		return 2;
	}

	// 写入数据写入的子地址
	if ( I2C_Master_byte_out(address) )
	{
		I2C_Master_stop();
		return 3;
	}

	// 发送RESTART信号
	if ( I2C_Master_start() )
	{
		I2C_Master_stop();
		return 4;
	}

	// 对设备地址进行SLA+R寻址
	if ( I2C_Master_byte_out( (device | 0x01) ) )
	{
		I2C_Master_stop();
		return 5;
	}

	// 读取一个字节的数据，并返回ACK
	if ( I2C_Master_byte_in(1, &buf_h) )
	{
		I2C_Master_stop();
		return 6;
	}

	// 读取一个字节的数据，并返回NACK
	if ( I2C_Master_byte_in(0, &buf_l) )
	{
		I2C_Master_stop();
		return 7;
	}

	// 发送STOP信号
	if ( I2C_Master_stop() )
	{
		return 8;
	}

	*bufPtr = ((uint16_t)buf_h << 8) | buf_l;

	return 0;
}


//-----------------------------------------------------------------------------
// I2C_Master_write_word
//-----------------------------------------------------------------------------
//
// Return Value:  0 : Success / 1 : Error
// Parameters:    None
//
// Configures     send one byte registet adress and write one word data
//
//-----------------------------------------------------------------------------
uint8_t I2C_Master_write_word (uint8_t device, uint8_t address, uint16_t buf )
{
	// 发送START信号
	if ( I2C_Master_start() )
	{
		I2C_Master_stop();
		return 1;
	}

	// 对设备地址进行SLA+W寻址
	if ( I2C_Master_byte_out( (device & 0xFE) ) )
	{
		I2C_Master_stop();
		return 2;
	}

	// 写入数据写入的子地址
	if ( I2C_Master_byte_out(address) )
	{
		I2C_Master_stop();
		return 3;
	}

	// 写入数据
	if ( I2C_Master_byte_out((uint8_t)(buf >> 8)) )
	{
		I2C_Master_stop();
		return 4;
	}

	// 写入数据
	if ( I2C_Master_byte_out((uint8_t)(buf >> 0)) )
	{
		I2C_Master_stop();
		return 5;
	}

	// 发送STOP信号
	if ( I2C_Master_stop() )
	{
		return 6;
	}

	return 0;
}


//-----------------------------------------------------------------------------
// I2C_Master_DRegAdr_readBytes
//-----------------------------------------------------------------------------
//
// Return Value:  0 : Success / 1 : Error
// Parameters:    None
//
// Configures     send two byte registet adress and read (length) bytes data
//
//-----------------------------------------------------------------------------
uint8_t I2C_Master_DRegAdr_readBytes( uint8_t device, uint16_t address, uint16_t length, uint8_t *bufPtr )
{
	uint16_t i;

   // 发送START信号
   if ( I2C_Master_start() )
   {
      I2C_Master_stop();
      return 1;
   }

   // 对设备地址进行SLA+W寻址
   if ( I2C_Master_byte_out( (device & 0xFE) ) )
   {
      I2C_Master_stop();
      return 2;
   }

   // 写入数据写入的子地址 高8位
   if ( I2C_Master_byte_out((address>>8)&0xFF) )
   {
      I2C_Master_stop();
      return 3;
   }

   // 写入数据写入的子地址 低8位
   if ( I2C_Master_byte_out((address>>0)&0xFF) )
   {
      I2C_Master_stop();
      return 3;
   }

   // 发送RESTART信号
   if ( I2C_Master_start() )
   {
      I2C_Master_stop();
      return 4;
   }

   // 对设备地址进行SLA+R寻址
   if ( I2C_Master_byte_out( (device | 0x01) ) )
   {
      I2C_Master_stop();
      return 5;
   }


   for ( i = 0; i < length; i++ )
   {
      if ( I2C_Master_byte_in( ((i==(length-1))?1:0), &bufPtr[i]) )
      {
         I2C_Master_stop();
         return 6;
      }
   }

   // 发送STOP信号
   if ( I2C_Master_stop() )
   {
      return 7;
   }

   return 0 ;
}


//-----------------------------------------------------------------------------
// I2C_Master_DRegAdr_writeBytes
//-----------------------------------------------------------------------------
//
// Return Value:  0 : Success / 1 : Error
// Parameters:    None
//
// Configures     send two byte registet adress and write (length) bytes data
//
//-----------------------------------------------------------------------------
uint8_t I2C_Master_DRegAdr_writeBytes( uint8_t device, uint16_t address, uint16_t length, uint8_t *bufPtr )
{
	uint16_t i;

	// 发送START信号
	if ( I2C_Master_start() )
	{
		I2C_Master_stop();
		return 1;
	}

	// 对设备地址进行SLA+W寻址
	if ( I2C_Master_byte_out( (device & 0xFE) ) )
	{
		I2C_Master_stop();
		return 2;
	}

	// 写入数据写入的子地址 高8位
	if ( I2C_Master_byte_out((address>>8)&0xFF) )
	{
		I2C_Master_stop();
		return 3;
	}

	// 写入数据写入的子地址 低8位
	if ( I2C_Master_byte_out((address>>0)&0xFF) )
	{
		I2C_Master_stop();
		return 4;
	}

	for ( i = 0; i < length; i++ )
	{
		if ( I2C_Master_byte_out(bufPtr[i]) )
		{
			I2C_Master_stop();
			return 5;
		}
	}

	// 发送STOP信号
	if ( I2C_Master_stop() )
	{
		return 6;
	}

	return 0;
}






