#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_i2c_slave.h"
#include "Efm8lb12_smbus.h"
#include "Efm8lb12_100g_com_drv.h"
#include "Efm8lb12_task_sched.h"

#ifdef MSA_PAGE_4

extern volatile SOFT_TIMER_T soft_timer;

static QSFP_MSA_PAGE_T 		*_pg4  = NULL;
static MSA_LUT_T			*_stat = NULL;
static uint8_t _data[MSA_PAGE_SIZE] = { 0 };


static code uint8_t _writable_mask[MSA_PAGE_MASK_LEN] =
{
	0xFF, 0xFF,
	0xFF, 0xFF,
	0xFF, 0xFF,
	0xFF, 0xFF,
	0xFF, 0xFF,
	0xFF, 0xFF,
	0xFF, 0xFF,
	0xFF, 0xFF,
};


static void _msa_p4_get_pin_status()
{
	uint8_t *value = &_data[OFFSET_INDEX(MSA_PAGE4_PIN_STATUS)];
	uint8_t  pin_index = 0, pin_status[8] = {0};

	pin_status[0] = MODSEL;

#if defined(EFM8LB12E64_100G_SFP_AOC_101) || defined(EFM8LB12E64_100G_SFP_CWDM4_202) || \
    defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_LR4_301)
	pin_status[1] = RESET;
#endif

	pin_status[2] = LPMODE;
	pin_status[3] = TX_DIS;
	pin_status[4] = INTL;

#if defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	pin_status[5] = P1V8_EN;
#else
	pin_status[5] = TX3V3_EN;
#endif

#if defined(EFM8LB12E64_100G_SFP_CWDM4_207)
	pin_status[6] = RX3V3_EN;
#endif

	pin_status[7] = 0;

#if defined(EFM8LB12E64_100G_SFP_LR4_301)
	pin_status[6] = TEC_EN;

	#if defined(HW_VER_AM1_301) || defined(HW_VER_AM2_301) || defined(HW_VER_AM3_301)
	pin_status[7] = TX1V8_EN;
	#endif
#endif


	for(pin_index = 0; pin_index < 8; pin_index++)
	{
		*value  = CLEAR_BIT(*value, pin_index);
		*value = *value | (pin_status[pin_index] << pin_index);
	}
}

static void _msa_p4_set_pin_status(uint8_t value)
{
	uint8_t  pin_index = 0, pin_status[8];

	for(pin_index = 0; pin_index < 8; pin_index++)
	{
		pin_status[pin_index] = GET_BIT(value, pin_index);
	}

	//MODSEL 	 = pin_status[0];
	//RESET  	 = pin_status[1] ;
	//LPMODE 	 = pin_status[2];
	TX_DIS 	 = GET_BIT(pin_status[3], 0) ? 1 : 0;
	//INTL   	 = pin_status[4];
#if defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	P1V8_EN = GET_BIT(pin_status[5], 0) ? 1 : 0;
#else
	TX3V3_EN = GET_BIT(pin_status[5], 0) ? 1 : 0;
#endif


#ifdef EFM8LB12E64_100G_SFP_LR4_301
	TEC_EN    = GET_BIT(pin_status[6], 0) ? 1 : 0;

	#if defined(HW_VER_AM1_301) || defined(HW_VER_AM2_301) || defined(HW_VER_AM3_301)
		TX1V8_EN  = GET_BIT(pin_status[7], 0) ? 1 : 0;
	#endif
#endif

#ifdef EFM8LB12E64_100G_SFP_CWDM4_207
	RX3V3_EN    = GET_BIT(pin_status[6], 0) ? 1 : 0;
#endif
}

static void _msa_p4_get_reset_reason()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE   = 0x0;
	_data[OFFSET_INDEX(MSA_PAGE4_MCU_RESET_REASON)] = RSTSRC;
    SFRPAGE = SFRPAGE_SAVE;

    if(GET_BIT(_data[OFFSET_INDEX(MSA_PAGE4_MCU_RESET_REASON)],4) != 0x1)  //if there is Software Reset Force and Flag, not start temp slow boot
    {
    	TIMER_soft_start(&soft_timer.slow_boot_timer);
    }
}

uint8_t MSA_p4_read_by_isr(uint8_t offset)
{
	if(MSA_lower_page_get_passwd_status() & FACTORY_PWD_MATCH_OK)  //Vendor Mode:enter factory passward)
	{
		return _data[OFFSET_INDEX(offset)];
	}
	return 0;
}


uint8_t MSA_p4_write_by_isr(uint8_t offset, uint8_t value)
{
	if(MSA_lower_page_get_passwd_status() & FACTORY_PWD_MATCH_OK)  //Vendor Mode:enter factory passward
	{
		_data[OFFSET_INDEX(offset)] = value;

		if(offset == MSA_PAGE4_PIN_STATUS)
		{
			_msa_p4_set_pin_status(value);
		}
		else if((offset == MSA_PAGE4_DEBUG_CMD) && (value != MSA_ACTION_IDEL))
		{
			TASK_fast_sched(TASK_ID_DEBUG_CMD_POLL);
		}
		return GOS_OK;
	}
	return GOS_FAIL;
}


static void _msa_4_eep_erase()  //erase all eep info
{
	uint8_t buff[2] = {0};

	FLASH_Read((uint8_t *)(&buff[0]), MSA_FLASH_ADDR_PAGE_LOWER, 2);

	if((buff[0] == 0xFF) && (buff[1] == 0xFF))
	{
		MSA_page_lower_eeprom_init();
		MSA_p0_eeprom_init();
		MSA_p1_eeprom_init();
		MSA_p2_eeprom_init();
		MSA_p3_eeprom_init();
		MSA_p5_eeprom_init();
		MSA_p67_eeprom_init();
		MSA_p9_eeprom_init();
#ifdef MSA_PAGE_10
		MSA_p10_eeprom_init();
#endif
	}
}

void MSA_p4_debug_cmd_on_poll(void)
{
	uint8_t  ret = GOS_OK;

	if (_data[OFFSET_INDEX(MSA_PAGE4_DEBUG_CMD)] != MSA_ACTION_IDEL &&
		_data[OFFSET_INDEX(MSA_PAGE4_DEBUG_CMD)] != GOS_OK &&
		_data[OFFSET_INDEX(MSA_PAGE4_DEBUG_CMD)] != GOS_FAIL)
	{
		uint8_t dev_addr = _data[OFFSET_INDEX(MSA_PAGE4_DEBUG_DEV_BASE)];
		uint8_t page_num = (_data[OFFSET_INDEX(MSA_PAGE4_DEBUG_PAGE_NUM)] >> 4);
		uint8_t cmd      = _data[OFFSET_INDEX(MSA_PAGE4_DEBUG_CMD)];
		uint16_t offset  = _data[OFFSET_INDEX(MSA_PAGE4_DEBUG_DEV_OFFSET)];
		uint8_t len      = ((_data[OFFSET_INDEX(MSA_PAGE4_DEBUG_CMD_DATA_LEN)] & MSA_PAGE4_DEBUG_CMD_LEN_MASK) >> 4) + 1;
		uint8_t *buf	 = &_data[OFFSET_INDEX(MSA_PAGE4_DEBUG_DATA0)];

		switch(cmd) // read
		{
			case MSA_ACTION_READ_BYTE:
			{
				if (dev_addr == MCU_SLAVE_ADDR) {
					switch(page_num){
						case MSA_PAGE_00H:
							memcpy(buf, (void *)(MSA_FLASH_ADDR_PAGE_00H + OFFSET_INDEX(offset)), len);
							break;
						case MSA_PAGE_06H:
						{
							uint8_t *drv_lut = MSA_p67_get_drv_lut();
							memcpy(buf, (void *)(drv_lut + OFFSET_INDEX(offset)), len);
							break;
						}
						default:
							buf[0] = INVALID_CHAR;
							break;
					}
				}
				else
				{
					offset |= ((_data[OFFSET_INDEX(MSA_PAGE4_DEBUG_PAGE_NUM)] & MSA_PAGE4_DEBUG_OFFSET_MASK) << 8);
					ret = SMB_read_multi_byte(dev_addr, offset, buf, len);
				}
				break;
			}
			case MSA_ACTION_WRITE_BYTE: 	// write
			{
				if (dev_addr == MCU_SLAVE_ADDR) {

				}
				else
				{
					offset |= ((_data[OFFSET_INDEX(MSA_PAGE4_DEBUG_PAGE_NUM)] & MSA_PAGE4_DEBUG_OFFSET_MASK) << 8);	
					ret = SMB_write_multi_byte(dev_addr, offset, buf, len);
				}
				break;
			}
			case MSA_ACTION_DEBUG_LOG:
			{
			#if defined(SUPPORT_LOG_FUNC)
				MSA_p2_handler(MSA_ACTION_DEBUG_LOG, 0, &_data[OFFSET_INDEX(MSA_PAGE4_DEBUG_DATA0)]);
			#endif
				break;
			}
			case MSA_ACTION_START_POLL:
				_data[OFFSET_INDEX(MSA_PAGE4_LUT_TAB_POLL)] = true;
				break;
			case MSA_ACTION_STOP_POLL:
				_data[OFFSET_INDEX(MSA_PAGE4_LUT_TAB_POLL)] = false;
				break;
			case MSA_ACTION_DRYRUN_CALI:
			{
				MSA_p67_handler(MSA_ACTION_DRYRUN_CALI, 0, &_data[OFFSET_INDEX(MSA_PAGE4_DEBUG_DATA0)]);
				break;
			}
			case MSA_ACTION_ENABLE_CONSOLE:
			{
				//SFP_DRV_SET_CONSOLE(buf[0]);
				break;
			}
			case MSA_ACTION_JUMP_TO_BOOT:
			{
				*((uint8_t SI_SEG_DATA *)0x00) = 0xA5;
				RSTSRC = RSTSRC_SWRSF__SET | RSTSRC_PORSF__SET;
				break;
			}
			case MSA_ACTION_START_DDMI:
				_data[OFFSET_INDEX(MSA_PAGE4_DDMI_POLL)] = true;
				break;
			case MSA_ACTION_STOP_DDMI:
				_data[OFFSET_INDEX(MSA_PAGE4_DDMI_POLL)] = false;
				break;
			case MSA_ACTION_RESET:
				CHIP_RESET();			//reset txcdr,rxcdr
				RSTSRC |= 0x10; 		//Writing a 1 to fourth bit forces a system reset
				break;
			case MSA_ACTION_INIT_EEP_REG:
				MSA_p67_eeprom_init();
				break;
#ifdef MSA_PAGE_10
			case MSA_ACTION_SET_EEP_REG:
			{
				MSA_REG_SET_T *reg = (MSA_REG_SET_T *)&_data[OFFSET_INDEX(MSA_PAGE4_DEBUG_DATA0)];
				MSA_p10_regdata_set(reg);
				break;
			}
#endif
			case MSA_ACTION_SET_LUT:
			{
				ret = MSA_p67_set_lut_now(buf[1], buf[0]);
				break;
			}
#ifdef EFM8LB12E64_100G_SFP_LR4_301
			case MSA_ACTION_TEC_ENABLE:
			{
				LR4_301_set_tec_enable(buf[0]);
				break;
			}
#endif
			case MSA_ACTION_LASER_ON:
			{
				//SFP_DRV_LASER_ON_ALL();
				break;
			}
			case MSA_ACTION_LASER_OFF:
			{
				//SFP_DRV_LASER_OFF_ALL();
				break;
			}
			case MSA_ACTION_FACTORY_RESET:
			{
				MSA_factory_reset();
				break;
			}
			case MSA_ACTION_START_AGING:
			{
				MSA_p9_start_aging();
				break;
			}
			case MSA_ACTION_STOP_AGING:
			{
				MSA_p9_stop_aging();
				break;
			}
			case MSA_ACTION_AGINIG_EEP_INIT:
			{
				//MSA_p5_aging_clear();
				MSA_p9_aging_eep_init();
				break;
			}
			case MSA_ACTION_ENTER_BONDING:
			{
				//SFP_DRV_SET_BONDING_MODE();
				break;
			}
			case MSA_ACTION_EEP_ERASE:
			{
				_msa_4_eep_erase();
				break;
			}
			default:
				break;
		}

		if(ret != GOS_FAIL)
		{
			_data[OFFSET_INDEX(MSA_PAGE4_DEBUG_CMD)] = GOS_OK;
		}
		else
		{
			_data[OFFSET_INDEX(MSA_PAGE4_DEBUG_CMD)] = GOS_FAIL;
		}
	}
}

uint8_t MSA_p4_update_hw_ddmi(void)
{
	uint8_t  channel = 0;

	_stat->temp = DDM_GET_TEMP(MSA_DRV_VAL_HW);
	_stat->vcc  = DDM_GET_VCC(MSA_DRV_VAL_HW);

	if(_data[OFFSET_INDEX(MSA_PAGE4_DDMI_POLL)])
	{
		for (channel = 0; channel< MSA_CHANNEL_NUM; channel++)
		{
			_stat->ch[channel].stat.txbias = DDM_GET_BIAS(channel, MSA_DRV_VAL_HW);
			_stat->ch[channel].stat.txpwr  = DDM_GET_TX_POWER(channel, MSA_DRV_VAL_HW);
			_stat->ch[channel].stat.rssi   = DDM_GET_RX_POWER(channel, MSA_DRV_VAL_HW);
		}
	}
	else
	{
		return GOS_FAIL;
	}

	return GOS_OK;
}

void MSA_p4_on_poll(void)
{
#ifdef SUPPORT_TEC_FUNC
	uint16_t val;
#endif

	_msa_p4_get_pin_status();

#ifdef SUPPORT_TEC_FUNC
	val = LR4_301_get_tec_tempout();
	_data[OFFSET_INDEX(MSA_PAGE4_TEC_TEMP_READBACK_MSB)] = U16_MSB(val);
	_data[OFFSET_INDEX(MSA_PAGE4_TEC_TEMP_READBACK_LSB)] = U16_LSB(val);

	val = LR4_301_get_tec_tempset();
	_data[OFFSET_INDEX(MSA_PAGE4_TEC_TEMP_SET_MSB)] = U16_MSB(val);
	_data[OFFSET_INDEX(MSA_PAGE4_TEC_TEMP_SET_LSB)] = U16_LSB(val);

	val = LR4_301_get_itec();
	_data[OFFSET_INDEX(MSA_PAGE4_ITEC_MSB)] = U16_MSB(val);
	_data[OFFSET_INDEX(MSA_PAGE4_ITEC_LSB)] = U16_LSB(val);

	val = LR4_301_get_vtec();
	_data[OFFSET_INDEX(MSA_PAGE4_VTEC_MSB)] = U16_MSB(val);
	_data[OFFSET_INDEX(MSA_PAGE4_VTEC_LSB)] = U16_LSB(val);	
#endif	
}

static void _msa_4_data_init()
{
	_data[OFFSET_INDEX(MSA_PAGE4_BOOT_STATUS)] 	= 0x1;
	_data[OFFSET_INDEX(MSA_PAGE4_LUT_TAB_POLL)] = 0x1;  //default auto enable
	_data[OFFSET_INDEX(MSA_PAGE4_DDMI_POLL)] 	= true; //default ddmi enable
	_data[OFFSET_INDEX(MSA_PAGE4_BUILD_SECOND)] = get_second(__TIME__);
	_data[OFFSET_INDEX(MSA_PAGE4_BUILD_MINUTE)] = get_minute(__TIME__);
	_data[OFFSET_INDEX(MSA_PAGE4_BUILD_HOUR)] 	= get_hour(__TIME__);
	_data[OFFSET_INDEX(MSA_PAGE4_BUILD_DAY)] 	= get_day(__DATE__);
	_data[OFFSET_INDEX(MSA_PAGE4_BUILD_MONTH)] 	= get_month(__DATE__);
	_data[OFFSET_INDEX(MSA_PAGE4_BUILD_YEAR)] 	= get_year(__DATE__);
	_data[OFFSET_INDEX(MSA_PAGE4_VERSION_HI)] 	= (uint8_t)__VERSION_HI__;
	_data[OFFSET_INDEX(MSA_PAGE4_VERSION_LO)] 	= (uint8_t)__VERSION_LO__;
	_msa_p4_get_reset_reason();
}

uint8_t MSA_p4_get_auto_poll(void)
{
	return _data[OFFSET_INDEX(MSA_PAGE4_LUT_TAB_POLL)];
}

uint8_t MSA_p4_get_ddmi_poll(void)
{
	return _data[OFFSET_INDEX(MSA_PAGE4_DDMI_POLL)];
}

void MSA_p4_init(QSFP_MSA_PAGE_T *page)
{
	_pg4 = page;

	memset(_data, 0, MSA_PAGE_SIZE);
	_pg4->ptr        			= &_data[0];
	_pg4->len        			= sizeof(_data); // MSA_PAGE_SIZE
	_pg4->page_flash_base 		= NULL;
	_pg4->eep_mask				= NULL;
	_pg4->msa_writable_mask   	= &_writable_mask[0];
	_pg4->vendor_writable_mask  = NULL;
	_stat = (MSA_LUT_T *)page->ptr;
	_msa_4_data_init();
}

MSA_LUT_T * MSA_p4_get_laser_state(void)
{
	if(ASSERT(_stat))
	{
		return NULL;
	}
	return _stat;
}

#endif

