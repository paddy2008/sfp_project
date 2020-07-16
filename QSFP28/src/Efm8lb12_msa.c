#include "Efm8lb12_common.h"
#include "Efm8lb12_eep_interface.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_task_sched.h"
#include "Efm8lb12_i2c_slave.h"
#include "Efm8lb12_100g_com_drv.h"

QSFP_MSA_DRV_DATA msa_drv_data;

static volatile uint16_t _msa_control_drv_mask = MSA_CONTROL_BYTE_MASK;

static volatile MSA_WRITE_EEP_BUFF_T _msa_write_eep;

void MSA_factory_reset(void)
{
	MSA_page_lower_eeprom_init();
	MSA_p0_eeprom_init();
	MSA_p1_eeprom_init();
	MSA_p2_eeprom_init();
	MSA_p3_eeprom_init();
}

void MSA_init(void)
{
	memset((char *)&_msa_write_eep, 0, sizeof(_msa_write_eep));

	memset((char *)&msa_drv_data, 0, sizeof(msa_drv_data));
    msa_drv_data.op = MSA_OP_IDLE;

#ifdef MSA_PAGE_2  //because page2 is used for log function, so it is init at first
    MSA_p2_init(&msa_drv_data.pages[MSA_PAGE_02H]);
#endif

    MSA_p0_init(&msa_drv_data.pages[MSA_PAGE_00H]);

#ifdef MSA_PAGE_1
    MSA_p1_init(&msa_drv_data.pages[MSA_PAGE_01H]);
#endif

#ifdef MSA_PAGE_3
    MSA_p3_init(&msa_drv_data.pages[MSA_PAGE_03H]);
#endif

#ifdef MSA_PAGE_4
    MSA_p4_init(&msa_drv_data.pages[MSA_PAGE_04H]);
#endif

#ifdef MSA_PAGE_5
    MSA_p5_init(&msa_drv_data.pages[MSA_PAGE_05H]);
#endif

#ifdef MSA_PAGE_6
    MSA_p67_init(&msa_drv_data.pages[MSA_PAGE_06H]);
#endif

#ifdef MSA_PAGE_9
    MSA_p9_init(&msa_drv_data.pages[MSA_PAGE_09H]);
#endif

#ifdef MSA_PAGE_10
    MSA_p10_init(&msa_drv_data.pages[MSA_PAGE_0AH]);
#endif

#ifdef MSA_PAGE_LOWER_PAGE
   MSA_page_lower_init(&msa_drv_data.lower_page);
#endif

	MSA_page_lower_set_boot_status(false);  //set mod status not ready
}


void MSA_write_eep(uint8_t offset, const uint8_t value)  //this function is called by i2c slave isr
{
	uint8_t index = 0, page = 0;

	for(index = 0; index < MSA_PAGE_SIZE; index ++)
	{
		if(GET_BIT(_msa_write_eep.buff[index].page, 7) == 0x0) //This addr is empty
		{
			page = MSA_page_lower_get_page_select();
			_msa_write_eep.buff[index].offset = offset;
			_msa_write_eep.buff[index].value  = value;
			_msa_write_eep.buff[index].page   = SET_BIT(page, 7);
			_msa_write_eep.num ++;
			break;
		}
	}
}

static uint8_t  _msa_get_page_by_index(uint8_t index)
{
	if(_msa_write_eep.buff[index].offset >= 0x0 && _msa_write_eep.buff[index].offset < 0x80)
	{
		return MSA_PAGE_LOWER;
	}
	else
	{
		return (_msa_write_eep.buff[index].page & 0x7F);
	}
}

void MSA_write_eep_poll()
{
	FLADDR  addr = 0;
	uint8_t i = 0, page = 0;
	QSFP_MSA_PAGE_T  *page_ptr = NULL;

	if(_msa_write_eep.num == 0)
	{
		return;
	}

	for (i = 0; i < MSA_PAGE_SIZE, _msa_write_eep.num; i++)
	{
		if(GET_BIT(_msa_write_eep.buff[i].page, 7))  //if the buff is not empty
		{
			page = _msa_get_page_by_index(i);
			page_ptr = MSA_get_page_ptr(page);

			if(ASSERT(page_ptr) || ASSERT(page_ptr->page_flash_base))
			{
				_msa_write_eep.num -- ;
				_msa_write_eep.buff[i].page  = CLEAR_BIT(_msa_write_eep.buff[i].page, 7);
				return;
			}

			if(MSA_lower_page_get_passwd_status() == 0x0)  //Customer mode:not enter OEM passward and not enter facotry passward
			{
				if(get_bit(page_ptr->msa_writable_mask, OFFSET_INDEX(_msa_write_eep.buff[i].offset)) == 0x0)
				{
					 _msa_write_eep.num -- ;
					 _msa_write_eep.buff[i].page  = CLEAR_BIT(_msa_write_eep.buff[i].page, 7);  //this code must be after the last code, after this code can be excueted , MSA_write_eep can be called right now
					 continue;
				}
			}

			flash_write_byte((FLADDR)(page_ptr->page_flash_base + OFFSET_INDEX(_msa_write_eep.buff[i].offset)), _msa_write_eep.buff[i].value);

			_msa_write_eep.num -- ;
			_msa_write_eep.buff[i].page  = CLEAR_BIT(_msa_write_eep.buff[i].page, 7);
		}
	}
}

bool MSA_write(uint8_t offset, const uint8_t value)
{
    uint8_t ret = 0;

    if (offset == OFFSET_INDEX(offset)) // lower page
    {
    	ret = MSA_page_lower_write_by_isr(OFFSET_INDEX(offset), value);
    }
    else
    {
    	MSA_PAGE_NUM page = 0;

    	page = MSA_page_lower_read_by_isr(OFFSET_INDEX(MSA_OFFSET_LOWER_PAGE_SELECT_BYTE));

    	switch(page) {
#ifdef MSA_PAGE_0
    	case MSA_PAGE_00H:
    		ret = MSA_p0_write_by_isr(offset, value);
    		break;
#endif
#ifdef MSA_PAGE_1
    	case MSA_PAGE_01H:
    		ret = MSA_p1_write_by_isr(offset, value);
    		break;
#endif
#ifdef MSA_PAGE_2
    	case MSA_PAGE_02H:
			ret = MSA_p2_write_by_isr(offset, value);
			break;
#endif
#ifdef MSA_PAGE_3
		case MSA_PAGE_03H:
			ret = MSA_p3_write_by_isr(offset, value);
			break;
#endif
#ifdef MSA_PAGE_4
    	case MSA_PAGE_04H:
    		ret = MSA_p4_write_by_isr(offset, value);
    		break;
#endif
#ifdef MSA_PAGE_5
    	case MSA_PAGE_05H:
    		ret = MSA_p5_write_by_isr(offset, value);
    		break;
#endif
#ifdef MSA_PAGE_6
    	case MSA_PAGE_06H:
    	case MSA_PAGE_07H:
    	case MSA_PAGE_08H:
    	{
    		ret = MSA_p67_write_by_isr(page, offset, value);
    		break;
    	}
#endif
#ifdef MSA_PAGE_9
		case MSA_PAGE_09H:
			ret = MSA_p9_write_by_isr(offset, value);
			break;
#endif
#ifdef MSA_PAGE_10
		case MSA_PAGE_0AH:
			ret = MSA_p10_write_by_isr(offset, value);
			break;
#endif
    	default:
    		ret = -1;
    		break;
    	}
    }
    return ret;
}

uint8_t MSA_read(uint8_t offset)
{
    int8_t ret = 0;

	if (offset == OFFSET_INDEX(offset)) // lower page
	{
		ret  = MSA_page_lower_read_by_isr(offset);
	}
	else
	{
		MSA_PAGE_NUM page = 0;
		page  = MSA_page_lower_read_by_isr(MSA_OFFSET_LOWER_PAGE_SELECT_BYTE);

		switch(page) {
#ifdef MSA_PAGE_0
		case MSA_PAGE_00H:
			ret = MSA_p0_read_by_isr(offset);
			break;
#endif
#ifdef MSA_PAGE_1
		case MSA_PAGE_01H:
			ret = MSA_p1_read_by_isr(offset);
			break;
#endif
#ifdef MSA_PAGE_2
		case MSA_PAGE_02H:
			ret = MSA_p2_read_by_isr(offset);
			break;
#endif
#ifdef MSA_PAGE_3
		case MSA_PAGE_03H:
			ret = MSA_p3_read_by_isr(offset);
			break;
#endif
#ifdef MSA_PAGE_4
		case MSA_PAGE_04H:
			ret = MSA_p4_read_by_isr(offset);
			break;
#endif
#ifdef MSA_PAGE_5
		case MSA_PAGE_05H:
			ret = MSA_p5_read_by_isr(offset);
			break;
#endif
#ifdef MSA_PAGE_6
		case MSA_PAGE_06H:
		case MSA_PAGE_07H:
		case MSA_PAGE_08H:
		{
			ret = MSA_p67_read_by_isr(page, offset);
			break;
		}
#endif
#ifdef MSA_PAGE_9
		case MSA_PAGE_09H:
			ret = MSA_p9_read_by_isr(offset);
			break;
#endif
#ifdef MSA_PAGE_10
		case MSA_PAGE_0AH:
			ret = MSA_p10_read_by_isr(offset);
			break;
#endif
		default:
			ret = -1;
			break;
		}
	}
	return ret;
}

QSFP_MSA_PAGE_T	* MSA_get_page_ptr(MSA_PAGE_NUM pg_num)
{
	if(pg_num == MSA_PAGE_LOWER)
	{
		return &msa_drv_data.lower_page;
	}
	else if(pg_num >= MSA_PAGE_00H && pg_num < MSA_PAGE_MAX)
	{
		return &msa_drv_data.pages[pg_num];
	}
	else
	{
		return NULL;
	}
}

uint8_t MSA_check_code(uint8_t *start, uint8_t len)
{
	uint8_t  i = 0;
	uint16_t ret = 0;

	for(i = 0 ; i < len; i++)
	{
		ret += *start;
		start ++;
	}

	return (ret & 0xFF);
}

uint16_t MSA_get_control_drv_mask()
{
	return _msa_control_drv_mask;
}

void MSA_set_control_drv_mask(uint16_t value)
{
	_msa_control_drv_mask = value;
}

void MSA_control_byte_poll()
{
#if defined(SUPPORT_MSA_CONTROL_BYTE)
	QSFP_MSA_PAGE_T *pg3 	  = MSA_get_page_ptr(MSA_PAGE_03H);
	QSFP_MSA_PAGE_T *lower_pg = MSA_get_page_ptr(MSA_PAGE_LOWER);
	uint8_t channel = 0, byte = 0;

	if(ASSERT(pg3) || ASSERT(lower_pg))
	{
		return;
	}

	if((_msa_control_drv_mask & MSA_CONTROL_BYTE_MASK) == 0x0)
	{
		return;
	}

	if(_msa_control_drv_mask & TX_ENABLE)
	{
		SFP_DRV_SET_TX_ENABLE(lower_pg->ptr[OFFSET_INDEX(MSA_OFFSET_LOWER_TX_DISABLE)]);
		_msa_control_drv_mask &= ~TX_ENABLE;
	}

	if(_msa_control_drv_mask & LOW_POWER_MODE)
	{
		MSA_page_lower_set_low_power_mode();
		_msa_control_drv_mask &= ~LOW_POWER_MODE;
	}

	if(_msa_control_drv_mask & TX_RX_CDR_CONTROL)
	{
		SFP_DRV_SET_CDR_CONTROL(lower_pg->ptr[OFFSET_INDEX(MSA_OFFSET_LOWER_TX_CDR_CTL)]);
		_msa_control_drv_mask &= ~TX_RX_CDR_CONTROL;
	}

	if(_msa_control_drv_mask & TX1_TX2_EQ)
	{
		uint8_t tx_adapt_eq = 0;

		tx_adapt_eq = pg3->ptr[OFFSET_INDEX(MSA_OFFSET_PAGE_03H_RX_OUTPUT_DISABLE)] & 0x0F;

		for(channel = 0x0; channel < 0x2; channel++)
		{
			if(GET_BIT(tx_adapt_eq, channel))
			{
				byte = 0xFF;
			}
			else
			{
				byte = pg3->ptr[OFFSET_INDEX(MSA_OFFSET_PAGE_03H_TX1_TX2_INPUT_EQUAL_CTL)];
				byte = (byte >> (4 * ((channel + 1) % 2))) & 0x0F;
			}
			SFP_DRV_SET_TX_EQ(channel, byte);
		}
		_msa_control_drv_mask &= ~TX1_TX2_EQ;
	}

	if(_msa_control_drv_mask & TX3_TX4_EQ)
	{
		uint8_t tx_adapt_eq = 0;

		tx_adapt_eq = pg3->ptr[OFFSET_INDEX(MSA_OFFSET_PAGE_03H_RX_OUTPUT_DISABLE)] & 0x0F;

		for(channel = 0x2; channel < MSA_CHANNEL_NUM; channel++)
		{
			if(GET_BIT(tx_adapt_eq, channel))
			{
				byte = 0xFF;
			}
			else
			{
				byte = pg3->ptr[OFFSET_INDEX(MSA_OFFSET_PAGE_03H_TX3_TX4_INPUT_EQUAL_CTL)];
				byte = (byte >> (4 * ((channel + 1) % 2))) & 0x0F;
			}
			SFP_DRV_SET_TX_EQ(channel, byte);
		}
		_msa_control_drv_mask &= ~TX3_TX4_EQ;
	}

	if(_msa_control_drv_mask & RX1_RX2_EMPHASIS)
	{
		byte = pg3->ptr[OFFSET_INDEX(MSA_OFFSET_PAGE_03H_RX1_RX2_OUTPUT_EQUAL_CTL)];

		for(channel = 0x0; channel < 0x2; channel++)
		{
			SFP_DRV_SET_RX_EMPHASIS(channel, (byte >> (4 * ((channel + 1) % 2))) & 0x0F);
		}
		_msa_control_drv_mask &= ~RX1_RX2_EMPHASIS;
	}

	if(_msa_control_drv_mask & RX3_RX4_EMPHASIS)
	{
		byte = pg3->ptr[OFFSET_INDEX(MSA_OFFSET_PAGE_03H_RX3_RX4_OUTPUT_EQUAL_CTL)];

		for(channel = 0x2; channel < MSA_CHANNEL_NUM; channel++)
		{
			SFP_DRV_SET_RX_EMPHASIS(channel, (byte >> (4 * ((channel + 1) % 2))) & 0x0F);
		}
		_msa_control_drv_mask &= ~RX3_RX4_EMPHASIS;
	}

	if(_msa_control_drv_mask & RX1_RX2_AMPLITUDE)
	{
		byte = pg3->ptr[OFFSET_INDEX(MSA_OFFSET_PAGE_03H_RX1_RX2_OUTPUT_AMPL_CTL)];

		for(channel = 0x0; channel < 0x2; channel++)
		{
			SFP_DRV_SET_RX_AMPLITUDE(channel, (byte >> (4 * ((channel + 1) % 2))) & 0x0F);
		}
		_msa_control_drv_mask &= ~RX1_RX2_AMPLITUDE;
	}

	if(_msa_control_drv_mask & RX3_RX4_AMPLITUDE)
	{
		byte = pg3->ptr[OFFSET_INDEX(MSA_OFFSET_PAGE_03H_RX3_RX4_OUTPUT_AMPL_CTL)];

		for(channel = 0x2; channel < MSA_CHANNEL_NUM; channel++)
		{
			SFP_DRV_SET_RX_AMPLITUDE(channel, (byte >> (4 * ((channel + 1) % 2))) & 0x0F);
		}
		_msa_control_drv_mask &= ~RX3_RX4_AMPLITUDE;
	}

	if(_msa_control_drv_mask & TX_RX_SQ_DISABLE)
	{
		byte = pg3->ptr[OFFSET_INDEX(MSA_OFFSET_PAGE_03H_RX_TX_SQ)];
		SFP_DRV_SET_CDR_SQ_DISABLE(byte);
		_msa_control_drv_mask &= ~TX_RX_SQ_DISABLE;
	}

	if(_msa_control_drv_mask & RX_OUTPUT_TX_ADAPT_EQ)
	{
		uint8_t byte241 = pg3->ptr[OFFSET_INDEX(MSA_OFFSET_PAGE_03H_RX_OUTPUT_DISABLE)];
		uint8_t byte234 = pg3->ptr[OFFSET_INDEX(MSA_OFFSET_PAGE_03H_TX1_TX2_INPUT_EQUAL_CTL)];
		uint8_t byte235 = pg3->ptr[OFFSET_INDEX(MSA_OFFSET_PAGE_03H_TX3_TX4_INPUT_EQUAL_CTL)];

		SFP_DRV_SET_RX_OUTPUT_DISABLE(byte241);
		SFP_DRV_SET_TX_ADAPT_EQ(byte241, byte234, byte235);
		_msa_control_drv_mask &= ~RX_OUTPUT_TX_ADAPT_EQ;
	}
#endif
}

