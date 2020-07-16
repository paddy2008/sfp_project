/*
 * msa_page5.c
 *
 *  Created on: 2017-10-28
 *      Author: cuilin
 */

#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_i2c_slave.h"
#include "Efm8lb12_smbus.h"
#include "Efm8lb12_100g_com_drv.h"

#ifdef MSA_PAGE_5

static QSFP_MSA_PAGE_T		*_pg5;
static MSA_MANU_AGE_CFG_T	manu_age_cfg;

uint8_t MSA_p5_read_by_isr(uint8_t offset)
{
	uint8_t value = 0;

	if(MSA_lower_page_get_passwd_status() & FACTORY_PWD_MATCH_OK)  //Vendor Mode:enter factory passward
	{
		value =  FLASH_ByteRead_by_i2c_slave_isr(MSA_FLASH_ADDR_PAGE_05H + OFFSET_INDEX(offset));
	}

	return value;
}

uint8_t MSA_p5_write_by_isr(uint8_t offset, uint8_t value)
{
	if(MSA_lower_page_get_passwd_status() & FACTORY_PWD_MATCH_OK)  //Vendor Mode:enter factory passward
	{
		MSA_write_eep(offset, value);
		return GOS_OK;
	}
	return GOS_FAIL;
}

void MSA_p5_eeprom_init(void)
{
	const uint8_t buff[MSA_PAGE_SIZE] = {0};

	flash_write_multi_byte((FLADDR)MSA_FLASH_ADDR_PAGE_05H, &buff[0], MSA_PAGE_SIZE);
}

void MSA_p5_aging_clear(void)
{
	uint8_t buff[MSA_P5_AGING_CONFIG_SIZE] = {0};

	flash_write_multi_byte((FLADDR)MSA_FLASH_ADDR_PAGE_05H + OFFSET_INDEX(MSA_P5_AGE_ENANLE_STATUS), &buff[0], MSA_P5_AGING_CONFIG_SIZE);
}

MSA_MANU_AGE_CFG_T * MSA_p5_get_manu_age_cfg()
{
	return &manu_age_cfg;
}


void MSA_p5_init(QSFP_MSA_PAGE_T *page)
{
	_pg5 				  		= page;
	_pg5->ptr        	  		= NULL;
	_pg5->len             		= MSA_PAGE_SIZE;
	_pg5->page_flash_base 		= MSA_FLASH_ADDR_PAGE_05H;
	_pg5->eep_mask				= NULL;
	_pg5->msa_writable_mask   	= NULL;
	_pg5->vendor_writable_mask  = NULL;

	FLASH_Read((uint8_t *)(&manu_age_cfg), MSA_FLASH_ADDR_PAGE_LOWER +  OFFSET_INDEX(MSA_P5_MANU_AGE_CFG_START), sizeof(manu_age_cfg));
}

#endif
