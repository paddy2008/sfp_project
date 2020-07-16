#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_i2c_slave.h"
#include "Efm8lb12_port.h"
#include "Efm8lb12_msa_pin.h"
#include "Efm8lb12_100g_com_drv.h"
#include "Efm8lb12_smbus.h"


static QSFP_MSA_PAGE_T 				*_page_data      = NULL;
static MSA_PAGE_LOWER_ISR_FLAG_T	*_isr_flag       = NULL;

static volatile uint8_t _data_ram[MSA_PAGE_SIZE] = {0};
static volatile uint8_t _old_mon[sizeof(MSA_PAGE_LOWER_MON_T)] = {0};
static volatile uint8_t _sem = 1;
static volatile uint8_t _passwd_status = 0;
static bool _read_ready_intl = false;

#if defined(MSA_NORMAL_MODE)

static const uint8_t  _eeprom_init_data[MSA_PAGE_SIZE] =
{
	0x11, 0x07, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, //0x00 ~ 0x07
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x08 ~ 0x0F
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x10 ~ 0x17
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x18 ~ 0x1F
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x20 ~ 0x27
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x28 ~ 0x2F
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x30 ~ 0x37
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x38 ~ 0x3F
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x40 ~ 0x47
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x48 ~ 0x4F
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x50 ~ 0x57
	0x00, 0x00, 0x00, 0x00, 0x00,  					//0x58 ~ 0x5C

#if defined(EFM8LB12E64_100G_SFP_LR4_301)
	0x04, 											//0x5D
#else
	0x00,											//0x5D
#endif
	0x00, 0x00,										//0x5E ~ 0x5F

	0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x60 ~ 0x67
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x68 ~ 0x6F
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x4C, //0x70 ~ 0x77
	0x50, 0x41, 0x4E, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x78 ~ 0x7F
};

static const uint8_t _msa_writable_mask[MSA_PAGE_MASK_LEN] =
{
	//1-- Write, 0--Not Write(Only Read), bit0 replace low addr, bit7 replace high addr
	0x00, 0x00, //byte0   ~ byte7, 	byte8   ~ byte15
	0x00, 0x00,	//byte16  ~ byte23, byte24  ~ byte31
	0x00, 0x00,	//byte32  ~ byte39, byte40  ~ byte47
	0x00, 0x00, //byte48  ~ byte55, byte56  ~ byte63
	0x00, 0x00, //byte64  ~ byte71, byte72  ~ byte79
	0xC0, 0xFF, //byte80  ~ byte87, byte88  ~ byte95
	0xFF, 0x8F, //byte96  ~ byte103,byte104 ~ byte111
	0x7D, 0xF8, //byte112 ~ byte119,byte120 ~ byte127
};

static const uint8_t _vendor_writable_mask[MSA_PAGE_MASK_LEN] =
{
	//1-- Write, 0--Not Write(Only Read), bit0 replace low addr, bit7 replace high addr
	0x03, 0x00, //byte0   ~ byte7, 	byte8   ~ byte15
	0x00, 0x00,	//byte16  ~ byte23, byte24  ~ byte31
	0x00, 0x00,	//byte32  ~ byte39, byte40  ~ byte47
	0x00, 0x00, //byte48  ~ byte55, byte56  ~ byte63
	0x00, 0x00, //byte64  ~ byte71, byte72  ~ byte79
	0xC0, 0xFF, //byte80  ~ byte87, byte88  ~ byte95
	0xFF, 0xFF, //byte96  ~ byte103,byte104 ~ byte111
	0xFF, 0xFF, //byte112 ~ byte119,byte120 ~ byte127
};


static const uint8_t _eep_mask[MSA_PAGE_MASK_LEN] =
{
	//1-- Write, 0--Not Write(Only Read), bit0 replace low addr, bit7 replace high addr
	0x03, 0x00, //byte0   ~ byte7, 	byte8   ~ byte15
	0x00, 0x00,	//byte16  ~ byte23, byte24  ~ byte31
	0x00, 0x00,	//byte32  ~ byte39, byte40  ~ byte47
	0x00, 0x00, //byte48  ~ byte55, byte56  ~ byte63
	0x00, 0x00, //byte64  ~ byte71, byte72  ~ byte79
	0x80, 0xFF, //byte80  ~ byte87, byte88  ~ byte95
	0x07, 0xF0, //byte96  ~ byte103,byte104 ~ byte111
	0x83, 0x07, //byte112 ~ byte119,byte120 ~ byte127
};

#elif defined(SKYLANE_MODE)

static const uint8_t  _eeprom_init_data[MSA_PAGE_SIZE] =
{
	0x11, 0x07, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, //0x00 ~ 0x07
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x08 ~ 0x0F
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x10 ~ 0x17
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x18 ~ 0x1F
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x20 ~ 0x27
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x28 ~ 0x2F
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x30 ~ 0x37
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x38 ~ 0x3F
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x40 ~ 0x47
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x48 ~ 0x4F
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x50 ~ 0x57
	0x00, 0x00, 0x00, 0x00, 0x00,  					//0x58 ~ 0x5C

#if defined(EFM8LB12E64_100G_SFP_LR4_301)
	0x04, 											//0x5D
#else
	0x00,											//0x5D
#endif
	0x00, 0x00,										//0x5E ~ 0x5F

	0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF ,0x00, //0x60 ~ 0x67
	0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x68 ~ 0x6F
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x4C, //0x70 ~ 0x77
	0x50, 0x41, 0x4E, 0x00, 0x00, 0x00, 0x00 ,0x00, //0x78 ~ 0x7F
};

static const uint8_t _msa_writable_mask[MSA_PAGE_MASK_LEN] =
{
	//1-- Write, 0--Not Write(Only Read), bit0 replace low addr, bit7 replace high addr
	0x00, 0x00, //byte0   ~ byte7, 	byte8   ~ byte15
	0x00, 0x00,	//byte16  ~ byte23, byte24  ~ byte31
	0x00, 0x00,	//byte32  ~ byte39, byte40  ~ byte47
	0x00, 0x00, //byte48  ~ byte55, byte56  ~ byte63
	0x00, 0x00, //byte64  ~ byte71, byte72  ~ byte79
	0xC0, 0xFF, //byte80  ~ byte87, byte88  ~ byte95
	0xBF, 0x8F, //byte96  ~ byte103,byte104 ~ byte111
	0x7D, 0xF8, //byte112 ~ byte119,byte120 ~ byte127
};

static  uint8_t _eep_mask[MSA_PAGE_MASK_LEN] =
{
	//1-- Write, 0--Not Write(Only Read), bit0 replace low addr, bit7 replace high addr
	0x23, 0x00, //byte0   ~ byte7, 	byte8   ~ byte15
	0x00, 0x00,	//byte16  ~ byte23, byte24  ~ byte31
	0x00, 0x00,	//byte32  ~ byte39, byte40  ~ byte47
	0x00, 0x00, //byte48  ~ byte55, byte56  ~ byte63
	0x00, 0x00, //byte64  ~ byte71, byte72  ~ byte79
	0x80, 0xFF, //byte80  ~ byte87, byte88  ~ byte95
	0x07, 0xF4, //byte96  ~ byte103,byte104 ~ byte111
	0x83, 0x07, //byte112 ~ byte119,byte120 ~ byte127
};


static const uint8_t _vendor_writable_mask[MSA_PAGE_MASK_LEN] =
{
	//1-- Write, 0--Not Write(Only Read), bit0 replace low addr, bit7 replace high addr
	0x23, 0x00, //byte0   ~ byte7, 	byte8   ~ byte15
	0x00, 0x00,	//byte16  ~ byte23, byte24  ~ byte31
	0x00, 0x00,	//byte32  ~ byte39, byte40  ~ byte47
	0x00, 0x00, //byte48  ~ byte55, byte56  ~ byte63
	0x00, 0x00, //byte64  ~ byte71, byte72  ~ byte79
	0xC0, 0xFF, //byte80  ~ byte87, byte88  ~ byte95
	0xBF, 0xFF, //byte96  ~ byte103,byte104 ~ byte111
	0xFF, 0xFF, //byte112 ~ byte119,byte120 ~ byte127
};


#endif
//SKYLANE_MODE END

static void _msa_page_lower_control_drv_mask(uint8_t offset);

void MSA_page_lower_set_intr_flag()
{
	uint8_t channel = 0;
	MSA_INTR_STATUS_T drv_intl_status;
	static uint8_t prev_tx_state = 0;

	memset(&drv_intl_status, '\0', sizeof(drv_intl_status));  //must clear zero

	if(!MSA_p4_get_ddmi_poll()) //support ddmi disable cmd to stop polling
	{
		return;
	}

	SFP_DRV_INTR_STATUS(&drv_intl_status);

	if ((prev_tx_state && (!TX_DIS)) || (TX_DIS))  // from lower power mode change to high power mode, there is no need to update interrupt flag
	{
		if(TX_DIS)
		{
			prev_tx_state = TX_DIS ? 1 : 0;
		}
		else
		{
			prev_tx_state = 0;  //from lower power mode change to high power mode, there is need to reset prev_tx_state
		}
		MSA_pin_on_poll();
		return;  //notice:there must be to write code "return" in order to avoid next action make tx_dis = 0
	}

	//set flag
	for(channel = MSA_PWR_CHANNEL_1; channel < MSA_CHANNEL_NUM; channel ++)
	{
		if(GET_BIT(drv_intl_status.tx_los, channel)) //tx los
		{
			_isr_flag->txrx_los |= MSA_PAGE_LOWER_TX_LOS_MASK(channel);
		}

#if defined(MSA_NORMAL_MODE)
		if(GET_BIT(drv_intl_status.tx_lol, channel)) //tx lol
		{
			_isr_flag->txrx_lol |= MSA_PAGE_LOWER_TX_LOL_MASK(channel);
		}
#elif defined(SKYLANE_MODE)

#endif
		if(GET_BIT(drv_intl_status.tx_fault, channel)) //tx fault
		{
			_isr_flag->tx_fault |= MSA_PAGE_LOWER_TX_FAULT_MASK(channel);
		}

		if(GET_BIT(drv_intl_status.rx_los, channel)) //rx los
		{
			_isr_flag->txrx_los |= MSA_PAGE_LOWER_RX_LOS_MASK(channel);
		}

#if defined(MSA_NORMAL_MODE)
		if(GET_BIT(drv_intl_status.rx_lol, channel)) //rx lol
		{
			_isr_flag->txrx_lol |= MSA_PAGE_LOWER_RX_LOL_MASK(channel);
		}
#elif defined(SKYLANE_MODE)

#endif
	}

	MSA_pin_on_poll();  //set intl pin status after update intr flag right now
}

//because ddmi data are two bytes, there must be handled specially
static uint8_t _msa_page_lower_ram_data_handle(uint8_t offset)
{
	uint8_t value = 0;
	static  bool  update_dom = false;

	if((OFFSET_INDEX(offset) >= MSA_OFFSET_LOWER_TEMP_MSB && OFFSET_INDEX(offset) <= MSA_OFFSET_LOWER_TX4_PWR_LSB) && !COM_GET_LOCK(_sem))
	{
		value = _old_mon[OFFSET_INDEX(offset) - MSA_OFFSET_LOWER_TEMP_MSB];
		update_dom = true;
	}
	else
	{
		if(update_dom)
		{
			value = _old_mon[OFFSET_INDEX(offset) - MSA_OFFSET_LOWER_TEMP_MSB];
			update_dom = false;
		}
		else
		{
			value =  _page_data->ptr[OFFSET_INDEX(offset)];
		}
	}
	return value;
}

uint8_t MSA_page_lower_read_by_isr(uint8_t offset)
{
	uint8_t value = 0;

	/*byte 119 ~ byte 126 is not support read*/
	if((OFFSET_INDEX(offset) >= MSA_OFFSET_LOWER_PSW_CHG_ENTRY_AREA_BYTE119) && (OFFSET_INDEX(offset) <= MSA_OFFSET_LOWER_PSW_ENTRY_AREA_BYTE126))
	{
		value = 0;
	}
	else
	{
		value = _msa_page_lower_ram_data_handle(offset);
	}

#if defined(MSA_NORMAL_MODE)
	//byte3 ~ byte14 are read clear
	if((OFFSET_INDEX(offset) >= MSA_OFFSET_LOWER_LOS) && (OFFSET_INDEX(offset) <= MSA_OFFSET_LOWER_TX3_TX4_ALM_WARNING))
	{
		_page_data->ptr[OFFSET_INDEX(offset)] = 0;
	}
#elif defined(SKYLANE_MODE)
	//byte3 ~ byte14 are read clear
	if((OFFSET_INDEX(offset) >= MSA_OFFSET_LOWER_LOS) && (OFFSET_INDEX(offset) <= MSA_OFFSET_LOWER_TX3_TX4_ALM_WARNING))
	{
		if(OFFSET_INDEX(offset) != MSA_OFFSET_LOWER_LOL)
			_page_data->ptr[OFFSET_INDEX(offset)] = 0;
	}
#endif

	return value;
}

uint8_t MSA_page_lower_write_by_isr(uint8_t offset, uint8_t value)
{
	if(MSA_lower_page_get_passwd_status())  //Vendor mode or OEM mode
	{
		if (get_bit_by_isr(_page_data->vendor_writable_mask, OFFSET_INDEX(offset)))  //write access
		{
			_page_data->ptr[OFFSET_INDEX(offset)] = value;
			_msa_page_lower_control_drv_mask(OFFSET_INDEX(offset));

			if(OFFSET_INDEX(offset) >= MSA_OFFSET_LOWER_PSW_ENTRY_AREA_BYTE123 && OFFSET_INDEX(offset) <= MSA_OFFSET_LOWER_PSW_ENTRY_AREA_BYTE126)
			{
				MSA_page_lower_check_passwd();
			}

			if (get_bit_by_isr(_page_data->eep_mask, OFFSET_INDEX(offset)))
			{
				MSA_write_eep(offset, value);
			}
		}
	}
	else  //Customer mode
	{
		if (get_bit_by_isr(_page_data->msa_writable_mask, OFFSET_INDEX(offset)))
		{
			_page_data->ptr[OFFSET_INDEX(offset)] = value;
			_msa_page_lower_control_drv_mask(OFFSET_INDEX(offset));

			if((OFFSET_INDEX(offset) >= MSA_OFFSET_LOWER_PSW_ENTRY_AREA_BYTE123) && (OFFSET_INDEX(offset) <= MSA_OFFSET_LOWER_PSW_ENTRY_AREA_BYTE126))
			{
				MSA_page_lower_check_passwd();
			}
		}
	}

	return GOS_OK;
}

static void _msa_page_lower_update_ddmi()
{
    MSA_PAGE_LOWER_MON_T 	    *mon = (MSA_PAGE_LOWER_MON_T *)&_page_data->ptr[MSA_OFFSET_LOWER_TEMP_MSB];
	MSA_PAGE_LOWER_MON_T        *mon_tmp = _old_mon;
	MSA_PAGE3_MASK_T 			*page3_mask = MSA_p3_get_mask();
	MSA_PAGE3_DDMI_THRESHOLD_T  *threshold  = MSA_p3_get_ddmi_threshold();
	uint8_t	i = 0;
	static uint8_t prev_tx_state = 0;

	if(ASSERT(mon) || ASSERT(mon_tmp) || ASSERT(page3_mask) || ASSERT(threshold))
	{
		return;
	}

	//update hw ddmi before update msa ddmi
	if(GOS_OK != MSA_p4_update_hw_ddmi())
	{
		return;
	}

	mon_tmp->temp = DDM_GET_TEMP(MSA_DRV_VAL_NORM);
	mon_tmp->vcc  = DDM_GET_VCC(MSA_DRV_VAL_NORM);

	for (i = 0; i < MSA_CHANNEL_NUM; i++)
	{
		mon_tmp->rssi[i] 	= DDM_GET_RX_POWER(i, MSA_DRV_VAL_NORM);
		mon_tmp->txbias[i] 	= DDM_GET_BIAS(i, MSA_DRV_VAL_NORM);
		mon_tmp->txpower[i] = DDM_GET_TX_POWER(i, MSA_DRV_VAL_NORM);
	}

	COM_LOCK_SEM(_sem);
	memcpy(mon, &_old_mon, sizeof(_old_mon));
	COM_UNLOCK_SEM(_sem);

	// Temperature Alarm/Warning
	if (mon->temp > (int16_t)threshold->temp.hi_alarm)
		_isr_flag->temp |= MSA_PAGE_LOWER_HIGH_ALARM_TEMP_MASK;
	if(mon->temp > (int16_t)threshold->temp.hi_warn)
		_isr_flag->temp |= MSA_PAGE_LOWER_HIGH_WARN_TEMP_MASK;
	if(mon->temp < (int16_t)threshold->temp.lo_alarm)
		_isr_flag->temp |= MSA_PAGE_LOWER_LOW_ALARM_TEMP_MASK;
	if(mon->temp < (int16_t)threshold->temp.lo_warn)
		_isr_flag->temp |= MSA_PAGE_LOWER_LOW_WARN_TEMP_MASK;

	// VCC Alarm/Warning
	if (mon->vcc > threshold->vcc.hi_alarm)
		_isr_flag->vcc |= MSA_PAGE_LOWER_HIGH_ALARM_VCC_MASK;
	if(mon->vcc > threshold->vcc.hi_warn)
		_isr_flag->vcc |= MSA_PAGE_LOWER_HIGH_WARN_VCC_MASK;
	if(mon->vcc < threshold->vcc.lo_alarm)
		_isr_flag->vcc |= MSA_PAGE_LOWER_LOW_ALARM_VCC_MASK;
	if(mon->vcc < threshold->vcc.lo_warn)
		_isr_flag->vcc |= MSA_PAGE_LOWER_LOW_WARN_VCC_MASK;

	if ((prev_tx_state && (!TX_DIS)) || (TX_DIS))  // from lower power mode change to high power mode, there is no need to update interrupt flag
	{
		if(TX_DIS)
		{
			prev_tx_state = TX_DIS ? 1 : 0;
		}
		else
		{
			prev_tx_state = 0;  //from lower power mode change to high power mode, there is need to reset prev_tx_state
		}
		return; //notice:there must be to write code "return" in order to avoid next action make tx_dis = 0
	}

	// RX Power of 4 channels
	for (i = 0; i < MSA_CHANNEL_NUM; i++)
	{
		if (mon->rssi[i] >  threshold->rxpower.hi_alarm)
			_isr_flag->rx_power |= MSA_PAGE_LOWER_HIGH_ALARM_RX_POWER_MASK(GET_OFFSET_MASK(i));
		if(mon->rssi[i] <  threshold->rxpower.lo_alarm)
			_isr_flag->rx_power |= MSA_PAGE_LOWER_LOW_ALARM_RX_POWER_MASK(GET_OFFSET_MASK(i));
		if(mon->rssi[i] > threshold->rxpower.hi_warn)
			_isr_flag->rx_power |= MSA_PAGE_LOWER_HIGH_WARN_RX_POWER_MASK(GET_OFFSET_MASK(i));
		if(mon->rssi[i] < threshold->rxpower.lo_warn)
			_isr_flag->rx_power |= MSA_PAGE_LOWER_LOW_WARN_RX_POWER_MASK(GET_OFFSET_MASK(i));
	}

	// TX Bias of 4 channels
	for (i = 0; i < MSA_CHANNEL_NUM; i++)
	{
		if (mon->txbias[i] > threshold->txbias.hi_alarm)
			_isr_flag->tx_bias |= MSA_PAGE_LOWER_HIGH_ALARM_TX_BIAS_MASK(GET_OFFSET_MASK(i));
		if(mon->txbias[i] < threshold->txbias.lo_alarm)
			_isr_flag->tx_bias |= MSA_PAGE_LOWER_LOW_ALARM_TX_BIAS_MASK(GET_OFFSET_MASK(i));
		if(mon->txbias[i] > threshold->txbias.hi_warn)
			_isr_flag->tx_bias |= MSA_PAGE_LOWER_HIGH_WARN_TX_BIAS_MASK(GET_OFFSET_MASK(i));
		if(mon->txbias[i] < threshold->txbias.lo_warn)
			_isr_flag->tx_bias |= MSA_PAGE_LOWER_LOW_WARN_TX_BIAS_MASK(GET_OFFSET_MASK(i));
	}

	// TX Power of 4 channels
	for (i = 0; i < MSA_CHANNEL_NUM; i++)
	{
		if (mon->txpower[i] > threshold->txpower.hi_alarm)
			_isr_flag->tx_power |= MSA_PAGE_LOWER_HIGH_ALARM_TX_POWER_MASK(GET_OFFSET_MASK(i));
		if(mon->txpower[i] < threshold->txpower.lo_alarm)
			_isr_flag->tx_power |= MSA_PAGE_LOWER_LOW_ALARM_TX_POWER_MASK(GET_OFFSET_MASK(i));
		if(mon->txpower[i] > threshold->txpower.hi_warn)
			_isr_flag->tx_power |= MSA_PAGE_LOWER_HIGH_WARN_TX_POWER_MASK(GET_OFFSET_MASK(i));
		if(mon->txpower[i] < threshold->txpower.lo_warn)
			_isr_flag->tx_power |= MSA_PAGE_LOWER_LOW_WARN_TX_POWER_MASK(GET_OFFSET_MASK(i));
	}
}

void MSA_page_lower_set_low_power_mode()
{
	uint8_t low_power_enable = _page_data->ptr[OFFSET_INDEX(MSA_OFFSET_LOWER_PWR_CTL)];

	if(GET_BIT(low_power_enable, 0) == 0x1)  //power_over_ride bit = 1
	{
		PORT_set_intr_en(PORT_INTR_LPMODE, PORT_INTR_DISABLED);

		if(GET_BIT(low_power_enable, 1) == 0x1)  //power set bit = 1
		{
			SFP_DRV_SET_LOW_POWER(LOWER_POWER_MODE);  //lower power
		}
		else  //power set bit = 0
		{
			SFP_DRV_SET_LOW_POWER(HIGH_POWER_MODE);  //high power
		}
	}
	else
	{
		PORT_set_intr_en(PORT_INTR_LPMODE, PORT_INTR_EANBLED);
	}
}

void MSA_page_lower_on_poll()
{
	_msa_page_lower_update_ddmi();
}

void MSA_page_lower_eeprom_init(void)
{
	flash_write_multi_byte(MSA_FLASH_ADDR_PAGE_LOWER, (uint8_t *)_eeprom_init_data, sizeof(_eeprom_init_data));
}


void MSA_page_lower_load_to_ram()
{
	FLASH_Read((uint8_t *)(&_data_ram[0]), MSA_FLASH_ADDR_PAGE_LOWER, MSA_PAGE_SIZE);
}

void MSA_page_lower_init(QSFP_MSA_PAGE_T *page)
{
	memset(page, 0, sizeof(QSFP_MSA_PAGE_T));
	_page_data = page;

	memset(_data_ram, 0, sizeof(_data_ram));
	page->ptr        			= &_data_ram[0];
	page->len       	 		= sizeof(_data_ram);
	page->page_flash_base 		= MSA_FLASH_ADDR_PAGE_LOWER; // _eeprom_data;
	page->eep_mask				= & _eep_mask[0];
	page->msa_writable_mask     = &_msa_writable_mask[0];
	page->vendor_writable_mask  = &_vendor_writable_mask[0];

	_isr_flag  = (MSA_PAGE_LOWER_ISR_FLAG_T *)&_data_ram[MSA_OFFSET_LOWER_LOS];

	MSA_page_lower_load_to_ram();
}

static void _msa_page_lower_control_drv_mask(uint8_t offset)
{
	if (get_bit_by_isr(_page_data->msa_writable_mask, OFFSET_INDEX(offset)))
	{
		uint16_t drv_mask = MSA_get_control_drv_mask();

		if(OFFSET_INDEX(offset) == OFFSET_INDEX(MSA_OFFSET_LOWER_TX_DISABLE))
		{
			drv_mask |= TX_ENABLE;
		}

		if(OFFSET_INDEX(offset) == OFFSET_INDEX(MSA_OFFSET_LOWER_PWR_CTL))
		{
			drv_mask |= LOW_POWER_MODE;
		}

		if(OFFSET_INDEX(offset) == OFFSET_INDEX(MSA_OFFSET_LOWER_TX_CDR_CTL))
		{
			drv_mask |= TX_RX_CDR_CONTROL;
		}
		MSA_set_control_drv_mask(drv_mask);
	}
}

void MSA_page_lower_set_boot_status(bool enable)
{
	//make data_not_ready = 1, indacate data is not ready
	if(enable == false)
	{
		INTL = 1;  //during power up reset, INTL is always high
		_data_ram[MSA_OFFSET_LOWER_STATUS] |= MSA_PAGE_LOWER_INTL_SET;
		_data_ram[MSA_OFFSET_LOWER_STATUS] |= MSA_LOWER_PAGE_DATA_NOT_READY;
	}
	//make data_not_ready = 0, indacate data is ready
	else
	{
		INTL = 0;  //make INTL = 0 after module is ready
		_data_ram[MSA_OFFSET_LOWER_STATUS] &= MSA_PAGE_LOWER_INTL_CLEAR;
		_data_ram[MSA_OFFSET_LOWER_STATUS] &= MSA_LOWER_PAGE_DATA_READY;
	}
}

uint8_t MSA_page_lower_get_page_select()
{
	if(ASSERT(_page_data))
	{
		return GOS_FAIL;
	}
	return _page_data->ptr[OFFSET_INDEX(MSA_OFFSET_LOWER_PAGE_SELECT_BYTE)];
}

void MSA_page_lower_check_passwd()
{
	uint8_t i = 0;
	const uint8_t  factory_pwd[] = {0x8F,0x9E, 0xAD, 0xEC};
	uint8_t *eep_pwd = (uint8_t *)&_page_data->ptr[OFFSET_INDEX(MSA_OFFSET_LOWER_PSW_CHG_ENTRY_AREA_BYTE119)];
	uint8_t *ram_pwd = (uint8_t *)&_page_data->ptr[OFFSET_INDEX(MSA_OFFSET_LOWER_PSW_ENTRY_AREA_BYTE123)];

	if(memcmp(eep_pwd, ram_pwd, 4) == 0x0)
	{
		_passwd_status |= OEM_PWD_MATCH_OK;
	}
	else
	{
		_passwd_status &= ~OEM_PWD_MATCH_OK;
	}

	if(memcmp(factory_pwd, ram_pwd, 4) == 0x0)
	{
		_passwd_status |= FACTORY_PWD_MATCH_OK;
	}
	else
	{
		_passwd_status &= ~FACTORY_PWD_MATCH_OK;
	}
}

uint8_t MSA_lower_page_get_passwd_status()
{
	return _passwd_status;
}

bool MSA_lower_page_read_ready_intl()
{
	return _read_ready_intl;
}

void MSA_lower_page_update_intl_pin_staus(void)
{
	uint8_t i = 3;

	for(; i < 15; i++)
	{
#if defined(SKYLANE_MODE)
		if (i == 0x5)
		{
			continue;
		}
#endif
		if(_page_data->ptr[i])
		{
			return;
		}
	}
	_read_ready_intl = true;
	INTL = 1;
	_page_data->ptr[MSA_OFFSET_LOWER_STATUS] |= MSA_PAGE_LOWER_INTL_SET;
}


