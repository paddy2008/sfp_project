
#ifndef MALD_37645B_DATA_STRUCT_DEF_H_
#define MALD_37645B_DATA_STRUCT_DEF_H_

struct Table_Device_MALD_37645B_struct
{
	uint8_t CHIPID; 				   // 0x00
	uint8_t REVID;                     // 0x01
	uint8_t Reset;                     // 0x02
	uint8_t Misc_IO_Control;           // 0x03
	uint8_t CDRctrl;                   // 0x04
	uint8_t I2C_address_mode;          // 0x05
	uint8_t Channel_mode;              // 0x06
	uint8_t Lockphase;                 // 0x07

	uint8_t Reserve_08H_0AH[3];        // 0x08-0x0A

	uint8_t LOS_LOL_Tx_Fault_Status;   // 0x0B
	uint8_t LOS_LOL_Tx_Alarm;          // 0x0C
	uint8_t IgnoreTxFault;             // 0x0D

	uint8_t Reserve_0EH_0FH[2];        // 0x0E-0x0F

	uint8_t LOS_THRSH_AUTO_SQUELCH;    // 0x10

	uint8_t Reserve_11H;               // 0x11

	uint8_t CTLE_X;                    // 0x12

	uint8_t Reserve_13H_1FH[13];       // 0x13-0x1F

	uint8_t OUTPUT_MUTE_SLEW;          // 0x20
	uint8_t Ibias;                     // 0x21
	uint8_t Imod;                      // 0x22
	uint8_t PreFall;                   // 0x23
	uint8_t TDE;                       // 0x24
	uint8_t CrossingAdj;               // 0x25

	uint8_t Reserve_26H;               // 0x26

	uint8_t Iburnin;                   // 0x27
	uint8_t Burnin_enable;             // 0x28

	uint8_t Reserve_29H_5FH[55];       // 0x29-0x5F

	uint8_t ADC_config0;               // 0x60
	uint8_t ADC_config2;               // 0x61

	uint8_t Reserve_62H_64H[3];        // 0x62-0x64

	uint8_t ADC_out0_msbs;             // 0x65
	uint8_t ADC_out0_lsbs;             // 0x66
	uint8_t ADC_Tx_select;             // 0x67

};

#endif
