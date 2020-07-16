/*
 * msa_page2.c
 *
 *  Created on: 2017-10-13
 *      Author: cuilin
 */

#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"
#include "version.h"
#include <stdarg.h>  //for va_list ap
#include <stdio.h>   //for printf
#include <string.h>  //for strlen
#include <stdlib.h>  //for atoi
#include <math.h>
#ifdef MSA_PAGE_2

static QSFP_MSA_PAGE_T *_pg2;

#if defined(SUPPORT_LOG_FUNC)
// _log[0] is start index of log length
static uint8_t _log[MSA_DBG_LOG_SIZE];
static uint8_t _enabled;

#endif

static const uint8_t _msa_writable_mask[MSA_PAGE_MASK_LEN] =
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

void MSA_p2_eeprom_init(void)
{
	const uint8_t buff[MSA_PAGE_SIZE] = {0};

	flash_write_multi_byte((FLADDR)MSA_FLASH_ADDR_PAGE_02H, &buff[0], MSA_PAGE_SIZE);
}

void MSA_p2_init(QSFP_MSA_PAGE_T *page)
{
	_pg2 = page;
#if defined(SUPPORT_LOG_FUNC)
	_pg2->ptr        	  		= _log;
	_pg2->len             		= MSA_DBG_LOG_SIZE;
#else
	_pg2->ptr        	  		= NULL;
	_pg2->len             		= 0;
#endif

	_pg2->page_flash_base 		= MSA_FLASH_ADDR_PAGE_02H;
	_pg2->eep_mask				= NULL;
	_pg2->msa_writable_mask   	= &_msa_writable_mask[0];
	_pg2->vendor_writable_mask  = NULL;

#if defined(SUPPORT_LOG_FUNC)
	memset(_log, 0, MSA_DBG_LOG_SIZE);
	_log[0] = 1;
	_enabled = MSA_DBG_LOG_ENABLE;
#endif
}

uint8_t MSA_p2_write_by_isr(uint8_t offset, uint8_t value)
{
	MSA_write_eep(offset, value);
	return GOS_OK;
}


uint8_t MSA_p2_read_by_isr(uint8_t offset)
{
#if defined(SUPPORT_LOG_FUNC)
	return _log[OFFSET_INDEX(offset)];
#else
	return FLASH_ByteRead_by_i2c_slave_isr(MSA_FLASH_ADDR_PAGE_02H + OFFSET_INDEX(offset));
#endif
}

#if defined(SUPPORT_LOG_FUNC)
uint8_t MSA_p2_handler(MSA_ACTION action, uint8_t offset, uint8_t *pValue)
{
	uint8_t ret = 0;

	switch(action){
	case MSA_ACTION_READ_BYTE:
		*pValue = _log[OFFSET_INDEX(offset)];
		break;
	case MSA_ACTION_DEBUG_LOG:
		if (*pValue == MSA_DEBUG_LOG_CLEAR)	{
			if (_enabled) {
				memset(_log, 0, MSA_DBG_LOG_SIZE);
				_log[0] = 1;
			}
		}else if (*pValue == 0)
			_enabled = false;
		else
			_enabled = true;
		break;
	default:
		ret = -1;
		break;
	}

	return ret;
}

void MSA_p2_log(uint8_t *fmt,...)
{
char string[128];
uint8_t slen;
if (_enabled)
	{
	va_list ap;
	va_start(ap,fmt);
	vsprintf(string,fmt,ap);
	va_end(ap);
	slen = strlen(string);
	if ((_log[0] + slen) >= MSA_DBG_LOG_SIZE)
				return;
			strcpy(&_log[_log[0]], string);
			_log[0] += slen;
	}
}

void MSA_p2_log_isr(uint8_t *fmt,...)
{
char string[128];
uint8_t slen;
if (_enabled)
	{
	va_list ap;
	va_start(ap,fmt);
	vsprintf(string,fmt,ap);
	va_end(ap);
	slen = strlen(string);
	if ((_log[0] + slen) >= MSA_DBG_LOG_SIZE)
				return;
			strcpy(&_log[_log[0]], string);
			_log[0] += slen;
	}
}

#endif
#endif

