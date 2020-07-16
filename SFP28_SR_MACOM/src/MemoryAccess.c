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


// 各数据表结构
SI_SEGMENT_VARIABLE( ddm_A0Data, struct Table_Size_256_struct,   				SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_Lower,  struct LOWER_MEMORY_MAP_struct, 				SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T00,    struct Table_Size_128_struct,   				SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T80,    struct Table_General_Control_struct,   		SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T81,    struct Table_TX_Cal_Ctrl_struct,       		SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T82,    struct Table_RX_Cal_Ctrl_struct,       		SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T83,    struct Table_Temperature_Compensation_struct,  SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T84,    struct Table_ModSet_Calibration_struct,       	SI_SEG_XDATA );

SI_SEGMENT_VARIABLE( ddm_T86,    struct Table_Device_MALD_37645B_struct,       	SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T87,    struct Table_Device_MATA_37644B_struct,       	SI_SEG_XDATA );

SI_SEGMENT_VARIABLE( ddm_T90,    struct Table_Monitor_struct,		SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T91,    struct Table_Device_Debug_struct,	SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T92,    struct Table_System_Debug_struct,	SI_SEG_XDATA );


SI_SEGMENT_VARIABLE( Module_ADC_DOM_s, struct Module_ADC_DOM_struct, SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( I2c_Buffer,       struct I2C_BUFFER_Struct,	 SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( Module_Status_s,  struct Module_Status_struct,  SI_SEG_DATA  );
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
void TransferDataToRam (void)
{
	uint8_t CheckSum;
	int     i;

	FLASH_Read( (uint8_t SI_SEG_XDATA *)&ddm_A0Data, A0_TABLE_ADDRESS, A0_TABLE_SIZE );
	FLASH_Read( (uint8_t SI_SEG_XDATA *)&ddm_Lower, TABLE_LOWER_ADDRESS, TABLE_LOWER_SIZE );
	FLASH_Read( (uint8_t SI_SEG_XDATA *)&ddm_T00, TABLE00_ADDRESS, TABLE00_SIZE );
	FLASH_Read( (uint8_t SI_SEG_XDATA *)&ddm_T80, TABLE80_ADDRESS, TABLE80_SIZE );
	FLASH_Read( (uint8_t SI_SEG_XDATA *)&ddm_T81, TABLE81_ADDRESS, TABLE81_SIZE );
	FLASH_Read( (uint8_t SI_SEG_XDATA *)&ddm_T82, TABLE82_ADDRESS, TABLE82_SIZE );
	FLASH_Read( (uint8_t SI_SEG_XDATA *)&ddm_T83, TABLE83_ADDRESS, TABLE83_SIZE );
	FLASH_Read( (uint8_t SI_SEG_XDATA *)&ddm_T84, TABLE84_ADDRESS, TABLE84_SIZE );
	FLASH_Read( (uint8_t SI_SEG_XDATA *)&ddm_T86, TABLE86_ADDRESS, TABLE86_SIZE );
	FLASH_Read( (uint8_t SI_SEG_XDATA *)&ddm_T87, TABLE87_ADDRESS, TABLE87_SIZE );

	memset( (uint8_t SI_SEG_XDATA *)&ddm_T90, 0x00, TABLE90_SIZE );
	memset( (uint8_t SI_SEG_XDATA *)&ddm_T91, 0x00, TABLE91_SIZE );
	memset( (uint8_t SI_SEG_XDATA *)&ddm_T92, 0x00, TABLE92_SIZE );

	ddm_Lower.G_CtrlStatus_Bits.CtrlStatus_110.Byte = 0x00;

	// 更新版本信息到监视数据表中
	ddm_T90.FWVersion[0] = VERSIONNUM00;
	ddm_T90.FWVersion[1] = VERSIONNUM01;
	ddm_T90.FWVersion[2] = VERSIONNUM02;
	ddm_T90.FWVersion[3] = BUILDNUM;

	ddm_Lower.PageSelect        = 0x00;
	ddm_Lower.PasswordEntryArea = 0x00;
	ddm_Lower.SecurityLevel     = 0x00;

	if( (ddm_T80.PassWord_1 == 0xFFFFFFFF) && (ddm_T80.PassWord_2 == 0xFFFFFFFF)  )
	{
		ddm_Lower.Threshold_Values.Temperature.High_Alarm   = 0x5000;
		ddm_Lower.Threshold_Values.Temperature.Low_Alarm    = 0xF600;
		ddm_Lower.Threshold_Values.Temperature.High_Warning = 0x4B00;
		ddm_Lower.Threshold_Values.Temperature.Low_Warning  = 0xFB00;

		ddm_Lower.Threshold_Values.Voltage.High_Alarm   = 0x8CA0;
		ddm_Lower.Threshold_Values.Voltage.Low_Alarm    = 0x7530;
		ddm_Lower.Threshold_Values.Voltage.High_Warning = 0x88B8;
		ddm_Lower.Threshold_Values.Voltage.Low_Warning  = 0x7918;

		ddm_Lower.Threshold_Values.TXBais.High_Alarm   = 0x1770;
		ddm_Lower.Threshold_Values.TXBais.Low_Alarm    = 0x03E8;
		ddm_Lower.Threshold_Values.TXBais.High_Warning = 0x1388;
		ddm_Lower.Threshold_Values.TXBais.Low_Warning  = 0x07D0;

		ddm_Lower.Threshold_Values.TXPower.High_Alarm   = 0x4DF0;
		ddm_Lower.Threshold_Values.TXPower.Low_Alarm    = 0x03E8;
		ddm_Lower.Threshold_Values.TXPower.High_Warning = 0x3DE8;
		ddm_Lower.Threshold_Values.TXPower.Low_Warning  = 0x0603;

		ddm_Lower.Threshold_Values.RXPower.High_Alarm   = 0x4DF0;
		ddm_Lower.Threshold_Values.RXPower.Low_Alarm    = 0x031A;
		ddm_Lower.Threshold_Values.RXPower.High_Warning = 0x3DE8;
		ddm_Lower.Threshold_Values.RXPower.Low_Warning  = 0x03E8;

		ddm_Lower.Threshold_Values.LaserTemperature.High_Alarm   = 0x000;
		ddm_Lower.Threshold_Values.LaserTemperature.Low_Alarm    = 0x000;
		ddm_Lower.Threshold_Values.LaserTemperature.High_Warning = 0x000;
		ddm_Lower.Threshold_Values.LaserTemperature.Low_Warning  = 0x000;

		ddm_Lower.Threshold_Values.TECCurrent.High_Alarm   = 0x000;
		ddm_Lower.Threshold_Values.TECCurrent.Low_Alarm    = 0x000;
		ddm_Lower.Threshold_Values.TECCurrent.High_Warning = 0x000;
		ddm_Lower.Threshold_Values.TECCurrent.Low_Warning  = 0x000;

		ddm_Lower.ExternalCal_Values.RxPower_Slope_4 = 0;
		ddm_Lower.ExternalCal_Values.RxPower_Slope_3 = 0;
		ddm_Lower.ExternalCal_Values.RxPower_Slope_2 = 0;
		ddm_Lower.ExternalCal_Values.RxPower_Slope_1 = 1;
		ddm_Lower.ExternalCal_Values.RxPower_Offset  = 0;

		ddm_Lower.ExternalCal_Values.TxBias_Slope   = 1;
		ddm_Lower.ExternalCal_Values.TxBias_Offset  = 0;

		ddm_Lower.ExternalCal_Values.TxPower_Slope   = 1;
		ddm_Lower.ExternalCal_Values.TxPower_Offset  = 0;

		ddm_Lower.ExternalCal_Values.Temperature_Slope   = 1;
		ddm_Lower.ExternalCal_Values.Temperature_Offset  = 0;

		ddm_Lower.ExternalCal_Values.Voltage_Slope   = 1;
		ddm_Lower.ExternalCal_Values.Voltage_Offset  = 0;

		ddm_T80.SN[0] = 0x30;
		ddm_T80.SN[1] = 0x30;
		ddm_T80.SN[2] = 0x30;
		ddm_T80.SN[3] = 0x30;

		ddm_T80.SN[4] = 0x30;
		ddm_T80.SN[5] = 0x30;
		ddm_T80.SN[6] = 0x30;
		ddm_T80.SN[7] = 0x30;

		ddm_T80.PassWord_1 = 0x27463571;
		ddm_T80.PassWord_2 = 0x193A897D;

		ddm_T80.EEPROMVersion[0] = 0xE0;
		ddm_T80.EEPROMVersion[1] = 0x00;
		ddm_T80.EEPROMVersion[2] = 0x03;
		ddm_T80.EEPROMVersion[3] = 0x01;

		ddm_T80.ModuleDefCtrl_SYS.Byte = 0x00;
		ddm_T80.ModuleDefCtrl_TX.Byte = 0x00;
		ddm_T80.ModuleDefCtrl_RX.Byte = 0x00;

		ddm_T80.V_Reference = 1.65;

		ddm_T81.TXPowerCal_Slope  = 1;
		ddm_T81.TXPowerCal_Offset = 0;

		ddm_T81.SoftAPC_Target = 0x1388;
		ddm_T81.Bias_Max       = 0x00A7;

		ddm_T81.BiasSet = 0;
		ddm_T81.ModSet  = 0;

		ddm_T82.RxPowerCal_Slope_4 = 0;
		ddm_T82.RxPowerCal_Slope_3 = 0;
		ddm_T82.RxPowerCal_Slope_2 = 0;
		ddm_T82.RxPowerCal_Slope_1 = 1;
		ddm_T82.RxPowerCal_Offset  = 0;

		ddm_T82.RxADC_DarkDown = 0;
		ddm_T82.RxADC_DarkUp   = 10;
		ddm_T82.RxLosAssert    = 0;
		ddm_T82.RxLosDeAssert  = 0;

		ddm_T83.Offset_Sensor = 0;
		ddm_T83.Slope_Sensor  = 1;

		ddm_T83.Offset_Chip_PCB  = 0;
		ddm_T83.Offset_PCB_Shell = 0;
		ddm_T83.Offset_Display   = 8;

		ddm_T84.ChipTemperature_Normal = 35;
		ddm_T84.ModSet_Slope_L  = 1;
		ddm_T84.ModSet_Offset_L = 0;
		ddm_T84.ModSet_Slope_H  = 1;
		ddm_T84.ModSet_Offset_H = 0;

		ddm_T86.Misc_IO_Control        = 0x00;
		ddm_T86.CDRctrl                = 0x00;
		ddm_T86.I2C_address_mode       = 0x00;
		ddm_T86.Channel_mode           = 0x40;
		ddm_T86.Lockphase              = 0x00;
		ddm_T86.Lockphase              = 0x00;
		ddm_T86.LOS_LOL_Tx_Alarm       = 0x00;
		ddm_T86.IgnoreTxFault          = 0x0E;
		ddm_T86.LOS_THRSH_AUTO_SQUELCH = 0x03;
		ddm_T86.CTLE_X                 = 0x03;
		ddm_T86.OUTPUT_MUTE_SLEW       = 0x00;
		ddm_T86.Ibias                  = 0x1B;
		ddm_T86.Imod                   = 0x6C;
		ddm_T86.PreFall                = 0x00;
		ddm_T86.TDE                    = 0x00;
		ddm_T86.CrossingAdj            = 0x1C;
		ddm_T86.Iburnin                = 0x00;
		ddm_T86.Burnin_enable          = 0x00;
		ddm_T86.ADC_config0            = 0x00;

		ddm_T87.Reset		 		= 0x00;
		ddm_T87.MONITORS            = 0x40;
		ddm_T87.CDRctrl             = 0x00;
		ddm_T87.I2C_address_mode    = 0x00;
		ddm_T87.Channel_mode        = 0x40;
		ddm_T87.Lockphase           = 0x00;
		ddm_T87.LOS_mode            = 0x01;
		ddm_T87.LOS_LOL_alarm       = 0x00;
		ddm_T87.LOS_CTRL            = 0x80;
		ddm_T87.SLA                 = 0x0F;
		ddm_T87.TIA_CTRL            = 0x00;
		ddm_T87.OUTPUT_CTRL         = 0x00;
		ddm_T87.OUTPUT_SWING        = 0x32;
		ddm_T87.OUTPUT_DEEMPH       = 0x06;
		ddm_T87.ADC_config0         = 0x12;
		ddm_T87.ADC_config2 	    = 0x00;

		FLASH_Update( TABLE_LOWER_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_Lower, TABLE_LOWER_CTRL_SIZE );
		FLASH_Update( TABLE00_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T00, TABLE00_SIZE );
		FLASH_Update( TABLE80_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T80, TABLE80_SIZE );
		FLASH_Update( TABLE81_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T81, TABLE81_SIZE );
		FLASH_Update( TABLE82_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T82, TABLE82_SIZE );
		FLASH_Update( TABLE83_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T83, TABLE83_SIZE );
		FLASH_Update( TABLE84_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T84, TABLE84_SIZE );
		FLASH_Update( TABLE86_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T86, TABLE86_SIZE );
		FLASH_Update( TABLE87_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T87, TABLE87_SIZE );
	}

	CheckSum = 0;
	for( i = 0; i < 95; i++ )
	{
		CheckSum += ( (uint8_t SI_SEG_XDATA*)&ddm_Lower )[i];
	}
	( (uint8_t SI_SEG_XDATA*)&ddm_Lower )[95] = CheckSum;
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
	uint16_t    processed_bytes;
	uint8_t     regAddress;                // 需要写入数据的子地址
	uint8_t     regData;                   // 需要写入数据的数据
	uint8_t     WriteEEPROMFlag = WRITEDISABLE;   // 写EEPORM标志


	// 如果I2C缓存中不存在数据，退出处理函数
	if ( I2c_Buffer.Status == I2C_BUFFER_IDLE )
	{
		return;
	}

    // 处理A0地址写入的数据
	if ( A0_7Bits_ADDR == I2c_Buffer.SubAddress )
	{
	 	regAddress = I2c_Buffer.A0_RegAddress;

		for ( processed_bytes = 0; processed_bytes < I2c_Buffer.NumOfEntries; processed_bytes++ )
		{
			regData = I2c_Buffer.DataBuff[processed_bytes];
			SubProcess_Table_A0(regAddress, regData);
			regAddress++;
		}

		FLASH_Update( A0_TABLE_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_A0Data, A0_TABLE_SIZE );
		I2c_Buffer.A0_RegAddress = regAddress;

	    // 112ms
		// FLASH_Update( A0_TABLE_ADDRESS+I2c_Buffer.A0_RegAddress, I2c_Buffer.DataBuff, I2c_Buffer.NumOfEntries );
		// I2c_Buffer.A0_RegAddress += I2c_Buffer.NumOfEntries;
	}
	/*
	else if ( (A2_7Bits_ADDR == I2c_Buffer.SubAddress) && (I2c_Buffer.A2_RegAddress > 0x7F) && (ddm_Lower.PageSelect == 0x00)  )
	{
		FLASH_Update( TABLE00_ADDRESS+I2c_Buffer.A2_RegAddress-128, I2c_Buffer.DataBuff, I2c_Buffer.NumOfEntries );
	    I2c_Buffer.A2_RegAddress += I2c_Buffer.NumOfEntries;
	}
	*/
	else if ( A2_7Bits_ADDR == I2c_Buffer.SubAddress  )
	{
		regAddress = I2c_Buffer.A2_RegAddress;

		for ( processed_bytes = 0; processed_bytes < I2c_Buffer.NumOfEntries; processed_bytes++ )
		{
			regData = I2c_Buffer.DataBuff[processed_bytes];

			// 低128字节的EEPROM存储数据 部分数据能被修改保存 0x00 - 0x5F
			if ( regAddress < TABLE_LOWER_CTRL_SIZE )
			{
				// 在工程模式下可以进行读写存操作
				if ( ddm_Lower.SecurityLevel == 0x02 )
				{
					SubProcess_TableLower( regAddress, regData );
					WriteEEPROMFlag = WRITEENABLE;
				}
			}
			// 存在其他需要特殊处理的地址，可添加在此处

			// 其他表数据
			else if ( regAddress > 0x7F )
			{
				switch ( ddm_Lower.PageSelect )
				{
					case 0x00: SubProcess_Table00( regAddress, regData ); WriteEEPROMFlag = WRITEENABLE; break;        // 表00 数据 写入
					case 0x80: if ( ddm_Lower.SecurityLevel == 0x02 ) {SubProcess_Table80( regAddress, regData ); WriteEEPROMFlag = WRITEENABLE;} break;  // 表 10写入
					case 0x81: if ( ddm_Lower.SecurityLevel == 0x02 ) {SubProcess_Table81( regAddress, regData ); WriteEEPROMFlag = WRITEENABLE;} break;  // 表 11写入
					case 0x82: if ( ddm_Lower.SecurityLevel == 0x02 ) {SubProcess_Table82( regAddress, regData ); WriteEEPROMFlag = WRITEENABLE;} break;  // 表 12写入
					case 0x83: if ( ddm_Lower.SecurityLevel == 0x02 ) {SubProcess_Table83( regAddress, regData ); WriteEEPROMFlag = WRITEENABLE;} break;  // 表 13写入
					case 0x84: if ( ddm_Lower.SecurityLevel == 0x02 ) {SubProcess_Table84( regAddress, regData ); WriteEEPROMFlag = WRITEENABLE;} break;  // 表 14写入
					case 0x86: if ( ddm_Lower.SecurityLevel == 0x02 ) {SubProcess_Table86( regAddress, regData ); WriteEEPROMFlag = WRITEENABLE;} break;  // 表 16写入
					case 0x87: if ( ddm_Lower.SecurityLevel == 0x02 ) {SubProcess_Table87( regAddress, regData ); WriteEEPROMFlag = WRITEENABLE;} break;  // 表 17写入
					case 0x91: if ( ddm_Lower.SecurityLevel == 0x02 )  SubProcess_Table91( regAddress, regData ); break;  // 表91写入
					case 0x92: if ( ddm_Lower.SecurityLevel == 0x02 )  SubProcess_Table92( regAddress, regData ); break;  // 表 92写入
					default:break;
				}

			} // End of  if ( regAddress > 0x7F )

			regAddress++;

		} // End of  for ( processed_bytes = 0; processed_bytes < I2c_Buffer.NumOfEntries; processed_bytes++ )

		// 保存数据( 保存模式打开)
		if ( ( WriteEEPROMFlag == WRITEENABLE ) && ( ddm_T80.ModuleDefCtrl_SYS.Bits.EEPROMSaveMode == 0 ) )
		{
			if ( regAddress < TABLE_LOWER_CTRL_SIZE )
			{
				FLASH_Update( TABLE_LOWER_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_Lower, TABLE_LOWER_CTRL_SIZE );
			}
			else if ( regAddress > 0x7F )
			{
				switch ( ddm_Lower.PageSelect )
				{
					case 0x00: FLASH_Update( TABLE00_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T00, TABLE00_SIZE );break;
					case 0x80: FLASH_Update( TABLE80_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T80, TABLE80_SIZE );break;
					case 0x81: FLASH_Update( TABLE81_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T81, TABLE81_SIZE );break;
					case 0x82: FLASH_Update( TABLE82_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T82, TABLE82_SIZE );break;
					case 0x83: FLASH_Update( TABLE83_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T83, TABLE83_SIZE );break;
					case 0x84: FLASH_Update( TABLE84_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T84, TABLE84_SIZE );break;
					case 0x86: FLASH_Update( TABLE86_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T86, TABLE86_SIZE );break;
					case 0x87: FLASH_Update( TABLE87_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T87, TABLE87_SIZE );break;

					default:break;
				}
			}
		}

		// 维护写之后的当前地址
		I2c_Buffer.A2_RegAddress = regAddress;

	} // End of else  // 处理A2地址写入的数据

	Prefetch_Data();

	// 清空I2C缓存区,并将缓存区状态修正为IDLE状态
	I2c_Buffer.NumOfEntries = 0x00;
	I2c_Buffer.Status 		= I2C_BUFFER_IDLE;

	// enable I2C slave
	// InitI2cSlave();
}


//-----------------------------------------------------------------------------
// SubProcess_Table_A0
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     处理A0数据
//
//-----------------------------------------------------------------------------
void SubProcess_Table_A0 ( uint8_t regAddress, uint8_t regData )
{
	( (uint8_t SI_SEG_XDATA*)&ddm_A0Data )[regAddress] = regData;
}



//-----------------------------------------------------------------------------
// SubProcess_TableLower
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     处理Lower数据
//
//-----------------------------------------------------------------------------
void SubProcess_TableLower ( uint8_t regAddress, uint8_t regData )
{
	if ( regAddress < TABLE_LOWER_SIZE )
	{
		( (uint8_t SI_SEG_XDATA*)&ddm_Lower )[regAddress] = regData;
	}
}


//-----------------------------------------------------------------------------
// SubProcess_Table00
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     处理Table00数据
//
//-----------------------------------------------------------------------------
void SubProcess_Table00 ( uint8_t regAddress, uint8_t regData )
{
	if ( regAddress < (0x80 + TABLE00_SIZE) )
	{
		( (uint8_t SI_SEG_XDATA*)&ddm_T00 )[regAddress - 0x80] = regData;
	}
}


//-----------------------------------------------------------------------------
// SubProcess_Table80
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures    处理Table80数据
//
//-----------------------------------------------------------------------------
void SubProcess_Table80 ( uint8_t regAddress, uint8_t regData )
{
	if ( regAddress < (0x80 + TABLE80_SIZE) )
	{
		( (uint8_t SI_SEG_XDATA*)&ddm_T80 )[regAddress - 0x80] = regData;

		if ( ddm_T80.ModuleDefCtrl_SYS.Bits.SystemReset == 1 )
		{
			ddm_T80.ModuleDefCtrl_SYS.Bits.SystemReset = 0;
			SystemReset();
		}
	}
}


//-----------------------------------------------------------------------------
// SubProcess_Table81
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures    处理Table81数据
//
//-----------------------------------------------------------------------------
void SubProcess_Table81 ( uint8_t regAddress, uint8_t regData )
{
	if ( regAddress < (0x80 + TABLE81_SIZE) )
	{
		( (uint8_t SI_SEG_XDATA*)&ddm_T81 )[regAddress - 0x80] = regData;
	}
}


//-----------------------------------------------------------------------------
// SubProcess_Table12
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures    处理Table82数据
//
//-----------------------------------------------------------------------------
void SubProcess_Table82 ( uint8_t regAddress, uint8_t regData )
{
	if ( regAddress < (0x80 + TABLE82_SIZE) )
	{
		( (uint8_t SI_SEG_XDATA*)&ddm_T82 )[regAddress - 0x80] = regData;
	}
}


//-----------------------------------------------------------------------------
// SubProcess_Table13
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures    处理Table83数据
//
//-----------------------------------------------------------------------------
void SubProcess_Table83 ( uint8_t regAddress, uint8_t regData )
{
	if ( regAddress < (0x80 + TABLE83_SIZE) )
	{
		( (uint8_t SI_SEG_XDATA*)&ddm_T83 )[regAddress - 0x80] = regData;
	}
}


//-----------------------------------------------------------------------------
// SubProcess_Table84
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures    处理Table84数据
//
//-----------------------------------------------------------------------------
void SubProcess_Table84 ( uint8_t regAddress, uint8_t regData )
{
	if ( regAddress < (0x80 + TABLE84_SIZE) )
	{
		( (uint8_t SI_SEG_XDATA*)&ddm_T84 )[regAddress - 0x80] = regData;
	}
}


//-----------------------------------------------------------------------------
// SubProcess_Table86
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures    处理Table86数据
//
//-----------------------------------------------------------------------------
void SubProcess_Table86 ( uint8_t regAddress, uint8_t regData )
{
	if ( regAddress < (0x80 + TABLE86_SIZE) )
	{
		( (uint8_t SI_SEG_XDATA*)&ddm_T86 )[regAddress - 0x80] = regData;
		I2C_MALD_37645B_Write_Byte( regAddress - 0x80, regData );
	}
}


//-----------------------------------------------------------------------------
// SubProcess_Table17
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures    处理Table87数据
//
//-----------------------------------------------------------------------------
void SubProcess_Table87 ( uint8_t regAddress, uint8_t regData )
{
	if ( regAddress < (0x80 + TABLE87_SIZE) )
	{
		( (uint8_t SI_SEG_XDATA*)&ddm_T87 )[regAddress - 0x80] = regData;
		I2C_MATA_37644B_Write_Byte( regAddress-0x80, regData );
	}
}



//-----------------------------------------------------------------------------
// SubProcess_Table91
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures    处理Table91数据
//
//-----------------------------------------------------------------------------
void SubProcess_Table91 ( uint8_t regAddress, uint8_t regData )
{
	uint8_t readdata;

	if ( regAddress < (0x80 + TABLE91_SIZE) )
	{
		( (uint8_t SI_SEG_XDATA*)&ddm_T91 )[regAddress - 0x80] = regData;

		if( regAddress == 0x85 )
		{
			I2C_Master_write_byte( (uint8_t)ddm_T91.Device_Addr, (uint8_t)ddm_T91.Device_WrAddr, (uint8_t)ddm_T91.Device_WrData );
		}
		else if( regAddress == 0x87 )
		{
			I2C_Master_read_byte( (uint8_t)ddm_T91.Device_Addr, (uint8_t)ddm_T91.Device_RdAddr, &readdata );
			ddm_T91.Device_RdData = readdata;
		}
	}
}



//-----------------------------------------------------------------------------
// CopyAndSetRamData
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures    处理Table92数据
//
//-----------------------------------------------------------------------------
void SubProcess_Table92 ( uint8_t regAddress, uint8_t regData )
{
	if ( regAddress < (0x80 + TABLE92_SIZE) )
	{
		( (uint8_t SI_SEG_XDATA*)&ddm_T92 )[regAddress - 0x80] = regData;

		if ( ( regAddress == 0x80 ) && ( ddm_T80.ModuleDefCtrl_SYS.Bits.EEPROMSaveMode == 1 ) )
		{
			switch( ddm_T92.SaveTableNum )
			{
				case 0x00: FLASH_Update( TABLE00_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T00, TABLE00_SIZE );break;
				case 0x80: FLASH_Update( TABLE80_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T80, TABLE80_SIZE );break;
				case 0x81: FLASH_Update( TABLE81_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T81, TABLE81_SIZE );break;
				case 0x82: FLASH_Update( TABLE82_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T82, TABLE82_SIZE );break;
				case 0x83: FLASH_Update( TABLE83_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T83, TABLE83_SIZE );break;
				case 0x84: FLASH_Update( TABLE84_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T84, TABLE84_SIZE );break;
				case 0x86: FLASH_Update( TABLE86_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T86, TABLE86_SIZE );break;
				case 0x87: FLASH_Update( TABLE87_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_T87, TABLE87_SIZE );break;

				case 0xFF: FLASH_Update( TABLE_LOWER_ADDRESS, (uint8_t SI_SEG_XDATA*)&ddm_Lower, TABLE_LOWER_CTRL_SIZE );break;
				default:break;
			}
		}
	}
}
