#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"
#include <stdarg.h>  //for va_list ap
#include <stdio.h>   //for printf
#include <string.h>  //for strlen
#include <stdlib.h>  //for atoi
#include <math.h>
#ifdef MSA_PAGE_1

static QSFP_MSA_PAGE_T *_pg1 = NULL;

void MSA_p1_init(QSFP_MSA_PAGE_T *page)
{
	_pg1 = page;
	_pg1->ptr        	  		= NULL;
	_pg1->len             		= 0;
	_pg1->page_flash_base 		= MSA_FLASH_ADDR_PAGE_01H;
	_pg1->eep_mask				= NULL;
	_pg1->msa_writable_mask   	= NULL;
	_pg1->vendor_writable_mask  = NULL;
}


uint8_t MSA_p1_read_by_isr(uint8_t offset)
{
	return FLASH_ByteRead_by_i2c_slave_isr(MSA_FLASH_ADDR_PAGE_01H + OFFSET_INDEX(offset));
}

uint8_t MSA_p1_write_by_isr(uint8_t offset, uint8_t value)
{
	if(MSA_lower_page_get_passwd_status())  //Vendor mode or OEM mode
	{
		MSA_write_eep(offset, value);
	}
	else  //Customer mode
	{
		//not access to write and read ,do nothing
	}
	return GOS_OK;
}

void MSA_p1_eeprom_init(void)
{
	const uint8_t buff[MSA_PAGE_SIZE] = {0};

	flash_write_multi_byte((FLADDR)MSA_FLASH_ADDR_PAGE_01H, &buff[0], MSA_PAGE_SIZE);
}


#endif
