#include "include.h"


SI_SEGMENT_VARIABLE( ddm_Lower,  extern struct LOWER_MEMORY_MAP_struct, 				SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T80,    extern struct Table_General_Control_struct,   			SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T81,    extern struct Table_TX_Cal_Ctrl_struct,       			SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T82,    extern struct Table_RX_Cal_Ctrl_struct,       			SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T83,    extern struct Table_Temperature_Compensation_struct,  	SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T84,    extern struct Table_ModSet_Calibration_struct,       	SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T90,    extern struct Table_Monitor_struct,		            SI_SEG_XDATA );

SI_SEGMENT_VARIABLE( Module_Status_s,   extern struct Module_Status_struct,   SI_SEG_DATA  );
SI_SEGMENT_VARIABLE( Module_ADC_DOM_s,  extern struct Module_ADC_DOM_struct,  SI_SEG_XDATA );
//SI_SEGMENT_VARIABLE( Module_CtrlData_s, extern struct Module_CtrlData_struct, SI_SEG_XDATA );

//-----------------------------------------------------------------------------
// GetDOM_Voltage
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     ��ȡ��ѹDOM����
//
//-----------------------------------------------------------------------------
void GetDOM_Voltage(void)
{
	float 	Voltage;

	Module_ADC_DOM_s.ADCValues.Voltage =  ADC0_Read(ADC0_POSITIVE_INPUT_VDD);

	// ��λ0.1mV
	Voltage = ( ( (float)Module_ADC_DOM_s.ADCValues.Voltage ) * ddm_T80.V_Reference / 16384.0 * 4 ) * 10000;

	Module_ADC_DOM_s.DOMValues.Voltage = ConvertFloatToUnsignedintWithRangeCheck(Voltage);
}


//-----------------------------------------------------------------------------
// GetDOM_Temperature
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     ��ȡ�¶�DOM����
//
//-----------------------------------------------------------------------------
void GetDOM_Temperature(void)
{
	float        Temp;
	int32_t      temp_long;
	int32_t      chiptemp_long;

	static uint16_t boot_time = 0;

	//��������ʱ����������ֵ�������ֵ��ҪԶԶ���������������㷨�е�Kֵ
	if ( boot_time <= 30000 ) boot_time++;

	// �¶ȼ��㹫ʽ
	Module_ADC_DOM_s.ADCValues.Temperature = ADC0_Read(ADC0_POSITIVE_INPUT_TEMP);

	Temp = ( (float)Module_ADC_DOM_s.ADCValues.Temperature * 4 - Module_ADC_DOM_s.ADCValues.tempsensor_0c) / 28.0;
	Temp = ( Temp - ddm_T83.Offset_Sensor ) / ddm_T83.Slope_Sensor;

	chiptemp_long = (int32_t)((Temp - (float)ddm_T83.Offset_Chip_PCB - (float)ddm_T83.Offset_PCB_Shell) * 256.0);

	// һ���¶��ȶ���������0.5�����������Ҫ����1��ĳ��ͣ���ֹ���м���������ߵ�������
	if ( Temp < ( ddm_T84.ChipTemperature_Normal - 1 ) )
	{
		Module_Status_s.TemperatureLevel_Flag = 0;
	}
	else if ( Temp > ( ddm_T84.ChipTemperature_Normal + 1 ) )
	{
		Module_Status_s.TemperatureLevel_Flag = 1;
	}

	// ����������ʾ���¶ȱ���ֵ
	if ( ddm_T80.ModuleDefCtrl_SYS.Bits.Temperature_Compensation_Enable == 1 )
	{
		// IC_PCB���¶�ƫ����뵽�������У������IC�Ĺ������ԣ����ϵ�����û�й�ϵ
		Temp = Temp - (float)ddm_T83.Offset_Chip_PCB
	                - ((float)ddm_T83.Offset_PCB_Shell + (float)ddm_T83.Offset_Display)
	                * (float)boot_time / ( (float)boot_time + 1000 );
	}
	else
	{
		Temp = Temp - (float)ddm_T83.Offset_Chip_PCB
	                - ((float)ddm_T83.Offset_PCB_Shell + (float)ddm_T83.Offset_Display);
	}

	// ���������ʵ�ʵ��¶�ֵת��ΪDOM��λ 1/256 C
	temp_long = (int32_t)(Temp * 256.0);

	if ( temp_long > 0x7FFF )
	{
		temp_long = 0x7FFF;   // ����ֵ��������ʾֵ
	}
	else if ( temp_long < (-32768) )
	{
		temp_long = -32768;   // ����ֵС����С��ʾֵ
	}


	if ( chiptemp_long > 0x7FFF )
	{
		chiptemp_long = 0x7FFF;   // ����ֵ��������ʾֵ
	}
	else if ( chiptemp_long < (-32768) )
	{
		chiptemp_long = -32768;  // ����ֵС����С��ʾֵ
	}

	Module_ADC_DOM_s.DOMValues.Temperature     = (int16_t)temp_long;
	Module_ADC_DOM_s.DOMValues.ChipTemperature = (int16_t)chiptemp_long;
}


//-----------------------------------------------------------------------------
// GetDOM_LDBias
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     ��ȡLD Bias DOM����
//
//-----------------------------------------------------------------------------
void GetDOM_LDBias(void)
{
	int       i;
	uint16_t  rWord;
	uint16_t  VAR2,VAR3;
	uint16_t  ADCvalue[3];
	float 	  Bias;

	// ���������رգ���ƫ�õ���Ϊ0��
	if ( Module_Status_s.TOSA_ON_Flag == 0 )
	{
		Bias = 0.0;
	}
	else
	{
		for( i = 0; i < 3; i++ )
		{
			I2C_MALD_37645B_Write_Byte( 0x60, 0x00 );
			I2C_MALD_37645B_Write_Byte( 0x67, 0x02 );
			delay_us( 100 );
			I2C_MALD_37645B_Read_Word( 0x65,  &rWord );
			VAR2 = ((rWord & 0xFF00)>>4) | (rWord & 0x000F);

			I2C_MALD_37645B_Write_Byte( 0x67, 0x03 );
			delay_us( 100 );
			I2C_MALD_37645B_Read_Word( 0x65,  &rWord );
			VAR3 = ((rWord & 0xFF00)>>4) | (rWord & 0x000F);

			ADCvalue[i] = (VAR2 - VAR3);
		}

		Module_ADC_DOM_s.ADCValues.Bias = ( ADCvalue[0] + ADCvalue[1] + ADCvalue[2] ) / 3;

		Bias = ((float)Module_ADC_DOM_s.ADCValues.Bias*1.0*40.1) / 2.0;
	}

	Module_ADC_DOM_s.DOMValues.Bias = ConvertFloatToUnsignedintWithRangeCheck(Bias);
}


//-----------------------------------------------------------------------------
// GetDOM_TxPower
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     ��ȡTxPower DOM����
//
//-----------------------------------------------------------------------------
void GetDOM_TxPower(void)
{
	float    TxPower;
	uint16_t rWord;

	// Select ADC Monitors IMON pad
	I2C_MALD_37645B_Write_Byte( 0x60, 0x02 );
	delay_us( 100 );
	I2C_MALD_37645B_Read_Word( 0x65,  &rWord );
	Module_ADC_DOM_s.ADCValues.TxPower = ((rWord & 0xFF00)>>4) | (rWord & 0x000F);


	if ( Module_Status_s.TOSA_ON_Flag == 0 )
	{
		TxPower = 1;
	}
	else
	{
		// TXPowerУ��������Ҫ��0.1uw��λ�̶�
		TxPower = ddm_T81.TXPowerCal_Slope * Module_ADC_DOM_s.ADCValues.TxPower + ddm_T81.TXPowerCal_Offset;

		if ( TxPower < 1 )
		{
			TxPower = 1;  // ��ֹ������ָ��Ĺ⹦��ֵ
		}
	}

	Module_ADC_DOM_s.DOMValues.TxPower = ConvertFloatToUnsignedintWithRangeCheck(TxPower);
}


//-----------------------------------------------------------------------------
// GetDOM_RxPower
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     ��ȡRxPower DOM����
//
//-----------------------------------------------------------------------------
void GetDOM_RxPower(void)
{
	float RxPower;

	Module_ADC_DOM_s.ADCValues.RxPower = ADC0_Read(ADC0_POSITIVE_INPUT_P3);
	if ( ( ( Module_Status_s.RxPower_Dark_Flag == 1 ) && ( Module_ADC_DOM_s.ADCValues.RxPower < ddm_T82.RxADC_DarkUp   ) ) ||
		 ( ( Module_Status_s.RxPower_Dark_Flag == 0 ) && ( Module_ADC_DOM_s.ADCValues.RxPower < ddm_T82.RxADC_DarkDown ) )	   )
	{
		RxPower = 1;
		Module_Status_s.RxPower_Dark_Flag = 0;  // �޹�״̬
	}
	else
	{
		// RXPowerУ��������Ҫ��0.1uw��λ�̶�
		// Rx = f(ADC) = B4 * RxADC^4 + B3 * RxADC^3 + B2 * RxAdc^2 + B1 * RxAdc + B0
		//RxPower = (float)Module_ADC_DOM_s.ADCValues.RxPower * ddm_T82.RxPowerCal_Slope_4  + ddm_T82.RxPowerCal_Slope_3;
		//RxPower = (float)Module_ADC_DOM_s.ADCValues.RxPower * RxPower                     + ddm_T82.RxPowerCal_Slope_2;
		//RxPower = (float)Module_ADC_DOM_s.ADCValues.RxPower * RxPower                     + ddm_T82.RxPowerCal_Slope_1;
		//RxPower = (float)Module_ADC_DOM_s.ADCValues.RxPower * RxPower                     + ddm_T82.RxPowerCal_Offset;
		RxPower = ddm_T82.RxPowerCal_Slope_1 * Module_ADC_DOM_s.ADCValues.RxPower + ddm_T82.RxPowerCal_Offset;

		if ( RxPower < 1 )
		{
			RxPower = 1;  // ��ֹ������ָ��Ĺ⹦��ֵ
		}

		Module_Status_s.RxPower_Dark_Flag = 1;  // �й�״̬
	}

	Module_ADC_DOM_s.DOMValues.RxPower = ConvertFloatToUnsignedintWithRangeCheck(RxPower);
}


//-----------------------------------------------------------------------------
// UpdateAlarmAndWarningBits
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     ���¸澯�ֽ�����
//
//-----------------------------------------------------------------------------
void UpdateAlarmAndWarningBits(void)
{
	uint8_t AlarmByte1   = 0;
	uint8_t AlarmByte2   = 0;
	uint8_t WarningByte1 = 0;
	uint8_t WarningByte2 = 0;


	// �Ƚ��¶���ֵ
	if ( Module_ADC_DOM_s.DOMValues.Temperature > ddm_Lower.Threshold_Values.Temperature.High_Alarm )
	{
		AlarmByte1 |= 0x80;
	}
	else if ( Module_ADC_DOM_s.DOMValues.Temperature < ddm_Lower.Threshold_Values.Temperature.Low_Alarm )
	{
		AlarmByte1 |= 0x40;
	}

	if ( Module_ADC_DOM_s.DOMValues.Temperature > ddm_Lower.Threshold_Values.Temperature.High_Warning )
	{
		WarningByte1 |= 0x80;
	}
	else if ( Module_ADC_DOM_s.DOMValues.Temperature < ddm_Lower.Threshold_Values.Temperature.Low_Warning )
	{
		WarningByte1 |= 0x40;
	}

	// �Ƚϵ�ѹ��ֵ
	if ( Module_ADC_DOM_s.DOMValues.Voltage > ddm_Lower.Threshold_Values.Voltage.High_Alarm )
	{
		AlarmByte2 |= 0x20;
	}
	else if ( Module_ADC_DOM_s.DOMValues.Voltage < ddm_Lower.Threshold_Values.Voltage.Low_Alarm )
	{
		AlarmByte2 |= 0x10;
	}

	if ( Module_ADC_DOM_s.DOMValues.Voltage > ddm_Lower.Threshold_Values.Voltage.High_Warning )
	{
		WarningByte2 |= 0x20;
	}
	else if ( Module_ADC_DOM_s.DOMValues.Voltage < ddm_Lower.Threshold_Values.Voltage.Low_Warning )
	{
		WarningByte2 |= 0x10;
	}


	// �Ƚ�ƫ�õ�����ֵ
	if ( Module_ADC_DOM_s.DOMValues.Bias > ddm_Lower.Threshold_Values.TXBais.High_Alarm )
	{
		AlarmByte1 |= 0x08;
	}
	else if ( Module_ADC_DOM_s.DOMValues.Bias < ddm_Lower.Threshold_Values.TXBais.Low_Alarm )
	{
		AlarmByte1 |= 0x04;
	}

	if ( Module_ADC_DOM_s.DOMValues.Bias > ddm_Lower.Threshold_Values.TXBais.High_Warning )
	{
		WarningByte1 |= 0x08;
	}
	else if ( Module_ADC_DOM_s.DOMValues.Bias < ddm_Lower.Threshold_Values.TXBais.Low_Warning )
	{
		WarningByte1 |= 0x04;
	}


	// �ȽϷ���⹦����ֵ
	if ( Module_ADC_DOM_s.DOMValues.TxPower > ddm_Lower.Threshold_Values.TXPower.High_Alarm )
	{
		AlarmByte1 |= 0x02;
	}
	else if ( Module_ADC_DOM_s.DOMValues.TxPower < ddm_Lower.Threshold_Values.TXPower.Low_Alarm )
	{
		AlarmByte1 |= 0x01;
	}

	if ( Module_ADC_DOM_s.DOMValues.TxPower > ddm_Lower.Threshold_Values.TXPower.High_Warning )
	{
		WarningByte1 |= 0x02;
	}
	else if ( Module_ADC_DOM_s.DOMValues.TxPower < ddm_Lower.Threshold_Values.TXPower.Low_Warning )
	{
		WarningByte1 |= 0x01;
	}

	// �ȽϽ��չ⹦����ֵ
	if ( Module_ADC_DOM_s.DOMValues.RxPower > ddm_Lower.Threshold_Values.RXPower.High_Alarm )
	{
		AlarmByte2 |= 0x80;
	}
	else if ( Module_ADC_DOM_s.DOMValues.RxPower < ddm_Lower.Threshold_Values.RXPower.Low_Alarm )
	{
		AlarmByte2 |= 0x40;
	}

	if ( Module_ADC_DOM_s.DOMValues.RxPower > ddm_Lower.Threshold_Values.RXPower.High_Warning )
	{
		WarningByte2 |= 0x80;
	}
	else if ( Module_ADC_DOM_s.DOMValues.RxPower < ddm_Lower.Threshold_Values.RXPower.Low_Warning )
	{
		WarningByte2 |= 0x40;
	}

	// ��ֵ������־�������ݽṹ��
	ddm_Lower.Alarm_Warning_Flag.AlarmFlag_112.Byte   = AlarmByte1;
	ddm_Lower.Alarm_Warning_Flag.AlarmFlag_113.Byte   = AlarmByte2;
	ddm_Lower.Alarm_Warning_Flag.WarningFlag_116.Byte = WarningByte1;
	ddm_Lower.Alarm_Warning_Flag.WarningFlag_117.Byte = WarningByte2;
}


//-----------------------------------------------------------------------------
// UpdateDOMData
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     ����DOM�ֽ�����
//
//-----------------------------------------------------------------------------
void UpdateDOMData(void)
{
	// ��I2C����Ϊ���е����������˫�ֽڵ�DOM����
	//if ( Module_Status_s.I2c_Busy_Flag == 0 )
	{
		IE_EA = 0; // �ر�ȫ���ж�

		ddm_Lower.AD_Readout.Temperature = Module_ADC_DOM_s.DOMValues.Temperature;
		ddm_Lower.AD_Readout.Voltage     = Module_ADC_DOM_s.DOMValues.Voltage;
		ddm_Lower.AD_Readout.TxBias      = Module_ADC_DOM_s.DOMValues.Bias;
		ddm_Lower.AD_Readout.TxPower     = Module_ADC_DOM_s.DOMValues.TxPower;
		ddm_Lower.AD_Readout.RxPower     = Module_ADC_DOM_s.DOMValues.RxPower;
		ddm_T90.ChipTemperatureDOM       = Module_ADC_DOM_s.DOMValues.ChipTemperature;

		ddm_T90.TemperatureADC = Module_ADC_DOM_s.ADCValues.Temperature;
		ddm_T90.TxBiasADC      = Module_ADC_DOM_s.ADCValues.Bias;
		ddm_T90.TxPowerADC     = Module_ADC_DOM_s.ADCValues.TxPower;
		ddm_T90.RxPowerADC     = Module_ADC_DOM_s.ADCValues.RxPower;
		ddm_T90.VoltageADC     = Module_ADC_DOM_s.ADCValues.Voltage;

		IE_EA = 1; // ����ȫ���ж�
	}
}
