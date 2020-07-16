#ifndef MATA_37644B_DATA_STRUCT_DEF_H_
#define MATA_37644B_DATA_STRUCT_DEF_H_

struct Table_Device_MATA_37644B_struct
{
	uint8_t CHIPID;			      // 0x00
	uint8_t REVID;                // 0x01
	uint8_t Reset;				  // 0x02
	uint8_t MONITORS;             // 0x03
	uint8_t CDRctrl;              // 0x04
	uint8_t I2C_address_mode;     // 0x05
	uint8_t Channel_mode;         // 0x06
	uint8_t Lockphase;            // 0x07

	uint8_t Reserve_08H_09H[2];   // 0x08-0x09

	uint8_t LOS_mode;             // 0x0A
	uint8_t LOS_LOL_Status;       // 0x0B
	uint8_t LOS_LOL_alarm;        // 0x0C

	uint8_t Reserve_0DH_0FH[3];   // 0x0D-0x0F

	uint8_t LOS_CTRL;             // 0x10
	uint8_t SLA;                  // 0x11
	uint8_t TIA_CTRL;             // 0x12

	uint8_t Reserve_13H_1FH[13];  // 0x13-0x1F

	uint8_t OUTPUT_CTRL;          // 0x20
	uint8_t OUTPUT_SWING;         // 0x21
	uint8_t OUTPUT_DEEMPH;        // 0x22

	uint8_t Reserve_23H_5FH[61];  // 0x23-0x5F

	uint8_t ADC_config0;		  // 0x60
	uint8_t ADC_config2;		  // 0x61

	uint8_t Reserve_62H_64H[3];   // 0x62-0x64

	uint8_t ADC_out0_msbs;        // 0x65
	uint8_t ADC_out0_lsbs;        // 0x66
};

#endif
