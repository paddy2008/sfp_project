/*
 * msa_page0.c
 *
 *  Created on: 2017-10-18
 *      Author: cuilin
 */
#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"

static QSFP_MSA_PAGE_T *_page0 = NULL;

static const uint8_t _msa_writable_mask[MSA_PAGE_MASK_LEN] =
{
	//1-- Write, 0--Not Write(Only Read), bit0 replace low addr, bit7 replace high addr
	0x00, 0x00, //byte128  ~ byte135, byte136 ~ byte143
	0x00, 0x00,	//byte144  ~ byte151, byte152 ~ byte159
	0x00, 0x00,	//byte160  ~ byte167, byte168 ~ byte175
	0x00, 0x00, //byte176  ~ byte183, byte184 ~ byte191
	0x00, 0x00, //byte192  ~ byte199, byte200 ~ byte207
	0x00, 0x00, //byte208  ~ byte231, byte232 ~ byte223
	0x00, 0x00, //byte224  ~ byte247, byte248 ~ byte239
	0x00, 0x00, //byte240  ~ byte263, byte264 ~ byte255
};

#if defined(MSA_NORMAL_MODE)
static const uint8_t  _eeprom_init_data[MSA_PAGE_SIZE] =
{
	0x11, //0x80

#if defined(EFM8LB12E64_100G_SFP_AOC_101) || defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0xcc, //0x81
#endif
#if defined(EFM8LB12E64_100G_SFP_LR4_301)
	0xce, //0x81
#endif

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	0x0c, //0x82
#endif
#if defined(EFM8LB12E64_100G_SFP_LR4_301) || defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0x07, //0x82
#endif

	0x80,0x00,0x00,0x00,0x00,   	//0x83 ~ 0x87
	0x00,0x00,0x00,0x05,0xff,0x00,  //0x88 ~ 0x8D

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	0x00, //0x8E
	0x25, //0x8F
#endif
#if defined(EFM8LB12E64_100G_SFP_LR4_301)
	0x0a, //0x8E
	0x00, //0x8F
#endif

#if defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0x02, //0x8E
	0x00, //0x8F
#endif

	0x00,0x00,//0x90 ~ 0x91

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	0x32,  //0x92
	0x00,  //0x93
#endif

#if defined(EFM8LB12E64_100G_SFP_LR4_301)
	0x00,  //0x92
	0x44,  //0x93
#endif

#if  defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0x00,  //0x92
	0x40,  //0x93
#endif

	0x43,0x49,0x47,0x20,   //0x94 ~ 0x97
	0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,   //0x98 ~ 0x9F
	0x20,0x20,0x20,0x20,0x00,0x00,0x00,0x00,   //0xA0 ~ 0xA7
	0x41,0x43,0x44,0x36,0x36,0x31,0x4d,0x4f,   //0xA8 ~ 0xAF
	0x31,0x48,0x4d,0x4d,0x42,0x30,0x20,0x20,   //0xB0 ~ 0xB7
	0x30,0x31,//0xB8 ~ 0xB9

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	0x42, //0xBA
	0x68, //0xBB
	0x07, //0xBC
#endif
#if defined(EFM8LB12E64_100G_SFP_LR4_301) || defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0x66, //0xBA
	0x58, //0xBB
#endif

#if defined(EFM8LB12E64_100G_SFP_LR4_301)
	0x00, //0xBC
#endif

#if defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0x05, //0xBC
#endif

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	0xd0, //0xBD
#endif
#if defined(EFM8LB12E64_100G_SFP_LR4_301)
	0xd1, //0xBD
#endif

#if defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0x14, //0xBD
#endif

	0x00,0x1b,   //0xBE ~ 0xBF

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	0x01,  //0xC0
	0x0f,  //0xC1
#endif

#if defined(EFM8LB12E64_100G_SFP_LR4_301)
	0x03, //0xC0
	0x07, //0xC1
#endif

#if defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0x06, //0xC0
	0x07, //0xC1
#endif

	0xff,//0xC2

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	0xda,//0xC3
#endif
#if defined(EFM8LB12E64_100G_SFP_LR4_301) || defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0x9e, //0xC3
#endif

	0x41,0x41,0x4d,0x32,   //0xC4 ~ 0xC7
	0x42,0x42,0x31,0x37,0x34,0x33,0x31,0x31,   //0xC8 ~ 0xCF
	0x31,0x31,0x20,0x20,0x31,0x37,0x31,0x30,   //0xD0 ~ 0xD7
	0x31,0x39,0x20,0x20,0x0c,0x00,0x68,0xe9,   //0xD8 ~ 0xDF
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //0xE0 ~ 0xE7
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //0xE8 ~ 0xEF
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //0xF0 ~ 0xF7
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //0xF8 ~ 0xFF
};

#elif defined(SKYLANE_MODE)
static const uint8_t  _eeprom_init_data[MSA_PAGE_SIZE] =
{
	0x11, //0x80

#if defined(EFM8LB12E64_100G_SFP_AOC_101) || defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0xcc, //0x81
#endif
#if defined(EFM8LB12E64_100G_SFP_LR4_301)
	0xce, //0x81
#endif

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	0x0c, //0x82
#endif
#if defined(EFM8LB12E64_100G_SFP_LR4_301) || defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0x07, //0x82
#endif

	0x80,0x00,0x00,0x00,0x00,   	//0x83 ~ 0x87
	0x00,0x00,0x00,0x05,0xff,0x00,  //0x88 ~ 0x8D

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	0x00, //0x8E
	0x25, //0x8F
#endif
#if defined(EFM8LB12E64_100G_SFP_LR4_301)
	0x0a, //0x8E
	0x00, //0x8F
#endif

#if defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0x02, //0x8E
	0x00, //0x8F
#endif

	0x00,0x00,//0x90 ~ 0x91

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	0x32,  //0x92
	0x00,  //0x93
#endif

#if defined(EFM8LB12E64_100G_SFP_LR4_301)
	0x00,  //0x92
	0x44,  //0x93
#endif

#if  defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0x00,  //0x92
	0x40,  //0x93
#endif

	0x53,0x6B,0x79,0x6C,   //0x94 ~ 0x97
	0x61,0x6E,0x65,0x20,0x4F,0x70,0x74,0x69,   //0x98 ~ 0x9F
	0x63,0x73,0x20,0x20,0x00,0x00,0x25,0xCD,   //0xA0 ~ 0xA7
	0x51,0x32,0x38,0x51,0x4C,0x30,0x30,0x32,   //0xA8 ~ 0xAF
	0x43,0x30,0x30,0x46,0x30,0x30,0x30,0x20,   //0xB0 ~ 0xB7
	0x30,0x31,//0xB8 ~ 0xB9

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	0x42, //0xBA
	0x68, //0xBB
	0x07, //0xBC
#endif
#if defined(EFM8LB12E64_100G_SFP_LR4_301) || defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0x66, //0xBA
	0x58, //0xBB
#endif

#if defined(EFM8LB12E64_100G_SFP_LR4_301)
	0x00, //0xBC
#endif

#if defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0x05, //0xBC
#endif

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	0xd0, //0xBD
#endif
#if defined(EFM8LB12E64_100G_SFP_LR4_301)
	0xd1, //0xBD
#endif

#if defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0x14, //0xBD
#endif

	0x00,0x1b,   //0xBE ~ 0xBF

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	0x01,  //0xC0
	0x0f,  //0xC1
#endif

#if defined(EFM8LB12E64_100G_SFP_LR4_301)
	0x03, //0xC0
	0x07, //0xC1
#endif

#if defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0x06, //0xC0
	0x07, //0xC1
#endif

	0xff,//0xC2

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
	0xda,//0xC3
#endif
#if defined(EFM8LB12E64_100G_SFP_LR4_301) || defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	0xde, //0xC3
#endif

	0x32,0x41,0x35,0x45,   //0xC4 ~ 0xC7
	0x30,0x31,0x30,0x30,0x30,0x31,0x20,0x20,   //0xC8 ~ 0xCF
	0x20,0x20,0x20,0x20,0x31,0x37,0x31,0x30,   //0xD0 ~ 0xD7
	0x31,0x39,0x20,0x20,0x0c,0x04,0x68,0xe9,   //0xD8 ~ 0xDF
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //0xE0 ~ 0xE7
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //0xE8 ~ 0xEF
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //0xF0 ~ 0xF7
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //0xF8 ~ 0xFF
};

#endif

uint8_t MSA_p0_read_by_isr(uint8_t offset)
{
	return FLASH_ByteRead_by_i2c_slave_isr(MSA_FLASH_ADDR_PAGE_00H + OFFSET_INDEX(offset));
}


uint8_t MSA_p0_write_by_isr(uint8_t offset, uint8_t value)
{
	if(MSA_lower_page_get_passwd_status())  //Vendor mode or OEM mode
	{
		MSA_write_eep(offset, value);
	}
	else  //Customer mode
	{
		//not access to write
	}
	return GOS_OK;
}

void MSA_p0_eeprom_init(void)
{
	flash_write_multi_byte((FLADDR)MSA_FLASH_ADDR_PAGE_00H, (uint8_t *)_eeprom_init_data, sizeof(_eeprom_init_data));
}


static void _msa_p0_update_checksum()
{
	uint8_t check_code_base = 0, check_code_ext = 0, check_base_target = 0,check_ext_target = 0;
	uint8_t base_len = MSA_P0_CHECK_CODE_BASE_END - MSA_P0_CHECK_CODE_BASE_START + 1;
	uint8_t end_len  = MSA_P0_CHECK_CODE_EXT_END - MSA_P0_CHECK_CODE_EXT_START + 1;
	uint8_t base_buff[63] = {0};
	uint8_t end_buff[31] = {0};
	QSFP_MSA_PAGE_T 			*lower_page = MSA_get_page_ptr(MSA_PAGE_LOWER);
	MSA_PAGE_LOWER_ISR_FLAG_T   *isr_flag = (MSA_PAGE_LOWER_ISR_FLAG_T *)&(lower_page->ptr[MSA_OFFSET_LOWER_LOS]);

	check_base_target = FLASH_ByteRead((FLADDR)(MSA_FLASH_ADDR_PAGE_00H + OFFSET_INDEX(MSA_P0_CHECK_CODE_BASE)));
	check_ext_target  = FLASH_ByteRead((FLADDR)(MSA_FLASH_ADDR_PAGE_00H + OFFSET_INDEX(MSA_P0_CHECK_CODE_EXT)));

	FLASH_Read((uint8_t *)base_buff, MSA_FLASH_ADDR_PAGE_00H + OFFSET_INDEX(MSA_P0_CHECK_CODE_BASE_START), base_len);
	FLASH_Read((uint8_t *)end_buff,  MSA_FLASH_ADDR_PAGE_00H + OFFSET_INDEX(MSA_P0_CHECK_CODE_EXT_START), end_len);

	check_code_base = MSA_check_code(base_buff, base_len);
	check_code_ext  = MSA_check_code(end_buff, end_len);


	if((check_code_base != check_base_target) || (check_code_ext != check_code_ext))
	{
		isr_flag->tx_fault |= 0xF;  //if checksum are error, report tx_fault
	}
}

void MSA_p0_init(QSFP_MSA_PAGE_T *page)
{
	_page0 = page;

	_page0->ptr        	  			= NULL;
	_page0->len           			= MSA_PAGE_SIZE;
	_page0->page_flash_base 		= MSA_FLASH_ADDR_PAGE_00H;
	_page0->eep_mask				= NULL;
	_page0->msa_writable_mask   	= &_msa_writable_mask[0];
	_page0->vendor_writable_mask    = NULL;

	_msa_p0_update_checksum();
}
