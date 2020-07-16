/*
 * msa_page8.c
 *
 *  Created on: 2018-3-18
 *      Author: cuilin
 */

#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_i2c_slave.h"
#include "Efm8lb12_smbus.h"
#include "Efm8lb12_100g_com_drv.h"

#ifdef MSA_PAGE_9

static QSFP_MSA_PAGE_T 				*_pg9 = NULL;
static MSA_MANU_AGE_CFG_T 			* _pg5_manu_cfg = NULL;
static ERROR_REASON_T    		  	error_reason;
static MSA_AGING_DETE_THRESHOLD_T 	dete_threshold;

extern volatile SOFT_TIMER_T soft_timer;

static uint8_t _msa_p9_get_aging_enable_status()
{
	QSFP_MSA_PAGE_T *pg5 = MSA_get_page_ptr(MSA_PAGE_05H);
	return FLASH_ByteRead((FLADDR)pg5->page_flash_base +  OFFSET_INDEX(MSA_P5_AGE_ENANLE_STATUS));
}

static void _msa_p9_aging_count()
{
	uint8_t count =  FLASH_ByteRead((FLADDR)(_pg9->page_flash_base +  OFFSET_INDEX(MSA_PAGE9_AGE_COUNT)));

	if(count == AGING_MAX_COUNT)
	{
		return;  //achieve the max aging count ,then return
	}

	if(count == ONE_BYTE_INVALID)
	{
		count = 0;
	}

	count ++;

	flash_write_byte((FLADDR)(_pg9->page_flash_base + OFFSET_INDEX(MSA_PAGE9_AGE_COUNT)), count);
}


static void _msa_p9_aging_state_save(MSA_AGING_STATE state)
{
	flash_write_byte((FLADDR)(_pg9->page_flash_base + OFFSET_INDEX(MSA_PAGE9_AGE_RESULT)), state);
}

static uint8_t _msa_p9_aging_state_read()
{
	return  FLASH_ByteRead((FLADDR)(_pg9->page_flash_base +  OFFSET_INDEX(MSA_PAGE9_AGE_RESULT)));
}

static void _msa_p9_start_temp_aging(int16_t temp)
{
	uint8_t ret = 0;

	dete_threshold.temp.max = MAX(temp, dete_threshold.temp.max);
	dete_threshold.temp.min = MIN(temp, dete_threshold.temp.min);

	if(temp > _pg5_manu_cfg->temp_hi_thrld)
	{
		ret = FLASH_ByteRead((FLADDR)(_pg9->page_flash_base +  OFFSET_INDEX(MSA_PAGE9_AGE_TEMP_VCC_FAIL)));
		ret |= MSA_AGING_MOD_FAIL_HI_TEMP;
		error_reason.temp_vcc |= ret;
		_msa_p9_aging_state_save(MSA_AGING_FAIL);
	}
}

static void _msa_p9_start_vcc_aging(uint16_t vcc)
{
	uint8_t ret = 0;

	if(vcc == 0x0)  //vcc is not ready
	{
		return;
	}

	dete_threshold.vcc.max = MAX(vcc, dete_threshold.vcc.max);
	dete_threshold.vcc.min = MIN(vcc, dete_threshold.vcc.min);

	if(vcc > _pg5_manu_cfg->vcc_hi_thrld)
	{
		ret = FLASH_ByteRead((FLADDR)(_pg9->page_flash_base +  OFFSET_INDEX(MSA_PAGE9_AGE_TEMP_VCC_FAIL)));
		ret |= MSA_AGING_MOD_FAIL_HI_VCC;
		error_reason.temp_vcc |= ret;
		_msa_p9_aging_state_save(MSA_AGING_FAIL);
	}

	if(vcc < _pg5_manu_cfg->vcc_lo_thrld)
	{
		ret = FLASH_ByteRead((FLADDR)(_pg9->page_flash_base +  OFFSET_INDEX(MSA_PAGE9_AGE_TEMP_VCC_FAIL)));
		ret |= MSA_AGING_MOD_FAIL_LO_VCC;
		error_reason.temp_vcc |= ret;
		_msa_p9_aging_state_save(MSA_AGING_FAIL);
	}
}

static void _msa_p9_start_ibias_aging(uint16_t ibias, uint8_t ch)
{
	uint8_t ret = 0;

	if(ibias == 0x0)  //ibias is not ready
	{
		return;
	}

	dete_threshold.ibias[ch].max = MAX(ibias, dete_threshold.ibias[ch].max);
	dete_threshold.ibias[ch].min = MIN(ibias, dete_threshold.ibias[ch].min);

	if(ibias > _pg5_manu_cfg->ibias_hi_thrld)
	{
		ret = FLASH_ByteRead((FLADDR)(_pg9->page_flash_base +  OFFSET_INDEX(MSA_PAGE9_AGE_IBIAS_FAIL)));
		ret |= (1 << (2 * ch));
		error_reason.ibias |= ret;
		_msa_p9_aging_state_save(MSA_AGING_FAIL);
	}

	if(ibias < _pg5_manu_cfg->ibias_lo_thrld)
	{
		ret = FLASH_ByteRead((FLADDR)(_pg9->page_flash_base +  OFFSET_INDEX(MSA_PAGE9_AGE_IBIAS_FAIL)));
		ret |= (1 << (2 * ch + 1));
		error_reason.ibias |= ret;
		_msa_p9_aging_state_save(MSA_AGING_FAIL);
	}
}

static void _msa_p9_start_rssi_aging(uint16_t rssi, uint8_t ch)
{
	uint8_t ret = 0;

	if(rssi == 0x0)  //rssi is not ready
	{
		return;
	}

	dete_threshold.rssi[ch].max = MAX(rssi, dete_threshold.rssi[ch].max);
	dete_threshold.rssi[ch].min = MIN(rssi, dete_threshold.rssi[ch].min);

	if(rssi > _pg5_manu_cfg->rssi_hi_thrld)
	{
		ret = FLASH_ByteRead((FLADDR)(_pg9->page_flash_base +  OFFSET_INDEX(MSA_PAGE9_AGE_RSSI_FAIL)));
		ret |= (1 << (2 * ch));
		error_reason.rssi |= ret;
		_msa_p9_aging_state_save(MSA_AGING_FAIL);
	}

	if(rssi < _pg5_manu_cfg->rssi_lo_thrld)
	{
		ret = FLASH_ByteRead((FLADDR)(_pg9->page_flash_base +  OFFSET_INDEX(MSA_PAGE9_AGE_RSSI_FAIL)));
		ret |= (1 << (2 * ch + 1));
		error_reason.rssi |= ret;
		_msa_p9_aging_state_save(MSA_AGING_FAIL);
	}
}

static void _msa_p9_start_mpd_aging(uint16_t mpd, uint8_t ch)
{
	uint8_t ret = 0;

	if(mpd == 0x0)  //mpd is not ready
	{
		return;
	}

	dete_threshold.mpd[ch].max = MAX(mpd, dete_threshold.mpd[ch].max);
	dete_threshold.mpd[ch].min = MIN(mpd, dete_threshold.mpd[ch].min);

	if(mpd > _pg5_manu_cfg->mpd_hi_thrld)
	{
		ret = FLASH_ByteRead((FLADDR)(_pg9->page_flash_base +  OFFSET_INDEX(MSA_PAGE9_AGE_MPD_FAIL)));
		ret |= (1 << (2 * ch));
		error_reason.mpd |= ret;
		_msa_p9_aging_state_save(MSA_AGING_FAIL);
	}

	if(mpd < _pg5_manu_cfg->mpd_lo_thrld)
	{
		ret = FLASH_ByteRead((FLADDR)(_pg9->page_flash_base +  OFFSET_INDEX(MSA_PAGE9_AGE_MPD_FAIL)));
		ret |= (1 << (2 * ch + 1));
		error_reason.mpd |= ret;
		_msa_p9_aging_state_save(MSA_AGING_FAIL);
	}
}

void MSA_p9_start_aging()
{
	QSFP_MSA_PAGE_T * const pg4 = MSA_get_page_ptr(MSA_PAGE_04H);
	MSA_LUT_T 	  * const stat = MSA_p4_get_laser_state();
	uint8_t ch = 0;

	if(ASSERT(pg4) || ASSERT(stat))
	{
		return;
	}

	_pg5_manu_cfg = MSA_p5_get_manu_age_cfg();

	stat->temp = DDM_GET_TEMP(MSA_DRV_VAL_HW);
	stat->vcc  = DDM_GET_VCC(MSA_DRV_VAL_HW);

	if(pg4->ptr[OFFSET_INDEX(MSA_PAGE4_DDMI_POLL)])
	{
		for (ch = 0; ch< MSA_CHANNEL_NUM; ch++)
		{
			stat->ch[ch].stat.txbias = DDM_GET_BIAS(ch, MSA_DRV_VAL_HW);
			stat->ch[ch].stat.txpwr  = DDM_GET_TX_POWER(ch, MSA_DRV_VAL_HW);
			stat->ch[ch].stat.rssi   = DDM_GET_RX_POWER(ch, MSA_DRV_VAL_HW);
		}
	}

	memset(&error_reason, 0, sizeof(error_reason));
	memset(&dete_threshold, 0, sizeof(dete_threshold));

	dete_threshold.temp.max = stat->temp;
	dete_threshold.temp.min = stat->temp;
	dete_threshold.vcc.max  = stat->vcc;
	dete_threshold.vcc.min  = stat->vcc;

	for(ch = 0; ch < MSA_CHANNEL_NUM; ch ++)
	{
		dete_threshold.ibias[ch].max = stat->ch[ch].stat.txbias;
		dete_threshold.ibias[ch].min = stat->ch[ch].stat.txbias;
		dete_threshold.rssi[ch].max  = stat->ch[ch].stat.rssi;
		dete_threshold.rssi[ch].min  = stat->ch[ch].stat.rssi;
		dete_threshold.mpd[ch].max   = stat->ch[ch].stat.txpwr;
		dete_threshold.mpd[ch].min   = stat->ch[ch].stat.txpwr;
	}

	_msa_p9_aging_state_save(MSA_AGING_LOW_TEMP_RUN);
	TIMER_soft_start(&soft_timer.update_aging_duration_timer);
}

void MSA_p9_stop_aging()
{
	MSA_p9_update_prev_aging_total_time();  //update aging time
	TIMER_soft_stop(&soft_timer.update_aging_duration_timer);//stop update aging time
	 _msa_p9_aging_count();					//update aging counter

	if(_msa_p9_aging_state_read() != MSA_AGING_FAIL)
	{
		_msa_p9_aging_state_save(MSA_AGING_SUCCESS);
	}

	//save detection state info flash
	flash_write_multi_byte((FLADDR)(_pg9->page_flash_base + OFFSET_INDEX(MSA_PAGE9_AGE_TEMP_VCC_FAIL)), (uint8_t *)&error_reason, sizeof(error_reason));
	flash_write_multi_byte((FLADDR)(_pg9->page_flash_base + OFFSET_INDEX(MSA_PAGE9_HI_TEMP_MSB)), (uint8_t *)&dete_threshold, sizeof(dete_threshold));
}

void MSA_p9_update_prev_aging_total_time()
{
	uint16_t prev_time = 0, total_time = 0;

	FLASH_Read((uint8_t *)(&prev_time),(FLADDR)(_pg9->page_flash_base +  OFFSET_INDEX(MSA_PAGE9_PREV_AGE_DURA_MSB)), 2);

	if(prev_time < PREV_AGING_MAX_DURATION)
	{
		total_time = prev_time + 5;
		flash_write_multi_byte((FLADDR)(_pg9->page_flash_base + OFFSET_INDEX(MSA_PAGE9_PREV_AGE_DURA_MSB)), (uint8_t *)(&total_time), 2);
	}
}

static int16_t _msa_p9_get_aging_total_time()
{
	uint16_t prev_time = 0;

	FLASH_Read((uint8_t *)(&prev_time),(FLADDR)(_pg9->page_flash_base +  OFFSET_INDEX(MSA_PAGE9_PREV_AGE_DURA_MSB)), 2);

	return prev_time;
}

static uint16_t _msa_p9_get_aging_target_time()
{
	return _pg5_manu_cfg->duration;
}

static void _msa_p9_aging_timeout()
{
	if(_msa_p9_get_aging_total_time() >= _msa_p9_get_aging_target_time())
	{
		if(_msa_p9_aging_state_read() != MSA_AGING_FAIL)
		{
			_msa_p9_aging_state_save(MSA_AGING_SUCCESS);

			MSA_p9_update_prev_aging_total_time();  //update aging time
			TIMER_soft_stop(&soft_timer.update_aging_duration_timer);//stop update aging time
		}
	}
}

void MSA_p9_on_poll()
{
	if((_msa_p9_aging_state_read() == MSA_AGING_RUNNING || _msa_p9_aging_state_read() == MSA_AGING_LOW_TEMP_RUN) && soft_timer.update_aging_duration_timer->enable) // Running && Aging start OK
	{
		MSA_LUT_T * const stat = MSA_p4_get_laser_state();
		uint8_t age_enable = _msa_p9_get_aging_enable_status();

		if(ASSERT(stat))
		{
			return;
		}

		if(stat->temp < _pg5_manu_cfg->temp_lo_thrld)
		{
			if(_msa_p9_aging_state_read() != MSA_AGING_LOW_TEMP_RUN)
			{
				_msa_p9_aging_state_save(MSA_AGING_LOW_TEMP_RUN);
			}
			return;
		}
		else
		{
			if(_msa_p9_aging_state_read() != MSA_AGING_RUNNING)
			{
			   _msa_p9_aging_state_save(MSA_AGING_RUNNING);
			}
		}

		//aging temp item
		if(age_enable & MSA_AGING_ITEM_TEMP)
		{
			_msa_p9_start_temp_aging(stat->temp);
		}

		//aging vcc item
		if(age_enable & MSA_AGING_ITEM_VCC)
		{
			_msa_p9_start_vcc_aging(stat->vcc);
		}

		//aging ibias item
		if(age_enable & MSA_AGING_ITEM_IBIAS)
		{
			uint8_t ch = 0;
			for(ch = 0; ch < MSA_CHANNEL_NUM; ch ++)
			{
				_msa_p9_start_ibias_aging(stat->ch[ch].stat.txbias, ch);
			}
		}

		//aging rssi item
		if(age_enable & MSA_AGING_ITEM_RSSI)
		{
			uint8_t ch = 0;
			for(ch = 0; ch < MSA_CHANNEL_NUM; ch ++)
			{
				_msa_p9_start_rssi_aging(stat->ch[ch].stat.rssi, ch);
			}
		}

		//aging mpd item
		if(age_enable & MSA_AGING_ITEM_MPD)
		{
			uint8_t ch = 0;
			for(ch = 0; ch < MSA_CHANNEL_NUM; ch ++)
			{
				_msa_p9_start_mpd_aging(stat->ch[ch].stat.txpwr, ch);
			}
		}

		_msa_p9_aging_timeout();

		if(_msa_p9_aging_state_read() != MSA_AGING_RUNNING &&  _msa_p9_aging_state_read() != MSA_AGING_LOW_TEMP_RUN) // Not Running
		{
			//save detection state info flash
			 _msa_p9_aging_count();
			flash_write_multi_byte((FLADDR)(_pg9->page_flash_base + OFFSET_INDEX(MSA_PAGE9_AGE_TEMP_VCC_FAIL)), (uint8_t *)&error_reason, sizeof(error_reason));
			flash_write_multi_byte((FLADDR)(_pg9->page_flash_base + OFFSET_INDEX(MSA_PAGE9_HI_TEMP_MSB)), (uint8_t *)&dete_threshold, sizeof(dete_threshold));
		}
	}
}

uint8_t MSA_p9_read_by_isr(uint8_t offset)
{
	uint8_t value = 0;

	if(MSA_lower_page_get_passwd_status() & FACTORY_PWD_MATCH_OK)  //Vendor Mode:enter factory passward
	{
		value = FLASH_ByteRead_by_i2c_slave_isr(MSA_FLASH_ADDR_PAGE_09H + OFFSET_INDEX(offset));
	}
	return value;
}

uint8_t MSA_p9_write_by_isr(uint8_t offset, uint8_t value)
{
	if(MSA_lower_page_get_passwd_status() & FACTORY_PWD_MATCH_OK)  //Vendor Mode:enter factory passward)
	{
		MSA_write_eep(offset, value);
		return GOS_OK;
	}
	return GOS_FAIL;
}

void MSA_p9_eeprom_init(void)
{
	uint8_t buff[MSA_PAGE_SIZE] = {0};

	flash_write_multi_byte((FLADDR)MSA_FLASH_ADDR_PAGE_09H + OFFSET_INDEX(MSA_PAGE9_AGE_COUNT), &buff[0], MSA_PAGE_SIZE);
}

void MSA_p9_aging_eep_init(void)
{
	const uint8_t buff[MSA_PAGE_SIZE - 1] = {0};

	flash_write_multi_byte((FLADDR)MSA_FLASH_ADDR_PAGE_09H + OFFSET_INDEX(MSA_PAGE9_AGE_RESULT), &buff[0], MSA_PAGE_SIZE - 1);
}

void MSA_p9_init(QSFP_MSA_PAGE_T *page)
{
	_pg9 = page;

	_pg9->ptr        	  		= NULL;
	_pg9->len             		= MSA_PAGE_SIZE;
	_pg9->page_flash_base 		= MSA_FLASH_ADDR_PAGE_09H;
	_pg9->eep_mask				= NULL;
	_pg9->msa_writable_mask   	= NULL;
	_pg9->vendor_writable_mask  = NULL;
}

#endif

