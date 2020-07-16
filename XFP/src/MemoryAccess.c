//-----------------------------------------------------------------------------
//      Copyright (c) 2017-2018,
//-----------------------------------------------------------------------------
//
// file name  :       MemoryAccess.c
//
// description:       Memory Access file
//
// Target     :       EFM8LB12F32ES0
//
// version    :       V100.001
//					  2019/07/30
//
//-----------------------------------------------------------------------------

#include "include.h"

volatile uint8_t max_offset = 0xFF;

volatile uint8_t sem = 1;

// 各数据表结构
SI_SEGMENT_VARIABLE( ddm_A0_pollData, 	 struct Table_Size_256_struct,   				SI_SEG_XDATA );  //A0 256bytes
SI_SEGMENT_VARIABLE( ddm_A0Data, 	   	 struct Table_Size_256_struct,   				SI_SEG_XDATA );  //A0 256bytes
SI_SEGMENT_VARIABLE( ddm_Lower,  	     struct LOWER_MEMORY_128_struct, 				SI_SEG_XDATA );  //A2 lower 128 bytes
SI_SEGMENT_VARIABLE( ddm_T00,    	     struct Table_Size_128_struct,   				SI_SEG_XDATA );	 //A2 high 128 bytes
SI_SEGMENT_VARIABLE( ddm_T02,    	 	 struct Table2_Size_128_struct,   				SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( dom_backup, 		 struct LOWER_MEMORY_DOM_struct,   				SI_SEG_XDATA );	 //dom 5 physical quantity
SI_SEGMENT_VARIABLE( I2c_Buffer,         struct I2C_BUFFER_Struct,	 					SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T80,         	 struct Page_80H_Module_Information_struct,	 	SI_SEG_XDATA );

//uint8_t tab_buf[128];

void InitPrivateData(void)
{
	// 初始化A0 256 bytes
	memset( (uint8_t SI_SEG_XDATA *)&ddm_A0Data, 0x00, sizeof(struct Table_Size_256_struct) );

	// 初始化A2 lower 128 bytes
	memset( (uint8_t SI_SEG_XDATA *)&ddm_Lower, 0x00, sizeof(struct LOWER_MEMORY_128_struct) );
	ddm_Lower.lower_memory.table_data[0x6E] = 1;

	// 初始化A2 high 128 bytes
	memset( (uint8_t SI_SEG_XDATA *)&ddm_T00, 0x00, sizeof(struct Table_Size_128_struct) );

	// 初始化page2 128 bytes
	memset( (uint8_t SI_SEG_XDATA *)&ddm_T02, 0x00, sizeof(struct Table2_Size_128_struct) );

    // 初始化I2C缓存数据
	memset( (uint8_t SI_SEG_XDATA *)&I2c_Buffer, 0x00, sizeof(struct I2C_BUFFER_Struct) );

	 // 初始化软件版本信息数据
	memset( (uint8_t SI_SEG_XDATA *)&ddm_T80, 0x00, sizeof(struct Page_80H_Module_Information_struct) );

	I2c_Buffer.SubAddress    = A0_7Bits_ADDR;
	I2c_Buffer.A0_RegAddress = 0;
	I2c_Buffer.A2_RegAddress = 0;
	I2c_Buffer.Status        = I2C_BUFFER_IDLE;

	ddm_T80.Firmware_Version[0] 	= get_second(__TIME__);
	ddm_T80.Firmware_Version[1] 	= get_minute(__TIME__);
	ddm_T80.Firmware_Version[2] 	= get_hour(__TIME__);
	ddm_T80.Firmware_Version[3] 	= get_day(__DATE__);
	ddm_T80.Firmware_Version[4] 	= get_month(__DATE__);
	ddm_T80.Firmware_Version[5] 	= get_year(__DATE__);
	ddm_T80.Firmware_Version[6] 	= (uint8_t)PRO_VERSION00;
	ddm_T80.Firmware_Version[7] 	= (uint8_t)PRO_VERSION01;
	ddm_T80.Firmware_Version[8] 	= (uint8_t)PRO_VERSION02;
	ddm_T80.Firmware_Version[9] 	= (uint8_t)SW_VERSION00;
	ddm_T80.Firmware_Version[10] 	= (uint8_t)SW_VERSION01;
	ddm_T80.Firmware_Version[11] 	= (uint8_t)SW_VERSION02;
	ddm_T80.Firmware_Version[12] 	= (uint8_t)SW_VERSION03;

#ifdef DEBUG_LOG_FUNC
	log_init((char *)&ddm_T02.table_data[0]);
#endif
}


//-----------------------------------------------------------------------------
// TransferDataToRam
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     从EEPROM/Flash中读取数据到Ram中
//
//-----------------------------------------------------------------------------
void TransferA0_DataToRam (void)
{
	FLASH_Read( (uint8_t SI_SEG_XDATA *)&ddm_A0Data, A0_TABLE_ADDRESS, A0_TABLE_SIZE );  //read A0 256 bytes
	ddm_Lower.lower_memory.table_data[A2_DATA_READY_ADDR] |= 0x1;  //set bit0 = 1 indicate A0 data ready and A2 data not ready

	// 执行一次预取数据
	Prefetch_Data();
}

void DataSynchronization(void)
{
	uint16_t i = 0;
	uint8_t A0_dev = A0_7Bits_ADDR << 1, page_save = 0;
	uint8_t A2_dev = A2_7Bits_ADDR << 1;

	for(i = 0; i < A0_TABLE_SIZE; i ++)
	{
		I2C_Master_read_byte(A0_dev, i,  (uint8_t SI_SEG_XDATA *)&ddm_A0_pollData.table_data[i]);     //read A0 256 bytes
	}

	for(i = 0; i < TABLE_LOWER_SIZE - 8; i ++)  //there is no need to update 0x78 ~ 0x7F
	{
		I2C_Master_read_byte(A2_dev, i,   (uint8_t SI_SEG_XDATA *)&ddm_Lower.lower_memory.table_data[i]);  		  //read A2 lower 128 bytes

		if((i == 0x6E) && ((ddm_Lower.lower_memory.table_data[i] & 0x1) == 0x0))
		{
			I2C_slave_enable(1);
			I2C_a2_addr_access_enable(1); //both support A0 addr and A2 addr
		}
	}

	for(i = 0; i < TABLE00_SIZE; i ++)
	{
		I2C_Master_read_byte(A2_dev, i + 0x80,  (uint8_t SI_SEG_XDATA *)&ddm_T00.table_data[i]);			  //read A2 high 128 bytes
	}

	if(memcmp((void *)&ddm_A0Data, (void *)&ddm_A0_pollData, A0_TABLE_SIZE) != 0x0)
	{
		memcpy((void *)&ddm_A0Data, (void *)&ddm_A0_pollData, A0_TABLE_SIZE);  				  //update ddm_A0Data
		FLASH_Update( A0_TABLE_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_A0Data, A0_TABLE_SIZE );  //update flash
	}
}

void RealTimeDataPoll(void)
{
	uint8_t i = 0;
	uint8_t A2_dev = A2_7Bits_ADDR << 1;

	COM_LOCK_SEM(sem);

	memcpy((void *)&dom_backup.dom.table_data[0],  (void *)&ddm_Lower.lower_memory.table_data[A2_DOM_START_ADDR], sizeof(struct LOWER_MEMORY_DOM_struct));

	for(i = A2_DOM_START_ADDR; i <= A2_DOM_END_ADDR; i++)
	{
		I2C_Master_read_byte(A2_dev, i, (uint8_t SI_SEG_XDATA *)&ddm_Lower.lower_memory.table_data[i]);
	}

	COM_UNLOCK_SEM(sem);

	for(i = A2_MONITOR_DATA_START_ADDR; i <= A2_MONITOR_DATA_END_ADDR; i++)
	{
		I2C_Master_read_byte(A2_dev, i, (uint8_t SI_SEG_XDATA *)&ddm_Lower.lower_memory.table_data[i]);
	}

	I2C_Master_read_byte(A2_dev, 0xE4,  (uint8_t SI_SEG_XDATA *)&ddm_T00.table_data[0xE4 - 0x80]);
}


//-----------------------------------------------------------------------------
// CopyAndSetRamData
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures 从EEPROM/Flash中读取数据到Ram中
//
//-----------------------------------------------------------------------------
void ProcessI2cBuffersData (void)
{
	uint16_t    processed_bytes = 0;
	uint8_t     regAddress = 0;                // 需要写入数据的子地址
	uint8_t     regData  = 0;    			   // 需要写入数据的数据
	uint8_t     A2_dev = A2_7Bits_ADDR << 1, A0_dev = A0_7Bits_ADDR << 1;


	// 如果I2C缓存中不存在数据，退出处理函数
	if (I2c_Buffer.Status == I2C_BUFFER_IDLE)
	{
		return;
	}

    // 处理A0地址写入的数据
	if (A0_7Bits_ADDR == I2c_Buffer.SubAddress)
	{
	 	regAddress = I2c_Buffer.A0_RegAddress;

		for ( processed_bytes = 0; processed_bytes < I2c_Buffer.NumOfEntries; processed_bytes++ )
		{
			regData = I2c_Buffer.DataBuff[processed_bytes];
			I2C_Master_write_byte(A0_dev, regAddress, regData);  //write data to 9601C
			regAddress++;
		}
		I2c_Buffer.A0_RegAddress = regAddress;
	}
	else if (A2_7Bits_ADDR == I2c_Buffer.SubAddress)
	{
		//log_save("\r\n 2: %bx", I2c_Buffer.NumOfEntries);

		regAddress = I2c_Buffer.A2_RegAddress;

		for (processed_bytes = 0; processed_bytes < I2c_Buffer.NumOfEntries; processed_bytes++)
		{
			regData = I2c_Buffer.DataBuff[processed_bytes];

			if (regAddress < 0x78)  //0x78 ~ 0x7E is passward area, there is no need to write 9601C
			{
				I2C_Master_write_byte(A2_dev, regAddress, regData);
			}
			else if(regAddress > 0x7f)
			{
				switch (ddm_Lower.lower_memory.struct_data.PageSelect)
				{
					case 0x00:
					case 0x01:
					{
						I2C_Master_write_byte(A2_dev, regAddress, regData);
						break;        // 表00 数据 写入
					}
					case 0x02:
					{
						#ifdef DEBUG_LOG_FUNC
						if(regAddress == 0x80 && regData == 0x1)
						{
							log_clear();
						}
						#else
						if (regAddress >= 0x80 && regAddress < (0x80 + TABLE02_SIZE))
						{
							( (uint8_t SI_SEG_XDATA*)&ddm_T02 )[regAddress - 0x80] = regData;
						}
						#endif
						break;
					}
					case 0x80:
					{
						if (regAddress >= 0x80 && regAddress < (0x80 + TABLE80_SIZE))
						{
							if(regAddress == 0xFF && regData == 0x1)
							{
								uint8_t SFRPAGE_SAVE = SFRPAGE;
								SFRPAGE   = 0x0;
								*((uint8_t SI_SEG_DATA *)0x00) = 0xA5;
								RSTSRC = RSTSRC_SWRSF__SET | RSTSRC_PORSF__SET;
								SFRPAGE = SFRPAGE_SAVE;
								return;
							}
							else if(regAddress == 0xFE && regData == 0x1)
							{
								I2C_a2_addr_access_enable(1); //both support A0 addr and A2 addr
							}
							else if(regAddress == 0xFD)
							{
								max_offset = regData;
							}
							( (uint8_t SI_SEG_XDATA*)&ddm_T80 )[regAddress - 0x80] = regData;
						}
						break;
					}
					default:
						break;
				}
			} // End of  if ( regAddress > 0x7F )

			regAddress++;

		} // End of  for ( processed_bytes = 0; processed_bytes < I2c_Buffer.NumOfEntries; processed_bytes++ )

		// 维护写之后的当前地址
		I2c_Buffer.A2_RegAddress = regAddress;

	} // End of else  // 处理A2地址写入的数据

	// take precedence enable I2C slave
	I2C_slave_enable(1);

	Prefetch_Data();

	// 清空I2C缓存区,并将缓存区状态修正为IDLE状态
	I2c_Buffer.NumOfEntries = 0x00;
	I2c_Buffer.Status 		= I2C_BUFFER_IDLE;

}
