/*
 * msa_page9.c
 *
 *  Created on: 2017-11-16
 *      Author: cuilin
 */


#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_i2c_slave.h"
#include "Efm8lb12_smbus.h"
#include "Efm8lb12_100g_com_drv.h"

#ifdef MSA_PAGE_10

static QSFP_MSA_PAGE_T *_pg10 = NULL;

uint8_t MSA_p10_read_by_isr(uint8_t offset)
{
	uint8_t value = 0;

	if(MSA_lower_page_get_passwd_status() & FACTORY_PWD_MATCH_OK) //Vendor Mode:enter factory passward
	{
		value =  FLASH_ByteRead_by_i2c_slave_isr(MSA_FLASH_ADDR_PAGE_0AH + OFFSET_INDEX(offset));
	}
	return value;
}

uint8_t MSA_p10_write_by_isr(uint8_t offset, uint8_t value)
{
	if(MSA_lower_page_get_passwd_status() & FACTORY_PWD_MATCH_OK) //Vendor Mode:enter factory passward
	{
		MSA_write_eep(offset, value);
		return GOS_OK;
	}
	return GOS_FAIL;
}

static bool MSA_p10_regdata_set_pre_handle(MSA_REG_SET_T *w_reg, uint8_t len)
{
	MSA_REG_SET_T r_reg;
	uint8_t i = 0 , msa_reg_size = 0;

	msa_reg_size = sizeof(MSA_REG_SET_T);

	for(i = 0; i < len / msa_reg_size; i++)
	{
		memset(&r_reg, 0, msa_reg_size);
		FLASH_Read((uint8_t *)&r_reg, (FLADDR)_pg10->page_flash_base + 4 * i, msa_reg_size);

		if(!memcmp(&r_reg, w_reg, msa_reg_size))
		{
			return false;
		}
		else if(r_reg.dev_addr == w_reg->dev_addr && r_reg.offset == w_reg->offset)
		{
			flash_write_multi_byte((FLADDR)_pg10->page_flash_base + 4 * i, (uint8_t *)w_reg, msa_reg_size);
			return false;
		}
	}

	if(i == len / msa_reg_size)
	{
		return true;
	}
	return true;
}

void MSA_p10_regdata_set(MSA_REG_SET_T *reg)
{
	uint8_t len = 0, msa_reg_size = 0;

	msa_reg_size = sizeof(MSA_REG_SET_T);

	len = FLASH_ByteRead(MSA_FLASH_ADDR_PAGE_05H + OFFSET_INDEX(MSA_P5_INIT_EEP_SIZE));

	if ((len + msa_reg_size) > _pg10->len)
		return;

	if(MSA_p10_regdata_set_pre_handle(reg, len) == false)
	{
		return;
	}

	flash_write_multi_byte((FLADDR)(_pg10->page_flash_base + len), (uint8_t *)reg, msa_reg_size);
	len += msa_reg_size;

	flash_write_byte((FLADDR)(MSA_FLASH_ADDR_PAGE_05H + OFFSET_INDEX(MSA_P5_INIT_EEP_SIZE)), len);
}

static void MSA_p10_eep_load_to_reg()
{
	uint8_t i = 0, len = 0, msa_reg_size = 0;
	MSA_REG_SET_T r_reg;

	msa_reg_size = sizeof(MSA_REG_SET_T);

	len = FLASH_ByteRead(MSA_FLASH_ADDR_PAGE_05H + OFFSET_INDEX(MSA_P5_INIT_EEP_SIZE));

	for(i = 0; i < len/msa_reg_size; i++)
	{
		memset(&r_reg, 0, msa_reg_size);
		FLASH_Read((uint8_t *)&r_reg, (FLADDR)(_pg10->page_flash_base + msa_reg_size * i), msa_reg_size);

#ifdef EFM8LB12E64_100G_SFP_LR4_301
		if(r_reg.dev_addr == LR4_TOSA_BASE_ADDR)
		{
			uint8_t page = U16_MSB(r_reg.offset);
			SMB_write_multi_byte(r_reg.dev_addr, LR4_TOSA_PAGE_SELECT_REG,&page, 1);
			SMB_write_multi_byte(r_reg.dev_addr, U16_LSB(r_reg.offset), &r_reg.value, 1);
		}
		else
		{
			SMB_write_multi_byte(r_reg.dev_addr, r_reg.offset, &r_reg.value, 1);
		}
#else
		SMB_write_multi_byte(r_reg.dev_addr, r_reg.offset, &r_reg.value, 1);
#endif
	}
}

void MSA_p10_init(QSFP_MSA_PAGE_T *page)
{
	_pg10 = page;

	_pg10->ptr        	  		= NULL;
	_pg10->len             		= MSA_PAGE_SIZE;
	_pg10->page_flash_base 		= MSA_FLASH_ADDR_PAGE_0AH;
	_pg10->eep_mask				= NULL;
	_pg10->msa_writable_mask   	= NULL;
	_pg10->vendor_writable_mask  = NULL;

	MSA_p10_eep_load_to_reg();
}

void MSA_p10_eeprom_init(void)
{
	const uint8_t buff[MSA_PAGE_SIZE] = {0};

	flash_write_multi_byte((FLADDR)MSA_FLASH_ADDR_PAGE_0AH, &buff[0], MSA_PAGE_SIZE);
}


#endif

