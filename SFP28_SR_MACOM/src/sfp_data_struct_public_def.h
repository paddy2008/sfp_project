//-----------------------------------------------------------------------------
//      Copyright (c) 2017-2018,
//-----------------------------------------------------------------------------
//
// file name  :       sfp_data_struct_public_def.h
//
// description:       SFP Data struct
//
// Target     :       SFP
//
// version    :       V100.001
//					  2019/07/30
//
//-----------------------------------------------------------------------------

#ifndef SFP_DATA_STRUCT_PUBLIC_DEF_H_
#define SFP_DATA_STRUCT_PUBLIC_DEF_H_

typedef struct Threshold_Values_INT_struct
{
	int16_t High_Alarm;
    int16_t Low_Alarm;
    int16_t High_Warning;
    int16_t Low_Warning;

} Threshold_Values_INT_s;

typedef struct Threshold_Values_UINT_struct
{
    uint16_t High_Alarm;
    uint16_t Low_Alarm;
    uint16_t High_Warning;
    uint16_t Low_Warning;

} Threshold_Values_UINT_s;


// Byte0-55 Threshold Values used for Alarm and Warning Flags(56 Bytes)
typedef struct Threshold_Values_struct
{
	Threshold_Values_INT_s  Temperature;
	Threshold_Values_UINT_s Voltage;
	Threshold_Values_UINT_s TXBais;
	Threshold_Values_UINT_s TXPower;
	Threshold_Values_UINT_s RXPower;

	Threshold_Values_INT_s LaserTemperature;
	Threshold_Values_INT_s TECCurrent;

} Threshold_Values_s;

// Byte56-94 Calibration constants for External Calibration Option(39 Bytes)
typedef struct ExternalCal_Values_struct
{
	float RxPower_Slope_4;
	float RxPower_Slope_3;
	float RxPower_Slope_2;
	float RxPower_Slope_1;
	float RxPower_Offset;

	uint16_t TxBias_Slope;
	int16_t  TxBias_Offset;

	uint16_t TxPower_Slope;
	int16_t  TxPower_Offset;

	uint16_t Temperature_Slope;
	int16_t  Temperature_Offset;

	uint16_t Voltage_Slope;
	int16_t  Voltage_Offset;

	uint8_t  Reserved[3];

} ExternalCal_Values_s;

// Byte96-109 A/D readout (14 Bytes)
typedef struct AD_Readout_struct
{
	int16_t  Temperature;
	uint16_t Voltage;
	uint16_t TxBias;
	uint16_t TxPower;
	uint16_t RxPower;
	
	int16_t LaserTemperature;
	int16_t TECCurrent;
	
} AD_Readout_s;

// Byte110 General Control/Status Bits
typedef struct G_CtrlStatus_Bits_110_bits
{
	uint8_t Data_Ready_Bar :1; // Bit0 Indicates transceiver has achieved power up and A/D data is ready.
							   //      Bit remains high until data is ready to be read at which time the
							   //      device sets the bit low.

	uint8_t RX_LOS :1; // Bit1 Indicates Optical Loss of Signal (per relevant optical link standard).
					   //      Updated within 100msec of change on pin.

	uint8_t TX_Fault :1;   // Bit2 Digital state of the TX Fault Output Pin
						   //      Updated within 100msec of change on pin.

	uint8_t Soft_Rate_Select_0 :1; // Bit3 Read/write bit that allows software rate select control

	uint8_t Rate_Select_0 :1; // Bit4 Digital state of the SFP Rate_Select Input Pin.
							  //      Updated within 100msec of change on pin.

	uint8_t Rate_Select_1 :1;   // Bit5 Digital state of the Rate_Select 1 Pin.
								//      Updated within 100msec of change on pin.

	uint8_t Soft_TxDisable :1; // Bit6 Optional read/write bit that allows software disable of laser.
							   //      Writing '1' disables laser. Turn on/off time is 100 msec max from
							   //      acknowledgement of serial byte transmission. This bit is “OR”d with the hard
							   //      TX_DISABLE pin value. Note, per SFP MSA TX_DISABLE pin is default enabled unless
							   //      pulled low by hardware. If Soft TX Disable is not implemented, the transceiver ignores
							   //      the value of this bit. Default power up value is 0.

	uint8_t TxDisable :1;    // Bit7 Digital state of the TX Disable Input Pin.
							 //      Updated within 100msec of change on pin.

} G_CtrlStatus_110_b;


typedef union G_CtrlStatus_Bits_110_union
{
	G_CtrlStatus_110_b Bits;
	uint8_t            Byte;

} G_CtrlStatus_110_u;


// Byte111 General Control/Status Bits
typedef struct G_CtrlStatus_Bits_111_bits
{
	uint8_t Reserved :8;  // Bit0 - Bit7 Reserved

} G_CtrlStatus_111_b;

typedef union G_CtrlStatus_Bits_111_union
{
	G_CtrlStatus_111_b Bits;
	uint8_t Byte;

} G_CtrlStatus_111_u;

// Byte110-111 General Control/Status bits (2 Bytes)
typedef struct G_CtrlStatus_Bits_struct
{
	G_CtrlStatus_110_u CtrlStatus_110;
	G_CtrlStatus_111_u CtrlStatus_111;

} G_CtrlStatus_Bits_s;

// Byte112 Alarm and Warning Flag Bits
typedef struct Alarm_Warning_Flag_112_bits
{
	uint8_t TXPower_Low_Alarm  :1;   // Bit0 low TX Power alarm.
	uint8_t TXPower_High_Alarm :1;   // Bit1 High TX Power alarm.
	uint8_t TXBias_Low_Alarm   :1;   // Bit2 low TX Bias alarm.
	uint8_t TXBias_High_Alarm  :1;   // Bit3 High TX Bias alarm.

	uint8_t Voltage_Low_Alarm  :1;   // Bit4 low Voltage alarm.
	uint8_t Voltage_High_Alarm :1;   // Bit5 High Voltage alarm.

	uint8_t Temp_Low_Alarm     :1;   // Bit6 low Temp alarm.
	uint8_t Temp_High_Alarm    :1;   // Bit7 High Temp alarm.

} AlarmFlag_112_b;

typedef union Alarm_Warning_Flag_112_union
{
	AlarmFlag_112_b Bits;
	uint8_t Byte;

} AlarmFlag_112_u;


// Byte113 Alarm and Warning Flag Bits
typedef struct Alarm_Warning_Flag_113_bits
{
	uint8_t Reserved              :2;   // Bit0 - Bit1 Reserved

	uint8_t TECCurrent_Low_Alarm  :1;   // Bit2 low TECCurrent alarm.
	uint8_t TECCurrent_High_Alarm :1;   // Bit3 High TECCurrent alarm.

	uint8_t LaserTemp_Low_Alarm   :1;   // Bit4 low LaserTemp alarm.
	uint8_t LaserTemp_High_Alarm  :1;   // Bit5 High LaserTemp alarm.

	uint8_t RXPower_Low_Alarm     :1;   // Bit6 low RX Power alarm.
	uint8_t RXPower_High_Alarm    :1;   // Bit7 High RX Power alarm.

} AlarmFlag_113_b;


typedef union Alarm_Warning_Flag_113_union
{
	AlarmFlag_113_b Bits;
	uint8_t Byte;

} AlarmFlag_113_u;


// Byte114 Input equalization level control
typedef struct Input_EQ_Level_Control_114_bits
{
	uint8_t HighRate :4;   // Bit0 - Bit3 Tx input equalization control RATE=HIGH
	uint8_t LowRate  :4;   // Bit4 - Bit7 Tx input equalization control RATE=LOW

} TxInputEQ_114_b;


typedef union Input_EQ_Level_Control_114_union
{
	TxInputEQ_114_b Bits;
	uint8_t Byte;

} TxInputEQ_114_u;


// Byte115 Output emphasis level control
typedef struct Output_Emphasis_Level_Control_115_bits
{
	uint8_t HighRate :4;   // Bit0 - Bit3 RX output emphasis control RATE=HIGH
	uint8_t LowRate  :4;   // Bit4 - Bit7 RX output emphasis control RATE=LOW

} RxOutputEmphasis_115_b;

typedef union Output_Emphasis_Level_Control_115_union
{
	RxOutputEmphasis_115_b Bits;
	uint8_t Byte;

} RxOutputEmphasis_115_u;


// Byte116 Alarm and Warning Flag Bits
typedef struct Alarm_Warning_Flag_116_bits
{
	uint8_t TXPower_Low_Warning  :1;   // Bit0 low TX Power Warning.
	uint8_t TXPower_High_Warning :1;   // Bit1 High TX Power Warning.
	uint8_t TXBias_Low_Warning   :1;   // Bit2 low TX Bias Warning.
	uint8_t TXBias_High_Warning  :1;   // Bit3 High TX Bias Warning.

	uint8_t Voltage_Low_Warning  :1;   // Bit4 low Voltage Warning.
	uint8_t Voltage_High_Warning :1;   // Bit5 High Voltage Warning.

	uint8_t Temp_Low_Warning     :1;   // Bit6 low Temp Warning.
	uint8_t Temp_High_Warning    :1;   // Bit7 High Temp Warning.

} WarningFlag_116_b;


typedef union Alarm_Warning_Flag_116_union
{
	WarningFlag_116_b Bits;
	uint8_t Byte;

} WarningFlag_116_u;


// Byte117 Alarm and Warning Flag Bits
typedef struct Alarm_Warning_Flag_117_bits
{
	uint8_t Reserved                :2;   // Bit0 - Bit1 Reserved

	uint8_t TECCurrent_Low_Warning  :1;   // Bit2 low TECCurrent Warning.
	uint8_t TECCurrent_High_Warning :1;   // Bit3 High TECCurrent Warning.

	uint8_t LaserTemp_Low_Warning   :1;   // Bit4 low LaserTemp Warning.
	uint8_t LaserTemp_High_Warning  :1;   // Bit5 High LaserTemp Warning.

	uint8_t RXPower_Low_Warning     :1;   // Bit6 low RX Power Warning.
	uint8_t RXPower_High_Warning    :1;   // Bit7 High RX Power Warning.

} WarningFlag_117_b;

typedef union Alarm_Warning_Flag_117_union
{
	WarningFlag_117_b Bits;
	uint8_t           Byte;

} WarningFlag_117_u;

// Byte112-117 Alarm and Warning Flag Bits (6 Bytes)
typedef struct Alarm_Warning_Flag_struct
{
	AlarmFlag_112_u AlarmFlag_112;
	AlarmFlag_113_u AlarmFlag_113;

	TxInputEQ_114_u        TxInputEQ_114;
	RxOutputEmphasis_115_u RxOutputEmphasis_115;

	WarningFlag_116_u WarningFlag_116;
	WarningFlag_117_u WarningFlag_117;

} Alarm_Warning_Flag_s;

// Byte118 Extended Module Control/Status Bytes
typedef struct E_CtrlStatus_Bits_118_bits
{
	uint8_t PowerLevel_Select :1;   // Bit0 Optional.
									//      SFF-8431 Power Level (maximum power dissipation) control bit.
									//      Value of zero enables Power Level 1 only (1.0 Watt max).
									//      Value of one enables Power Level 2 (1.5 Watt max).
									//      If Power Level 2 is not implemented, the SFP ignores the value of this bit.

	uint8_t PowerLevel :1;   // Bit1 Optional.
							 //      SFF-8431 Power Level (maximum power dissipation) status.
							 //      Value of zero indicates Power Level 1 operation (1.0 Watt max).
							 //      Value of one indicates Power Level 2 operation (1.5 Watt max).

	uint8_t Reserved :1;   // Bit2 Reserved
						   //      Updated within 100msec of change on pin.

	uint8_t Soft_Rate_Select_1 :1; // Bit3 Read/write bit that allows software rate select control

	uint8_t Reserved_1 :4;   // Bit4 - Bit7 Reserved

} E_CtrlStatus_118_b;

typedef union E_CtrlStatus_Bits_118_union
{
	E_CtrlStatus_118_b Bits;
	uint8_t Byte;

} E_CtrlStatus_118_u;

// Byte119 Extended Module Control/Status Bytes
typedef struct E_CtrlStatus_Bits_119_bits
{
	uint8_t RxCDRUnlocked :1;   // Bit0 Optional Rx CDR unlocked
	uint8_t TxCDRUnlocked :1;   // Bit1 Optional Tx CDR unlocked

	uint8_t Reserved :6;       // Bit2 - Bit7 Reserved

} E_CtrlStatus_119_b;


typedef union E_CtrlStatus_Bits_119_union
{
	E_CtrlStatus_119_b Bits;
	uint8_t Byte;

} E_CtrlStatus_119_u;

// Byte118-119 Extended Module Control/Status bits (2 Bytes)
typedef struct E_CtrlStatus_Bits_struct
{
	E_CtrlStatus_118_u CtrlStatus_118;
	E_CtrlStatus_119_u CtrlStatus_119;

} E_CtrlStatus_Bits_s;


// Byte0 - 127  --- low 128 byte
struct LOWER_MEMORY_MAP_struct
{
	Threshold_Values_s   Threshold_Values;     // 0x00 - 0x37 Threshold Values used for Alarm and Warning Flags(56 Bytes)
	ExternalCal_Values_s ExternalCal_Values;   // 0x38 - 0x5E Calibration constants for External Calibration Option(39 Bytes)
	uint8_t              Checksum;             // 0x5F        the low order 8 bits of the sum of bytes 0 – 94
	AD_Readout_s         AD_Readout;           // 0x60 - 0x6D A/D readout (14 Bytes)
	G_CtrlStatus_Bits_s  G_CtrlStatus_Bits;    // 0x6E - 0x6F General Control/Status bits (2 Bytes)
	Alarm_Warning_Flag_s Alarm_Warning_Flag;   // 0x70 - 0x75 Alarm and Warning Flag Bits (6 Bytes)
	E_CtrlStatus_Bits_s  E_CtrlStatus_Bits;    // 0x76 - 0x77 Extended Module Control/Status Bytes(2 Bytes)
	uint8_t              Reserved[2];          // 0x78 - 0x79 Reserved
	uint8_t              SecurityLevel;        // 0x7A        SecurityLevel
	uint32_t             PasswordEntryArea;    // 0x7B - 0x7E Password Entry Area (optional) (4 Bytes)
	uint8_t              PageSelect;           // 0x7F        Page Select Byte
};


// defien for A0H
struct Table_Size_256_struct
{
	uint8_t table_data[256];
};

// define Table00
struct Table_Size_128_struct
{
	uint8_t table_data[128];
};

// System control byte
typedef struct Module_Define_Control_SYS_bits
{
	uint8_t Reserved                        :5;   // Bit0 - Bit4 Reserved
	uint8_t EEPROMSaveMode                  :1;   // Bit5
	uint8_t Temperature_Compensation_Enable :1;   // Bit6
	uint8_t SystemReset                     :1;   // Bit7

} ModuleDefCtrl_SYS_b;


typedef union Module_Define_Control_SYS_union
{
	ModuleDefCtrl_SYS_b Bits;
	uint8_t             Byte;

} ModuleDefCtrl_SYS_u;


// Tx Control Byte
typedef struct Module_Define_Control_TX_bits
{
	uint8_t Reserved         :4; // Bit0 - Bit3 Reserved

	uint8_t TEC_Enable       :1; // Bit4 TEC电路开关
	uint8_t ModLUT_Enable    :1; // Bit5 调制电流查找表开关
	uint8_t BiasLimit_Enable :1; // Bit6 偏置电流限流开关
	uint8_t SoftAPC_Enable   :1; // Bit7 软件APC环路开关

} ModuleDefCtrl_TX_b;


typedef union Module_Define_Control_TX_union
{
	ModuleDefCtrl_TX_b Bits;
	uint8_t            Byte;

} ModuleDefCtrl_TX_u;


// Rx Control Byte
typedef struct Module_Define_Control_RX_bits
{
	uint8_t Reserved         :7;   // Bit0 - Bit6 Reserved
	uint8_t RXSquelch_Enable :1;   // Bit7

} ModuleDefCtrl_RX_b;


typedef union Module_Define_Control_RX_union
{
	ModuleDefCtrl_RX_b Bits;
	uint8_t            Byte;

} ModuleDefCtrl_RX_u;

// ---------------------------- T80 ------------------------------------
struct Table_General_Control_struct
{
	uint8_t             SN[8];            // 80h - 87h  用于存放产品的内部控制序列号
	uint32_t            PassWord_1;       // 88h - 8Bh  安全等级一密码
	uint32_t            PassWord_2;       // 8Ch - 8Fh  安全等级二密码
	uint8_t             EEPROMVersion[4]; // 90h - 93h  EEPROM版本号

	ModuleDefCtrl_SYS_u ModuleDefCtrl_SYS; // 94h 系统控制寄存器
	ModuleDefCtrl_TX_u  ModuleDefCtrl_TX;  // 95h 发射端相关控制寄存器
	ModuleDefCtrl_RX_u  ModuleDefCtrl_RX;  // 96h 接收端相关控制寄存器

	float               V_Reference;       // 97h - 9Ah 参考电压
};


// 用于定义发射端校正控制寄存器
struct Table_TX_Cal_Ctrl_struct
{
	float TXPowerCal_Slope;     // 80h - 83h 发射光功率校正增益
	float TXPowerCal_Offset;    // 84h - 87h 发射光功率校正偏移

	uint16_t  SoftAPC_Target;   // 88h - 89h 软件APC设置目标值
	uint16_t  Bias_Max;         // 8Ah - 8Bh 偏置电流限制值ADC

	uint16_t BiasSet;  			// 8Ch - 8Dh
	uint16_t ModSet;   			// 8Eh - 8Fh
};

// 用于定义接收端校正控制寄存器
struct Table_RX_Cal_Ctrl_struct
{
	float RxPowerCal_Slope_4; // 80h - 83h 接收光功率校正增益4
	float RxPowerCal_Slope_3; // 84h - 87h 接收光功率校正增益3
	float RxPowerCal_Slope_2; // 88h - 8Bh 接收光功率校正增益2
	float RxPowerCal_Slope_1; // 8Ch - 8Fh 接收光功率校正增益1
	float RxPowerCal_Offset;  // 90h - 93h 接收光功率校正偏移

	uint16_t RxADC_DarkDown;  // 94h - 95h 接收光功率校正无光点ADC下限值
	uint16_t RxADC_DarkUp;    // 96h - 97h 接收光功率校正无光点ADC上限值

	uint16_t RxLosAssert;     // 98h - 99h 接收光功率LOS 设置寄存器1
	uint16_t RxLosDeAssert;   // 9Ah - 9Bh 接收光功率LOS 设置寄存器2
};

// 用于定义温度补偿
struct Table_Temperature_Compensation_struct
{
	float Offset_Sensor;     // 80h - 83h 对传感器测试原始温度进行校正的偏移量
	float Slope_Sensor;      // 84h - 87h 对传感器测试原始温度进行校正的偏移量

	int8_t Offset_Chip_PCB;  // 88h : 芯片采样与PCB板温度差值
	int8_t Offset_PCB_Shell; // 89h : PCB板与外壳温度差值
	int8_t Offset_Display;   // 8Ah : 动态补偿显示值
};

// 用于定义调制电流校正系数
struct Table_ModSet_Calibration_struct
{
	int8_t ChipTemperature_Normal;  // 80h 常温芯片温度值（℃）

	float ModSet_Slope_L;  // 81h - 84h 低温和常温区间的调制电流计算增益
	float ModSet_Offset_L; // 85h - 88h 低温和常温区间的调制电流计算偏移
	float ModSet_Slope_H;  // 89h - 8Ch 常温和高温区间的调制电流计算增益
	float ModSet_Offset_H; // 8Dh - 90h 常温和高温区间的调制电流计算偏移
};

// 用于定义监视采样寄存器（SRAM）
struct Table_Monitor_struct
{
	uint8_t  FWVersion[4];       // 80h - 83h Firmware 版本号
	uint16_t TemperatureADC;     // 84h - 85h 温度采样ADC
	uint16_t VoltageADC;         // 86h - 87h 电压采样ADC
	uint16_t TxBiasADC;          // 88h - 89h 偏置电流采样ADC
	uint16_t TxPowerADC;         // 8Ah - 8Bh 发射光功率采样ADC
	uint16_t RxPowerADC;     	 // 8Ch - 8Dh 接收光功率采样ADC
	uint16_t ChipTemperatureDOM; // 8Eh - 8Eh
};

// 用于定义芯片调试（SRAM）
struct Table_Device_Debug_struct
{
	uint16_t Device_Addr;   // 80h - 81h ：芯片  地址
	uint16_t Device_WrAddr; // 82h - 83h ：写 寄存器 地址
	uint16_t Device_WrData; // 84h - 85h ：写 寄存器 数据
	uint16_t Device_RdAddr; // 86h - 87h ：读 寄存器 地址
	uint16_t Device_RdData; // 88h - 89h ：读 寄存器 数据
};

// 用于定义系统调试（SRAM）
struct Table_System_Debug_struct
{
	uint8_t SaveTableNum;  // 80h ：保存表名称
};

#endif
