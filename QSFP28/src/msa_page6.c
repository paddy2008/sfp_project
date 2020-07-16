
#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_i2c_slave.h"
#include "Efm8lb12_smbus.h"
#include "Efm8lb12_100g_com_drv.h"

#ifdef MSA_PAGE_6

static QSFP_MSA_PAGE_T *_pg6;
static QSFP_MSA_PAGE_T *_pg7;
static QSFP_MSA_PAGE_T *_pg8;

static int16_t  _cur_temp = 0;

static 	DDMI_CALI_PARA_T _ddmi_cali;

static SFP_DRV_LASER_CFG_T  _ch_cfg;

static SFP_DRV_LASER_CFG_T _lut_cfg[SFP_DRV_TEMP_MAX][MSA_CHANNEL_NUM];

static RX_CALI_PARA_T  _rx_cali_para;

uint8_t MSA_p67_read_by_isr(uint8_t pg_num, uint8_t offset)
{
	uint8_t value = 0;

	if(MSA_lower_page_get_passwd_status() & FACTORY_PWD_MATCH_OK)  //Vendor Mode:enter factory passward
	{
		if (pg_num == MSA_PAGE_06H)
		{
			value =  FLASH_ByteRead_by_i2c_slave_isr(MSA_FLASH_ADDR_PAGE_06H + OFFSET_INDEX(offset));
		}
		else if (pg_num == MSA_PAGE_07H)
		{
			value =  FLASH_ByteRead_by_i2c_slave_isr(MSA_FLASH_ADDR_PAGE_07H + OFFSET_INDEX(offset));
		}
		else
		{
			value =  FLASH_ByteRead_by_i2c_slave_isr(MSA_FLASH_ADDR_PAGE_08H + OFFSET_INDEX(offset));
		}
	}
	return value;

}

uint8_t MSA_p67_write_by_isr(uint8_t pg_num, uint8_t offset, uint8_t value)
{
	if(MSA_lower_page_get_passwd_status() & FACTORY_PWD_MATCH_OK)  //Vendor Mode:enter factory passward
	{
		pg_num = 0;
		MSA_write_eep(offset, value);
		return GOS_OK;
	}
	return GOS_FAIL;
}

void MSA_p67_on_poll()
{
	uint8_t  ch  = 0;
	MSA_LUT_T 	* laser_stat = MSA_p4_get_laser_state();

	if(ASSERT(laser_stat))
	{
		return;
	}

	if(!MSA_p4_get_auto_poll())
	{
		return;
	}

	// temperature change less than 1 C
	if (abs(laser_stat->temp - _cur_temp) < 0x100)
		return;

	memset(&_ch_cfg, 0, sizeof(_ch_cfg));
	for (ch = 0; ch < MSA_CHANNEL_NUM; ch++)
	{
		SFP_DRV_CH_CALCULATE_CFG(laser_stat->temp, ch, (SFP_DRV_LASER_CFG_T *)&_lut_cfg[0][0], &_ch_cfg);
		laser_stat->ch[ch].config.bias = _ch_cfg.bias;
		laser_stat->ch[ch].config.mod  = _ch_cfg.mod;
		laser_stat->ch[ch].config.cross = _ch_cfg.cross;
		SFP_DRV_ADJUST_LASER_CH(ch, &_ch_cfg);
	}
	_cur_temp = laser_stat->temp;
}

static void _msa_p67_drv_lut_init(SFP_DRV_LASER_CFG_T *cfg, MSA_LUT_T *lut, uint8_t max_ch)
{
	uint8_t i;

	for (i = 0; i < max_ch; i++)
	{
		cfg[i].temp = lut->temp;
		cfg[i].bias = lut->ch[i].config.bias;
		cfg[i].mod  = lut->ch[i].config.mod;
		cfg[i].cross = lut->ch[i].config.cross;
	}
}

static void _msa_p67_lut_init()
{
	MSA_LUT_T lut;
	uint8_t first_section_size = sizeof(_ddmi_cali.temp) + sizeof(_ddmi_cali.vcc) + sizeof(_ddmi_cali.current[0]);

	memset((void *)&lut, 0, sizeof(lut));
	memset(_lut_cfg, 0, sizeof(_lut_cfg));
	memset((void *)&_rx_cali_para, 0, sizeof(_rx_cali_para));

	memcpy((uint8_t *)&lut, (uint8_t *)_pg6->page_flash_base, sizeof(MSA_LUT_T));

	_msa_p67_drv_lut_init(&_lut_cfg[SFP_DRV_TEMP_LOW][0], &lut, MSA_CHANNEL_NUM);

	memcpy((uint8_t *)&lut, (uint8_t *)(((MSA_LUT_T code *)_pg6->page_flash_base)+1), sizeof(MSA_LUT_T));
	_msa_p67_drv_lut_init(&_lut_cfg[SFP_DRV_TEMP_MIDDLE][0], &lut, MSA_CHANNEL_NUM);

	memcpy((uint8_t *)&lut, (uint8_t *)_pg7->page_flash_base, sizeof(MSA_LUT_T));
	_msa_p67_drv_lut_init(&_lut_cfg[SFP_DRV_TEMP_HIGH][0], &lut, MSA_CHANNEL_NUM);

	memcpy((uint8_t *)&_ddmi_cali.temp, (uint8_t *)(((MSA_LUT_T code *)_pg6->page_flash_base)+2), first_section_size);

	memcpy((uint8_t *)&_ddmi_cali.current[1].bias, (uint8_t *)(((MSA_LUT_T code *)_pg7->page_flash_base)+1), sizeof(DDMI_CALI_PARA_T) - first_section_size);

	FLASH_Read((uint8_t *)&_rx_cali_para, (FLADDR)_pg8->page_flash_base, sizeof(_rx_cali_para));
}

uint8_t MSA_p67_set_lut_now(uint8_t channel, MSA_LUT_TYPE type)
{
	MSA_LUT_T lut;
	uint8_t   *lut_base_addr;
	uint8_t    ret = GOS_OK;

	switch(type)
	{
		case MSA_LUT_TYPE_LOW_TEMP:
			lut_base_addr = _pg6->page_flash_base;
			break;
		case MSA_LUT_TYPE_MID_TEMP:
			lut_base_addr = (uint8_t *)(((MSA_LUT_T code *)_pg6->page_flash_base)+1);
			break;
		case MSA_LUT_TYPE_HI_TEMP:
			lut_base_addr = _pg7->page_flash_base;
			break;
		default:
			ret = GOS_FAIL;
			break;
	}

	memset((uint8_t *)&lut, 0, sizeof(MSA_LUT_T));
	memcpy((uint8_t *)&lut, lut_base_addr, sizeof(MSA_LUT_T));

	SFP_DRV_GET_LASER_FULL_STATE(channel, &lut);

	flash_erase_range((uint16_t)lut_base_addr, sizeof(MSA_LUT_T));
	flash_write_multi_byte((uint16_t)lut_base_addr, (uint8_t *)&lut, sizeof(MSA_LUT_T));

	return ret;
}

void MSA_p67_init(QSFP_MSA_PAGE_T *page)
{
	_pg6 = page;
	_pg7 = (QSFP_MSA_PAGE_T *)(page + 1);
	_pg8 = (QSFP_MSA_PAGE_T *)(page + 2);

	_pg6->ptr        	  		= NULL;
	_pg6->len             		= MSA_PAGE_SIZE;
	_pg6->page_flash_base 		= MSA_FLASH_ADDR_PAGE_06H;
	_pg6->eep_mask				= NULL;
	_pg6->msa_writable_mask   	= NULL;
	_pg6->vendor_writable_mask  = NULL;

	_pg7->ptr        	  		= NULL;
	_pg7->len             		= MSA_PAGE_SIZE;
	_pg7->page_flash_base 		= MSA_FLASH_ADDR_PAGE_07H;
	_pg7->eep_mask				= NULL;
	_pg7->msa_writable_mask   	= NULL;
	_pg7->vendor_writable_mask  = NULL;

	_pg8->ptr        	  		= NULL;
	_pg8->len             		= MSA_PAGE_SIZE;
	_pg8->page_flash_base 		= MSA_FLASH_ADDR_PAGE_08H;
	_pg8->eep_mask				= NULL;
	_pg8->msa_writable_mask   	= NULL;
	_pg8->vendor_writable_mask  = NULL;
	_msa_p67_lut_init();
}

DDMI_CALI_PARA_T * MSA_p67_get_ddmi_cali_cfg()
{
	return &_ddmi_cali;
}

RX_CALI_PARA_T * MSA_p67_get_rx_cali_cfg()
{
	return &_rx_cali_para;
}

uint8_t * MSA_p67_get_drv_lut()
{
	return (uint8_t *)_lut_cfg;
}

uint8_t MSA_p67_handler(MSA_ACTION action, uint8_t offset, uint8_t *pValue)
{
	uint8_t ret = 0;
	MSA_WRITE_PARAM_T *param = (MSA_WRITE_PARAM_T *)pValue;

	offset = 0;

	switch(action){
	case MSA_ACTION_DRYRUN_CALI:
	{
		uint8_t  ch = *pValue; 	// data0 is channel number
		SFP_DRV_LASER_CFG_T *cfg = (SFP_DRV_LASER_CFG_T *)(pValue + 2);
		SFP_DRV_CH_CALCULATE_CFG(cfg->temp, ch, (SFP_DRV_LASER_CFG_T *)&_lut_cfg[0][0], cfg);
		break;
	}
	default:
		ret = -1;
		break;
	}
	return ret;
}

void MSA_p67_eeprom_init(void)
{
	const uint8_t buff[MSA_PAGE_SIZE] = {0};
	uint8_t ch = 0;
	RSSI_DARK_THLD cfg;

	for(ch = 0; ch < MSA_CHANNEL_NUM; ch++)
	{
		cfg.rssi_dark_thld[ch].darka = 0x5;
		cfg.rssi_dark_thld[ch].darkd = 0x8;
	}

	flash_write_multi_byte((FLADDR)MSA_FLASH_ADDR_PAGE_06H, &buff[0], MSA_PAGE_SIZE);
	flash_write_multi_byte((FLADDR)MSA_FLASH_ADDR_PAGE_07H, &buff[0], MSA_PAGE_SIZE);
	flash_write_multi_byte((FLADDR)MSA_FLASH_ADDR_PAGE_08H, &buff[0], MSA_PAGE_SIZE);
	flash_write_multi_byte((FLADDR)MSA_FLASH_ADDR_PAGE_08H + OFFSET_INDEX(MSA_P8_RSSI0_DARKA_THLD_MSB), (uint8_t*)&cfg, sizeof(RSSI_DARK_THLD));
}

/*This function is called to resume init when lower power is realized by power off cdr chip*/
void MSA_p67_resume_normal_mode()
{
	CHIP_INIT();  //cdr init again
	MSA_set_control_drv_mask(MSA_CONTROL_BYTE_MASK); //update msa control byte mask
	MSA_control_byte_poll();                         //update msa control byte into cdr chip
}

bool MSA_p67_check_apc_lut_tab(SFP_DRV_LASER_CFG_T *lut)
{
	uint8_t i = 0;
	int16_t *ptr = (int16_t *)lut;

	for(i = 0; i < (sizeof(SFP_DRV_LASER_CFG_T) * 3 * 4) / 2; i++)  //sizeof(SFP_DRV_LASER_CFG_T) * 3 * 4) / 2 is the num of the lut tab all elements
	{
		if((i % 4) == 3 || (i % 4 == 0))  //because temp and crossing may be equal to -1,so skip temp and crossing check
		{
			ptr++;
			continue;
		}

		if((*ptr & 0xFFFF) == 0xFFFF)
		{
			return false;
		}
		ptr++;
	}
	return true;
}


#endif
