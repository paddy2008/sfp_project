/*
 * msa_page3.c
 *
 *  Created on: 2017-11-7
 *      Author: cuilin
 */

#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_i2c_slave.h"
#include "Efm8lb12_smbus.h"
#include "Efm8lb12_100g_com_drv.h"

#ifdef MSA_PAGE_3

static QSFP_MSA_PAGE_T *_pg3 = NULL;

static volatile uint8_t _data_ram[MSA_PAGE_SIZE] = {0};

static void _msa_p3_update_control_drv_mask(uint8_t offset);

static const uint8_t  _msa_writable_mask[MSA_PAGE_MASK_LEN] =
{
	//1-- Write, 0--Not Write(Only Read), bit0 replace low addr, bit7 replace high addr
	0x00, 0x00, //byte128  ~ byte135, byte136 ~ byte143
	0x00, 0x00,	//byte144  ~ byte151, byte152 ~ byte159
	0x00, 0x00,	//byte160  ~ byte167, byte168 ~ byte175
	0x00, 0x00, //byte176  ~ byte183, byte184 ~ byte191
	0x00, 0x00, //byte192  ~ byte199, byte200 ~ byte207
	0x00, 0x00, //byte208  ~ byte215, byte216 ~ byte223
	0xFC, 0xFF, //byte224  ~ byte231, byte232 ~ byte239
	0xFF, 0xFF, //byte240  ~ byte247, byte248 ~ byte255
};

static const uint8_t _vendor_writable_mask[MSA_PAGE_MASK_LEN] =
{
	//1-- Write, 0--Not Write(Only Read), bit0 replace low addr, bit7 replace high addr
	0xFF, 0xFF, //byte0   ~ byte7, 	byte8   ~ byte15
	0xFF, 0xFF,	//byte16  ~ byte23, byte24  ~ byte31
	0xFF, 0xFF,	//byte32  ~ byte39, byte40  ~ byte47
	0xFF, 0xFF, //byte48  ~ byte55, byte56  ~ byte63
	0xFF, 0xFF, //byte64  ~ byte71, byte72  ~ byte79
	0xFF, 0xFF, //byte80  ~ byte87, byte88  ~ byte95
	0xFF, 0xFF, //byte96  ~ byte103,byte104 ~ byte111
	0xFF, 0xFF, //byte112 ~ byte119,byte120 ~ byte127
};


static const uint8_t _eep_mask[MSA_PAGE_MASK_LEN] =
{
	//1-- Write, 0--Not Write(Only Read), bit0 replace low addr, bit7 replace high addr
	0xFF, 0xFF, //byte128  ~ byte135, byte136 ~ byte143
	0xFF, 0xFF,	//byte144  ~ byte151, byte152 ~ byte159
	0xFF, 0xFF,	//byte160  ~ byte167, byte168 ~ byte175
	0xFF, 0xFF, //byte176  ~ byte183, byte184 ~ byte191
	0xFF, 0xFF, //byte192  ~ byte199, byte200 ~ byte207
	0xFF, 0xFF, //byte208  ~ byte215, byte216 ~ byte223
	0xFF, 0xFF, //byte224  ~ byte231, byte232 ~ byte239
	0x03, 0xFF, //byte240  ~ byte247, byte248 ~ byte255
};

static const uint8_t  _eeprom_init_data[MSA_PAGE_SIZE] =
{
#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	 0x50,   //0x80
#endif
#if defined(EFM8LB12E64_100G_SFP_LR4_301) || defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	 0x4b,  //0x80
#endif

	 0x00,  //0x81

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	 0xf6,  //0x82
#endif

#if defined(EFM8LB12E64_100G_SFP_LR4_301) || defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	 0xfb,  //0x82
#endif

	 0x00,  //0x83

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	 0x4b,  //0x84
#endif

#if defined(EFM8LB12E64_100G_SFP_LR4_301) || defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206)
	 0x46,  //0x84
#endif

#if defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	 0x49,  //0x84
#endif

	 0x00,  //0x85

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	 0xfb,  //0x86
#endif

#if defined(EFM8LB12E64_100G_SFP_LR4_301) || defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206)
	 0x00,  //0x86
#endif

#if defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	 0xfd,  //0x86
#endif

	 0x00,  //0x87
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //0x88 ~ 0x8F
	 0x8d, //0x90
	 0xcc, //0x91

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	 0x74, //0x92
	 0x04, //0x93
#endif

#if defined(EFM8LB12E64_100G_SFP_LR4_301) || defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	 0x76,  //0x92
	 0x98,  //0x93
#endif

	 0x87,0x5a,0x7a,0x76,   //0x94 ~ 0x97
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //0x98 ~ 0x9F
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //0xA0 ~ 0xA7
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //0xA8 ~ 0xAF

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	 0x62,0x1f,0x00,0x9e,0x3d,0xe9,0x01,0x8e,   //0xB0 ~ 0xB7
	 0x17,0x70,0x00,0xfa,0x13,0x88,0x03,0xe8,   //0xB8 ~ 0xBF
	 0x62,0x1f,0x03,0x67,0x43,0xe2,0x06,0xca,   //0xC0 ~ 0xC7
#endif

#if defined(EFM8LB12E64_100G_SFP_LR4_301)
	 0x8a,0x99,0x01,0xb4,0x6e,0x17,0x03,0x66,   //0xB0 ~ 0xB7
	 0xc3,0x50,0x30,0xd4,0xaf,0xc8,0x44,0x5c,   //0xB8 ~ 0xBF
	 0x8a,0x99,0x07,0x46,0x6e,0x17,0x0e,0x83,   //0xC0 ~ 0xC7
#endif

#if defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206)
	 0x3d,0xe9,0x03,0x1a,0x31,0x2d,0x03,0xe8,  //0xB0 ~ 0xB7
	 0x88,0xb8,0x05,0xdc,0x75,0x30,0x09,0xc4,  //0xB8 ~ 0xBF
	 0x7b,0x84,0x0c,0x5a,0x62,0x1e,0x0f,0x8d,  //0xC0 ~ 0xC7
#endif

#if  defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	 0x62,0x1f,0x00,0xb2,0x45,0x76,0x01,0x63,  //0xB0 ~ 0xB7
	 0xaf,0xc8,0x09,0xc4,0x9c,0x40,0x1d,0x4c,  //0xB8 ~ 0xBF
	 0x62,0x1f,0x05,0xc7,0x45,0x76,0x08,0xbf,  //0xC0 ~ 0xC7
#endif

	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //0xC8 ~ 0xCF
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //0xD0 ~ 0xD7
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //0xD8 ~ 0xDF
	 0xa7,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,   //0xE0 ~ 0xE7
	 0x00,0x00,  //0xE8,0xE9

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	 0x55,  //0xEA
	 0x55,   //0xEB
#endif

#if defined(EFM8LB12E64_100G_SFP_LR4_301) || defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206)
	 0x11,  //0xEA
	 0x11,  //0xEB
#endif

#if defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	 0x77,  //0xEA
	 0x77,  //0xEB
#endif

	 0x00,0x00,0x22,0x22,   //0xEC ~ 0xEF
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //0xF0 ~ 0xF7
	 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //0xF8 ~ 0xFF
};

uint8_t MSA_p3_read_by_isr(uint8_t offset)
{
	return _pg3->ptr[OFFSET_INDEX(offset)];
}

uint8_t MSA_p3_write_by_isr(uint8_t offset, uint8_t value)
{
	if(MSA_lower_page_get_passwd_status())  //Vendor mode or OEM mode
	{
		if (get_bit_by_isr(_pg3->vendor_writable_mask, OFFSET_INDEX(offset)))  //write access
		{
			_pg3->ptr[OFFSET_INDEX(offset)] = value;
			_msa_p3_update_control_drv_mask(OFFSET_INDEX(offset));

			if (get_bit_by_isr(_pg3->eep_mask, OFFSET_INDEX(offset)))
			{
				MSA_write_eep(offset, value);
			}
		}
	}
	else  //Customer mode
	{
		if (get_bit_by_isr(_pg3->msa_writable_mask, OFFSET_INDEX(offset)))
		{
			_pg3->ptr[OFFSET_INDEX(offset)] = value;
			_msa_p3_update_control_drv_mask(OFFSET_INDEX(offset));
		}
	}

	return GOS_OK;
}

static void _msa_p3_update_control_drv_mask(uint8_t offset)
{
	if (get_bit_by_isr(_pg3->msa_writable_mask, OFFSET_INDEX(offset)))
	{
		uint16_t drv_mask = MSA_get_control_drv_mask();

		if(OFFSET_INDEX(offset) == OFFSET_INDEX(MSA_OFFSET_PAGE_03H_TX1_TX2_INPUT_EQUAL_CTL))
		{
			drv_mask |= TX1_TX2_EQ;
		}
		else if(OFFSET_INDEX(offset) == OFFSET_INDEX(MSA_OFFSET_PAGE_03H_TX3_TX4_INPUT_EQUAL_CTL))
		{
			drv_mask |= TX3_TX4_EQ;
		}
		else if(OFFSET_INDEX(offset) == OFFSET_INDEX(MSA_OFFSET_PAGE_03H_RX1_RX2_OUTPUT_EQUAL_CTL))
		{
			drv_mask |= RX1_RX2_EMPHASIS;
		}
		else if(OFFSET_INDEX(offset) == OFFSET_INDEX(MSA_OFFSET_PAGE_03H_RX3_RX4_OUTPUT_EQUAL_CTL))
		{
			drv_mask |= RX3_RX4_EMPHASIS;
		}
		else if(OFFSET_INDEX(offset) == OFFSET_INDEX(MSA_OFFSET_PAGE_03H_RX1_RX2_OUTPUT_AMPL_CTL))
		{
			drv_mask |= RX1_RX2_AMPLITUDE;
		}
		else if(OFFSET_INDEX(offset) == OFFSET_INDEX(MSA_OFFSET_PAGE_03H_RX3_RX4_OUTPUT_AMPL_CTL))
		{
			drv_mask |= RX3_RX4_AMPLITUDE;
		}
		else if(OFFSET_INDEX(offset) == OFFSET_INDEX(MSA_OFFSET_PAGE_03H_RX_TX_SQ))
		{
			drv_mask |= TX_RX_SQ_DISABLE;
		}
		else if(OFFSET_INDEX(offset) == OFFSET_INDEX(MSA_OFFSET_PAGE_03H_RX_OUTPUT_DISABLE))
		{
			drv_mask |= RX_OUTPUT_TX_ADAPT_EQ;
		}
		MSA_set_control_drv_mask(drv_mask);
	}
}

void MSA_p3_eeprom_init(void)
{
	flash_write_multi_byte(MSA_FLASH_ADDR_PAGE_03H, (uint8_t *)_eeprom_init_data, sizeof(_eeprom_init_data));
}

MSA_PAGE3_DDMI_THRESHOLD_T *MSA_p3_get_ddmi_threshold()
{
	if(ASSERT(_pg3))
	{
		return NULL;
	}
	return (MSA_PAGE3_DDMI_THRESHOLD_T *)&(_pg3->ptr[OFFSET_INDEX(MSA_OFFSET_PAGE_03H_TEMP_HIGH_ALARM_MSB)]);
}

MSA_PAGE3_MASK_T *MSA_p3_get_mask()
{
	if(ASSERT(_pg3))
	{
		return NULL;
	}
	return (MSA_PAGE3_MASK_T *)&(_pg3->ptr[OFFSET_INDEX(MSA_OFFSET_PAGE_03H_RX1_RX2_PWR_MASK)]);
}

void MSA_p3_load_to_ram()
{
	FLASH_Read((uint8_t *)&_data_ram[0], MSA_FLASH_ADDR_PAGE_03H, MSA_PAGE_SIZE);
}

void MSA_p3_init(QSFP_MSA_PAGE_T *page)
{
	_pg3 				  		= page;
	_pg3->ptr        	  		= &_data_ram[0];
	_pg3->len             		= MSA_PAGE_SIZE;
	_pg3->page_flash_base 		= MSA_FLASH_ADDR_PAGE_03H;
	_pg3->eep_mask				= &_eep_mask[0];
	_pg3->msa_writable_mask   	= &_msa_writable_mask[0];
	_pg3->vendor_writable_mask  = &_vendor_writable_mask[0];

	MSA_p3_load_to_ram();
}
#endif
