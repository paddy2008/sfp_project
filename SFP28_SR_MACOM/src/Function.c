#include "Include.h"

// 各数据表结构
SI_SEGMENT_VARIABLE( ddm_Lower,  extern struct LOWER_MEMORY_MAP_struct, 				SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T80,    extern struct Table_General_Control_struct,   		    SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T81,    extern struct Table_TX_Cal_Ctrl_struct,       		    SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T82,    extern struct Table_RX_Cal_Ctrl_struct,       		    SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T84,    extern struct Table_ModSet_Calibration_struct,       	SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T86,    extern struct Table_Device_MALD_37645B_struct,         SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T87,    extern struct Table_Device_MATA_37644B_struct,       	SI_SEG_XDATA );

SI_SEGMENT_VARIABLE( Module_ADC_DOM_s,  extern struct Module_ADC_DOM_struct,  SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( I2c_Buffer,        extern struct I2C_BUFFER_Struct,	  SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( Module_Status_s,   extern struct Module_Status_struct,   SI_SEG_DATA  );
SI_SEGMENT_VARIABLE( Module_CtrlData_s,        struct Module_CtrlData_struct, SI_SEG_XDATA );

SI_LOCATED_VARIABLE_NO_INIT(TEMPSENSOR_0C_LOW,  uint8_t const, SI_SEG_CODE, TEMP_CAL_ADDRESS_LOW);
SI_LOCATED_VARIABLE_NO_INIT(TEMPSENSOR_0C_HIGH, uint8_t const, SI_SEG_CODE, TEMP_CAL_ADDRESS_HIGH);
//-----------------------------------------------------------------------------
// InitPrivateData
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     初始化私有数据
//
//-----------------------------------------------------------------------------
void InitPrivateData(void)
{
    // 初始化I2C缓存数据
	memset( (uint8_t SI_SEG_XDATA *)&I2c_Buffer, 0x00, sizeof(struct I2C_BUFFER_Struct) );

	I2c_Buffer.SubAddress    = A0_7Bits_ADDR;
	I2c_Buffer.A0_RegAddress = 0;
	I2c_Buffer.A2_RegAddress = 0;

	// 执行一次预取数据
	Prefetch_Data();

	// 初始化ADC DOM数据
	memset( (uint8_t SI_SEG_XDATA *)&Module_ADC_DOM_s, 0x00, sizeof(struct Module_ADC_DOM_struct) );

	// 初始化模块状态数据
	memset( (uint8_t SI_SEG_DATA *)&Module_Status_s, 0x00, sizeof(struct Module_Status_struct) );

	// 设定完成启动代码后，立即执行所有功能一遍
	Module_Status_s.Flush_DOMData = 1;

	// 设定LD驱动器状态为初始状态
	Module_Status_s.LD_Driver_ON_Flag = 0xFF;

	// 设定TOSA状态为初始状态
	Module_Status_s.TOSA_ON_Flag = 0xFF;

	// 设定 RxPower Dark 标志为无光状态
	Module_Status_s.RxPower_Dark_Flag = 0;

    // 设置I2C忙标志
	Module_Status_s.I2c_Busy_Flag = 0;

	// 初始化模块控制数据
	memset( (uint8_t SI_SEG_XDATA *)&Module_CtrlData_s, 0x00, sizeof(struct Module_CtrlData_struct) );

	Module_ADC_DOM_s.ADCValues.tempsensor_0c = (TEMPSENSOR_0C_HIGH << 8) | TEMPSENSOR_0C_LOW;
}


//-----------------------------------------------------------------------------
// InitDeviceData
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     初始化驱动器，外设
//
//-----------------------------------------------------------------------------
void InitDeviceData(void)
{
	Port_RESETB  = true;

	I2C_MALD_37645B_Write_Byte( 0x02, 0xAA );
	I2C_MALD_37645B_Write_Byte( 0x02, 0x00 );

	I2C_MALD_37645B_Read_Byte( 0x00, &(ddm_T86.CHIPID) );
	if( ddm_T86.CHIPID != 0x8F )
	{
		;
	}
	else
	{
		I2C_MALD_37645B_Write_Byte( 0x03, ddm_T86.Misc_IO_Control );
		I2C_MALD_37645B_Write_Byte( 0x04, ddm_T86.CDRctrl );
		I2C_MALD_37645B_Write_Byte( 0x05, ddm_T86.I2C_address_mode );
		I2C_MALD_37645B_Write_Byte( 0x06, ddm_T86.Channel_mode );
		I2C_MALD_37645B_Write_Byte( 0x07, ddm_T86.Lockphase );
		I2C_MALD_37645B_Write_Byte( 0x0C, ddm_T86.LOS_LOL_Tx_Alarm );
		I2C_MALD_37645B_Write_Byte( 0x0D, ddm_T86.IgnoreTxFault );
		I2C_MALD_37645B_Write_Byte( 0x10, ddm_T86.LOS_THRSH_AUTO_SQUELCH );
		I2C_MALD_37645B_Write_Byte( 0x12, ddm_T86.CTLE_X );
		I2C_MALD_37645B_Write_Byte( 0x20, ddm_T86.OUTPUT_MUTE_SLEW );
		I2C_MALD_37645B_Write_Byte( 0x21, ddm_T86.Ibias );
		I2C_MALD_37645B_Write_Byte( 0x22, ddm_T86.Imod );
		I2C_MALD_37645B_Write_Byte( 0x23, ddm_T86.PreFall );
		I2C_MALD_37645B_Write_Byte( 0x24, ddm_T86.TDE );
		I2C_MALD_37645B_Write_Byte( 0x25, ddm_T86.CrossingAdj );
		I2C_MALD_37645B_Write_Byte( 0x27, ddm_T86.Iburnin );
		I2C_MALD_37645B_Write_Byte( 0x28, ddm_T86.Burnin_enable );
		I2C_MALD_37645B_Write_Byte( 0x60, ddm_T86.ADC_config0 );
	}


	I2C_MATA_37644B_Write_Byte( 0x02, 0xAA );
	I2C_MATA_37644B_Write_Byte( 0x02, 0x00 );

	I2C_MATA_37644B_Read_Byte( 0x00, &(ddm_T87.CHIPID));
	if( ddm_T87.CHIPID != 0x8E )
	{
		;
	}
	else
	{
		// RX device initialization
		I2C_MATA_37644B_Write_Byte( 0x03, ddm_T87.MONITORS );
		I2C_MATA_37644B_Write_Byte( 0x04, ddm_T87.CDRctrl );
		I2C_MATA_37644B_Write_Byte( 0x05, ddm_T87.I2C_address_mode );
		I2C_MATA_37644B_Write_Byte( 0x06, ddm_T87.Channel_mode );
		I2C_MATA_37644B_Write_Byte( 0x07, ddm_T87.Lockphase );
		I2C_MATA_37644B_Write_Byte( 0x0A, ddm_T87.LOS_mode );
		I2C_MATA_37644B_Write_Byte( 0x0C, ddm_T87.LOS_LOL_alarm );
		I2C_MATA_37644B_Write_Byte( 0x10, ddm_T87.LOS_CTRL );
		I2C_MATA_37644B_Write_Byte( 0x11, ddm_T87.SLA );
		I2C_MATA_37644B_Write_Byte( 0x12, ddm_T87.TIA_CTRL );
		I2C_MATA_37644B_Write_Byte( 0x20, ddm_T87.OUTPUT_CTRL );
		I2C_MATA_37644B_Write_Byte( 0x21, ddm_T87.OUTPUT_SWING );
		I2C_MATA_37644B_Write_Byte( 0x22, ddm_T87.OUTPUT_DEEMPH );
		I2C_MATA_37644B_Write_Byte( 0x60, ddm_T87.ADC_config0 );
	}

	Module_CtrlData_s.TxPath.BiasSet = ddm_T86.Ibias;
	Module_CtrlData_s.TxPath.ModSet  = ddm_T86.Imod;
}


//-----------------------------------------------------------------------------
// UpdateModuleStatus
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     更新模块状态数据
//
//-----------------------------------------------------------------------------
void UpdateModuleStatus(void)
{
	uint8_t regdata;

	ddm_Lower.G_CtrlStatus_Bits.CtrlStatus_110.Bits.Rate_Select_0 = 0;//Port_RS0;
	ddm_Lower.G_CtrlStatus_Bits.CtrlStatus_110.Bits.Rate_Select_1 = 0;//Port_RS1;
	ddm_Lower.G_CtrlStatus_Bits.CtrlStatus_110.Bits.TxDisable     = Port_TX_DIS;
	Port_TX_DIS_LD                                                = Port_TX_DIS;

	I2C_MATA_37644B_Read_Byte( 0x0B, &regdata );
	ddm_Lower.G_CtrlStatus_Bits.CtrlStatus_110.Bits.RX_LOS = (regdata & 0x01);

	ddm_Lower.G_CtrlStatus_Bits.CtrlStatus_110.Bits.TX_Fault = Port_TX_FAULT_IN;
	Port_TX_FAULT_OUT                                        = Port_TX_FAULT_IN;

	// TX_CDR_LOL Bit 1 Interrupt: CDR loss-of-lock flag:0 = Deasserted 1 = Asserted (default)
	I2C_MALD_37645B_Read_Byte( 0x0B, &regdata );
	ddm_Lower.E_CtrlStatus_Bits.CtrlStatus_119.Bits.TxCDRUnlocked = (regdata >> 1) & 0x01;

	// RX_CDR_LOL Bit 1 Interrupt: CDR loss-of-lock flag:0 = Deasserted 1 = Asserted (default)
	I2C_MATA_37644B_Read_Byte( 0x0B, &regdata );
	ddm_Lower.E_CtrlStatus_Bits.CtrlStatus_119.Bits.RxCDRUnlocked = (regdata >> 1) & 0x01;
}


//-----------------------------------------------------------------------------
// TOSA_LDChip_ON_OFF
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     TOSA/LD芯片打开关闭
//
//-----------------------------------------------------------------------------
void TOSA_LDChip_ON_OFF(void)
{
	// 开启发射端，必须模块正常
	if ( ( ddm_Lower.G_CtrlStatus_Bits.CtrlStatus_110.Bits.Soft_TxDisable == 0 ) && (Port_TX_DIS == 0) )
	{
		//允许激光器打开
		if ( Module_Status_s.LD_Driver_ON_Flag != 1 )
		{
			Port_TX_DIS_LD = 0;
			Module_Status_s.LD_Driver_ON_Flag = 1;
		}
	}
	else
	{
		// 将激光器关闭
		if ( Module_Status_s.LD_Driver_ON_Flag != 0 )
		{
			Port_TX_DIS_LD = 1;
			Module_Status_s.LD_Driver_ON_Flag = 0;
		}
	}

	// 更新当前TOSA状态状态
	// LD_Driver_ON_Flag 为1，且没有发生Fault、TxDis，TOSA 打开
	if (    ( Module_Status_s.LD_Driver_ON_Flag == 1 )
	     && ( ddm_Lower.G_CtrlStatus_Bits.CtrlStatus_110.Bits.TxDisable  == 0 )
	     && ( ddm_Lower.G_CtrlStatus_Bits.CtrlStatus_110.Bits.TX_Fault   == 0 )  )
	{
		Module_Status_s.TOSA_ON_Flag =  1;
	}
	else
	{
		Module_Status_s.TOSA_ON_Flag =  0;
	}
}


//-----------------------------------------------------------------------------
// TxPowerAdjust
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     发射光功率调节
//
//-----------------------------------------------------------------------------
void TxPowerAdjust(void)
{
	float       TxPowerSet;
	float       TxPowerDom;
	float       BiasDom;

	// 如果TOSA是关闭状态，这个函数什么也不做
	if ( Module_Status_s.TOSA_ON_Flag == 0 )
	{
		return;
	}

    // 发射光功率目标值
	BiasDom    = Module_ADC_DOM_s.DOMValues.Bias * 0.002;
	TxPowerSet = ddm_T81.SoftAPC_Target * 0.1;
	TxPowerDom = Module_ADC_DOM_s.DOMValues.TxPower * 0.1;

	// 偏置电流的设定寄存值在限流的情况，不可以大于预订的DAC最大值
	// 同时设定快速更新标志，以快速调整BIAS的电流值，并不作下面的判断了
	if ( ddm_T80.ModuleDefCtrl_TX.Bits.BiasLimit_Enable == 1 )
	{
		if ( BiasDom > (float)ddm_T81.Bias_Max )
		{
			Module_CtrlData_s.TxPath.BiasSet--;

			I2C_MALD_37645B_Write_Byte( 0x21, Module_CtrlData_s.TxPath.BiasSet );

			Module_Status_s.Flush_DOMData = 1;
			return;
		}
	}

	// 判断光功率自动调节功能是否打开
	if( ddm_T80.ModuleDefCtrl_TX.Bits.SoftAPC_Enable == 1 )
	{
		// 发射光功率超过光功率设定值3%，需要减少偏置电流值
		if ( ( TxPowerDom > (TxPowerSet * 1.03) ) && ( Module_CtrlData_s.TxPath.BiasSet > 0 ) )
		{
			Module_CtrlData_s.TxPath.BiasSet--;

			I2C_MALD_37645B_Write_Byte( 0x21, Module_CtrlData_s.TxPath.BiasSet );

			Module_Status_s.Flush_DOMData = 1;
		}

		// 发射光功率小于光功率设定值97%，需要增加偏置电流值
		if (  ( TxPowerDom < (TxPowerSet * 0.97) ) && ( Module_CtrlData_s.TxPath.BiasSet < 0x7F ) )
		{
			// 如果开启了限流 电流DAC最多设定到计算出来的最大值
			// 如果关闭Bias限流
			if ( ddm_T80.ModuleDefCtrl_TX.Bits.BiasLimit_Enable == 0 || BiasDom < (float)ddm_T81.Bias_Max )
			{
				Module_CtrlData_s.TxPath.BiasSet++;

				I2C_MALD_37645B_Write_Byte( 0x21, Module_CtrlData_s.TxPath.BiasSet );

				Module_Status_s.Flush_DOMData = 1;
			}
		}
	}
	else
	{
		// 光功率自动调节功能关闭时，写入偏置DAC预设值。
		// 如果偏置DAC的预设值太大，会出现设定后，偏置电流大，导致限流发生，光功率出现跳动的情况
		if ( ddm_T81.BiasSet != Module_CtrlData_s.TxPath.BiasSet )
		{
			Module_CtrlData_s.TxPath.BiasSet = ddm_T86.Ibias;
			I2C_MALD_37645B_Write_Byte( 0x21, Module_CtrlData_s.TxPath.BiasSet );
		}
	}
}



//-----------------------------------------------------------------------------
// ModCurrentAdjust
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     调制电流调节
//
//-----------------------------------------------------------------------------
void ModCurrentAdjust(void)
{
	uint16_t   ModSet;
	float      ModSet_Float;

	// 如果TOSA是关闭状态，这个函数什么也不做
	if ( Module_Status_s.TOSA_ON_Flag == 0 )
	{
		return;
	}

	if ( ddm_T80.ModuleDefCtrl_TX.Bits.ModLUT_Enable == 0 )  // 调制电流校正功能已关闭
	{
		ModSet = ddm_T86.Imod;
	}
	else
	{
		if ( Module_Status_s.TemperatureLevel_Flag == 0 )
		{
			ModSet_Float = Module_ADC_DOM_s.DOMValues.ChipTemperature / 256.0 * ddm_T84.ModSet_Slope_L + ddm_T84.ModSet_Offset_L;
		}
		else
		{
			ModSet_Float = Module_ADC_DOM_s.DOMValues.ChipTemperature / 256.0 * ddm_T84.ModSet_Slope_H + ddm_T84.ModSet_Offset_H;
		}

		if ( ModSet_Float >= 255.0 )
		{
			ModSet = 255;
		}
		else if ( ModSet_Float <= 0.0 )
		{
			ModSet = 0;
		}
		else
		{
			ModSet = (uint8_t)ModSet_Float;
		}
	}

	if ( Module_CtrlData_s.TxPath.ModSet != ModSet )
	{
		Module_CtrlData_s.TxPath.ModSet = ModSet;
		I2C_MALD_37645B_Write_Byte( 0x22, Module_CtrlData_s.TxPath.ModSet );
	}
}


//-----------------------------------------------------------------------------
// SetDeviceCtrlData
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     设置芯片的控制数据
//
//-----------------------------------------------------------------------------
void SetDeviceCtrlData(void)
{
	// Loss Of Signal (LOS) hysteresis & assert threshold level control register.
	if ( Module_CtrlData_s.RxPath.RXLOSSet_1 != ddm_T82.RxLosAssert )
	{
		Module_CtrlData_s.RxPath.RXLOSSet_1 = ddm_T82.RxLosAssert;
		I2C_MATA_37644B_Write_Byte( 0x10, Module_CtrlData_s.RxPath.RXLOSSet_1 );
	}

	// 激光器关闭，不进行参数设置
	if ( Module_Status_s.TOSA_ON_Flag == 0 )
	{
		return;
	}

	if ( Module_CtrlData_s.TxPath.CPSet != ddm_T86.CrossingAdj )
	{
		Module_CtrlData_s.TxPath.CPSet = ddm_T86.CrossingAdj;
		I2C_MALD_37645B_Write_Byte( 0x25, Module_CtrlData_s.TxPath.CPSet );
	}
}
