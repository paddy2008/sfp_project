/*
 * Efm8lb12_100g_cwdm4_207.c
 *
 *  Created on: 2018-6-19
 *      Author: cuilin
 */
#include <math.h>
#include "Efm8lb12_adc.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_smbus.h"
#include "Efm8lb12_common.h"
#include "Efm8lb12_msa_pin.h"
#include "Efm8lb12_100g_com_drv.h"

#ifdef EFM8LB12E64_100G_SFP_CWDM4_207

static uint8_t _i2c_dev_ready = 0;

static void _cwdm4_207_port0_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;
	
	P0 = P0_B0__HIGH | P0_B1__LOW | P0_B2__HIGH | P0_B3__HIGH | P0_B4__HIGH | P0_B5__LOW | P0_B6__HIGH | P0_B7__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P0MDIN = P0MDIN_B0__ANALOG   					   // Vref
			| P0MDIN_B1__ANALOG                        // GND 
			| P0MDIN_B2__DIGITAL					   // SDA_M
			| P0MDIN_B3__DIGITAL 					   // SCL_M
			| P0MDIN_B4__DIGITAL  					   // MODSel
			| P0MDIN_B5__DIGITAL					   // LPMode
			| P0MDIN_B6__DIGITAL 					   // TXCDR INTR
			| P0MDIN_B7__ANALOG;					   // VCC


	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P0MDOUT = P0MDOUT_B0__OPEN_DRAIN
			| P0MDOUT_B1__OPEN_DRAIN 
			| P0MDOUT_B2__OPEN_DRAIN
			| P0MDOUT_B3__OPEN_DRAIN
			| P0MDOUT_B4__OPEN_DRAIN
			| P0MDOUT_B5__OPEN_DRAIN
			| P0MDOUT_B6__OPEN_DRAIN
			| P0MDOUT_B7__OPEN_DRAIN;


	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P0SKIP = P0SKIP_B0__SKIPPED
			| P0SKIP_B1__SKIPPED
			| P0SKIP_B2__NOT_SKIPPED	// SDA_M
			| P0SKIP_B3__NOT_SKIPPED	// SCL_M
			| P0SKIP_B4__SKIPPED
			| P0SKIP_B5__SKIPPED
			| P0SKIP_B6__SKIPPED
			| P0SKIP_B7__SKIPPED;

	//5. set p0MASK
	P0MASK = P0MASK_B0__IGNORED | P0MASK_B1__IGNORED | P0MASK_B2__IGNORED
			| P0MASK_B3__IGNORED | P0MASK_B4__COMPARED | P0MASK_B5__IGNORED
			| P0MASK_B6__COMPARED | P0MASK_B7__IGNORED;

	//6. set P0MAT
	P0MAT = P0MAT_B0__HIGH | P0MAT_B1__HIGH | P0MAT_B2__HIGH | P0MAT_B3__HIGH
			| P0MAT_B4__HIGH | P0MAT_B5__HIGH | P0MAT_B6__HIGH | P0MAT_B7__HIGH;

    SFRPAGE = SFRPAGE_SAVE;
}

static void _cwdm4_207_port1_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;

	P1 = P1_B0__HIGH | P1_B1__HIGH | P1_B2__HIGH | P1_B3__HIGH | P1_B4__HIGH | P1_B5__HIGH | P1_B6__HIGH | P1_B7__HIGH;

	P1MDIN = P1MDIN_B0__ANALOG		// MPD0
			| P1MDIN_B1__ANALOG		// MPD1
			| P1MDIN_B2__ANALOG		// MPD2
			| P1MDIN_B3__ANALOG		// MPD3
			| P1MDIN_B4__ANALOG		// VCC
			| P1MDIN_B5__ANALOG		// RSSI
			| P1MDIN_B6__DIGITAL	// ETS: External Temperature Sensor
			| P1MDIN_B7__DIGITAL;	// MSA INTL

	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN
			| P1MDOUT_B1__OPEN_DRAIN
			| P1MDOUT_B2__OPEN_DRAIN
			| P1MDOUT_B3__OPEN_DRAIN
			| P1MDOUT_B4__OPEN_DRAIN
			| P1MDOUT_B5__OPEN_DRAIN
			| P1MDOUT_B6__PUSH_PULL
			| P1MDOUT_B7__OPEN_DRAIN;

	P1SKIP = P1SKIP_B0__SKIPPED
			| P1SKIP_B1__SKIPPED
			| P1SKIP_B2__SKIPPED
			| P1SKIP_B3__SKIPPED
			| P1SKIP_B4__SKIPPED
			| P1SKIP_B5__SKIPPED
			| P1SKIP_B6__SKIPPED
			| P1SKIP_B7__SKIPPED;

	//5. set p1MASK
	P1MASK = P1MASK_B0__IGNORED | P1MASK_B1__IGNORED | P1MASK_B2__IGNORED
			| P1MASK_B3__IGNORED | P1MASK_B4__IGNORED | P1MASK_B5__IGNORED
			| P1MASK_B6__IGNORED | P1MASK_B7__IGNORED;

	//6. set P1MAT
	P1MAT = P1MAT_B0__HIGH | P1MAT_B1__HIGH | P1MAT_B2__HIGH | P1MAT_B3__HIGH
			| P1MAT_B4__HIGH | P1MAT_B5__HIGH | P1MAT_B6__HIGH | P1MAT_B7__HIGH;

	SFRPAGE = SFRPAGE_SAVE;
}

static void _cwdm4_207_port2_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE = 0x0;

	P2 = P2_B0__HIGH | P2_B1__HIGH | P2_B2__HIGH | P2_B3__HIGH | P2_B4__HIGH | P2_B5__LOW | P2_B6__HIGH ;

	P2MDIN = P2MDIN_B0__DIGITAL		// SDA_L
			| P2MDIN_B1__DIGITAL	// SCL_L
			| P2MDIN_B2__ANALOG		// Not Used
			| P2MDIN_B3__ANALOG		// Not Used
			| P2MDIN_B4__DIGITAL	// ToSA Reset
			| P2MDIN_B5__DIGITAL	// ToSA Disable
			| P2MDIN_B6__DIGITAL;	// RX Power Enable

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).

	P2MDOUT = P2MDOUT_B0__OPEN_DRAIN
			| P2MDOUT_B1__OPEN_DRAIN
			| P2MDOUT_B2__OPEN_DRAIN
			| P2MDOUT_B3__OPEN_DRAIN
			| P2MDOUT_B4__PUSH_PULL
			| P2MDOUT_B5__PUSH_PULL
			| P2MDOUT_B6__PUSH_PULL;

	P2SKIP = P2SKIP_B0__SKIPPED
			| P2SKIP_B1__SKIPPED
			| P2SKIP_B2__SKIPPED
			| P2SKIP_B3__SKIPPED | 0x10 | 0x20 | 0x40;
//			| P2SKIP_B4__SKIPPED
//			| P2SKIP_B5__SKIPPED
//			| P2SKIP_B6__SKIPPED;

	//5. set p2MASK
	P2MASK = P2MASK_B0__IGNORED | P2MASK_B1__IGNORED | P2MASK_B2__IGNORED
			| P2MASK_B3__IGNORED | P2MASK_B4__IGNORED | P2MASK_B5__IGNORED
			| P2MASK_B6__IGNORED ;

	//6. set P2MAT
	P2MAT = P2MAT_B0__HIGH | P2MAT_B1__HIGH | P2MAT_B2__HIGH | P2MAT_B3__HIGH
			| P2MAT_B4__HIGH | P2MAT_B5__HIGH | P2MAT_B6__HIGH ;

	SFRPAGE = SFRPAGE_SAVE;
}

static void _cwdm4_207_port3_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE = 0x0;

	P3 = P3_B0__HIGH | P3_B1__HIGH | P3_B2__HIGH | P3_B3__HIGH | P3_B4__HIGH;

	P3MDIN = P3MDIN_B0__DIGITAL		// TOSA/ROSA Power level adjust, not used
			| P3MDIN_B1__DIGITAL	// TX CDR LOS INTR
			| P3MDIN_B2__DIGITAL	// TOSA Power Enable
			| P3MDIN_B3__DIGITAL	// TX CDR Disable
			| P3MDIN_B4__DIGITAL;	// Not used

	P3MDOUT = P3MDOUT_B0__PUSH_PULL
			| P3MDOUT_B1__OPEN_DRAIN
			| P3MDOUT_B2__PUSH_PULL
			| P3MDOUT_B3__PUSH_PULL
			| P3MDOUT_B4__OPEN_DRAIN;

	SFRPAGE = SFRPAGE_SAVE;
}

static bool _cwdm4_207_txcdr_check_exist()
{
	uint8_t i = 0, Data = 0;

	/*
	if(TX3V3_EN == 0)
	{
		MSA_p2_log("\r\ntxcdr is lower power mode");
		return false;
	}
	*/

	for(i = 0; i < 3; i++)
	{
		if(GOS_OK == smb_read_byte(CWDM4_207_TXCDR_BASE_ADDR, 0x0, &Data))
		{
			MSA_p2_log("\r\ntxcdr smb read OK");
			return true;
		}
		else
		{
			MSA_p2_log("\r\ntxcdr smb read failed");
		}
		Hw_time5_delay_us(5000);
	}

	return false;
}

static bool _cwdm4_207_rxcdr_check_exist()
{
	uint8_t i = 0, Data = 0;

	/*
	if(TX3V3_EN == 0x0)
	{
		MSA_p2_log("\r\nrxcdr is lower power mode");
		return false;
	}
	*/


	for(i = 0; i < 3; i++)
	{
		if(GOS_OK == smb_read_byte(CWDM4_207_RXCDR_BASE_ADDR, 0x0, &Data))
		{
			MSA_p2_log("\r\nrxcdr smb read OK");
			return true;
		}
		else
		{
			MSA_p2_log("\r\nrxcdr smb read failed");
		}
		Hw_time5_delay_us(5000);
	}

	return false;
}


static bool _cwdm4_207_tosa_check_exist()
{
	uint8_t i = 0, Data = 0;

	/*
	if(TX3V3_EN == 0)
	{
		MSA_p2_log("\r\ntosa is lower power mode");
		return false;
	}
	*/

	for(i = 0; i < 3; i++)
	{
		if(GOS_OK == smb_read_byte(CWDM4_207_TOSA_BASE_ADDR, 0x0, &Data))
		{
			MSA_p2_log("\r\ntosa smb read OK");
			return true;
		}
		else
		{
			MSA_p2_log("\r\ntosa smb read failed");
		}
		Hw_time5_delay_us(5000);
	}
	return false;
}

static uint8_t _cwdm4_207_txcdr_m37049_i2c_read_byte(uint8_t OffsetAddr, uint8_t *Data)
{
	if((_i2c_dev_ready & TX_READY) == 0x0)
	{
		return GOS_FAIL;
	}
	return smb_read_byte(CWDM4_207_TXCDR_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t _cwdm4_207_txcdr_m37049_i2c_write_byte(uint8_t OffsetAddr, uint8_t Data)
{
	if((_i2c_dev_ready & TX_READY) == 0x0)
	{
		return GOS_FAIL;
	}
	return smb_write_byte(CWDM4_207_TXCDR_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t _cwdm4_207_tosa_m4820_i2c_read_byte(uint8_t OffsetAddr, uint8_t *Data)
{
	if((_i2c_dev_ready & TOSA_READY) == 0x0)
	{
		return GOS_FAIL;
	}
	return smb_read_byte(CWDM4_207_TOSA_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t _cwdm4_207_tosa_m4820_i2c_write_byte(uint8_t OffsetAddr,uint8_t Data)
{
	if((_i2c_dev_ready & TOSA_READY) == 0x0)
	{
		return GOS_FAIL;
	}
	return smb_write_byte(CWDM4_207_TOSA_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t _cwdm4_207_rxcdr_m37244_i2c_read_byte(uint8_t OffsetAddr, uint8_t *Data)
{
	if((_i2c_dev_ready & RX_READY) == 0x0)
	{
		return GOS_FAIL;
	}
	return smb_read_byte(CWDM4_207_RXCDR_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t _cwdm4_207_rxcdr_m37244_i2c_write_byte(uint8_t OffsetAddr,uint8_t Data)
{
	if((_i2c_dev_ready & RX_READY) == 0x0)
	{
		return GOS_FAIL;
	}
	return smb_write_byte(CWDM4_207_RXCDR_BASE_ADDR, OffsetAddr, Data);
}


static void _cwdm4_207_txcdr_m37049_reset()
{
	if(_cwdm4_207_txcdr_check_exist())
	{
		_i2c_dev_ready |= TX_READY;
	}

	_cwdm4_207_txcdr_m37049_i2c_write_byte(M3704X_RESET,0xaa);
}
static void _cwdm4_207_tosa_m4820_reset()
{
	if(_cwdm4_207_tosa_check_exist())
	{
		_i2c_dev_ready |= TOSA_READY;
	}

	_cwdm4_207_tosa_m4820_i2c_write_byte(0x2, 0x1);  	 //tosa reset

}

static void _cwdm4_207_rxcdr_m37244_reset()
{
	if(_cwdm4_207_rxcdr_check_exist())
	{
		_i2c_dev_ready |= RX_READY;
	}

	_cwdm4_207_rxcdr_m37244_i2c_write_byte(0x02, 0xaa);
	Soft_timer_delay_ms(100);
	_cwdm4_207_rxcdr_m37244_i2c_write_byte(0x02, 0x00);
	_cwdm4_207_rxcdr_m37244_i2c_write_byte(0x03, 0xf0);
}

static void _cwdm4_207_txcdr_m37049_init()
{
	_cwdm4_207_txcdr_m37049_reset();
	_cwdm4_207_txcdr_m37049_i2c_write_byte(M3704X_MODE_CH(2),0x04);
	_cwdm4_207_txcdr_m37049_i2c_write_byte(M3704X_MODE_CH(3),0x04);
//	_cwdm4_207_txcdr_m37049_i2c_write_byte(M3704X_OUTPUT_DEEMPH,0x58);
}

static uint8_t _cwdm4_207_tasa_m4820_value(uint8_t ch,SFP_DRV_LASER_CFG_T *cfg)
{
	uint8_t page;
	static code uint8_t mod_reg[][2]   = {{0x05,TOSA_PAGEN_VO1},{0x05,TOSA_PAGEN_VO2},{0x07,TOSA_PAGEN_VO1},{0x07,TOSA_PAGEN_VO2}};
	static code uint8_t cross_reg[][2] = {{0x04,TOSA_PAGEN_VO1},{0x04,TOSA_PAGEN_VO2},{0x06,TOSA_PAGEN_VO1},{0x06,TOSA_PAGEN_VO2}};


	_cwdm4_207_tosa_m4820_i2c_read_byte(TOSA_PAGE0_INDIRECT_PAGE,(uint8_t*)&page);

	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,ch+4);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGEN_GAIN_ADJUST,(uint8_t)cfg->bias);   //VB

	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,mod_reg[ch][0]);
	_cwdm4_207_tosa_m4820_i2c_write_byte(mod_reg[ch][1],(uint8_t)cfg->mod);			//VC

	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,cross_reg[ch][0]);
	_cwdm4_207_tosa_m4820_i2c_write_byte(cross_reg[ch][1],(uint8_t)cfg->cross);		//VX

	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,page);

	return GOS_OK;
}

static void _cwdm4_207_tosa_m4820_init()
{
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,0x00);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_RESET,0x01);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_RESET,0x00);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,0x04);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGEN_AGC_ENABLE,0x00);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,0x05);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGEN_AGC_ENABLE,0x00);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,0x06);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGEN_AGC_ENABLE,0x00);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,0x07);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGEN_AGC_ENABLE,0x00);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,0x00);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_CHX_SHUTDOW,0xff);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_ADC_ENABLE_II,0xe0);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_ADC_ENABLE_II+1,0x01);
}

static void _cwdm4_207_rxcdr_m37244_init()
{
	_cwdm4_207_rxcdr_m37244_i2c_write_byte(0x03,0xf0);
	_cwdm4_207_rxcdr_m37244_i2c_write_byte(0x19,0xff);
	_cwdm4_207_rxcdr_m37244_i2c_write_byte(0x1b,0x03);
	_cwdm4_207_rxcdr_m37244_i2c_write_byte(0x23,0xf0);
	_cwdm4_207_rxcdr_m37244_i2c_write_byte(0x40,0xf0);
	_cwdm4_207_rxcdr_m37244_i2c_write_byte(0x42,0x1e);
	_cwdm4_207_rxcdr_m37244_i2c_write_byte(0x43,0x1e);
	_cwdm4_207_rxcdr_m37244_i2c_write_byte(0x44,0x1e);
	_cwdm4_207_rxcdr_m37244_i2c_write_byte(0x45,0x1e);
}


void CWDM4_207_chip_reset()
{
	_cwdm4_207_txcdr_m37049_reset();
	_cwdm4_207_tosa_m4820_reset();
	_cwdm4_207_rxcdr_m37244_reset();
}

void CWDM4_207_chip_init()
{
	CWDM4_207_chip_reset();
	_cwdm4_207_txcdr_m37049_init();
	_cwdm4_207_tosa_m4820_init();
	_cwdm4_207_rxcdr_m37244_init();
}

void CWDM4_207_port_init()
{
	_cwdm4_207_port0_init();
	_cwdm4_207_port1_init();
	_cwdm4_207_port2_init();
	_cwdm4_207_port3_init();
}

int16_t  CWDM4_207_ddmi_get_temp(MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		DDMI_CALI_PARA_T *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		return ((int16_t)((float)(ddmi_cali->temp) * Timer_temp_slow_boot_cal()) + ADC0_get_mcu_temp());  		//MSA format uint: 1/256 C
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return  ADC0_get_mcu_temp();  //uint: 1/256 C
	}

	return GOS_FAIL;

}

uint16_t CWDM4_207_ddmi_get_vcc(MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		DDMI_CALI_PARA_T *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		return (ddmi_cali->vcc + ADC0_get_vcc()); //uint:0.1mv
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return ADC0_get_vcc(); //uint:0.1mv
	}
	return 0;
}

static uint16_t _cwdm4_207_get_bias_average_value()
{
	uint16_t buff16[AVERAGE_MAX_COUNTER] = {0};
	uint8_t i = 0, value_h = 0, value_l = 0;

	for(i = 0; i < AVERAGE_MAX_COUNTER; i++)
	{
		_cwdm4_207_tosa_m4820_i2c_read_byte(TOSA_PAGEN_EXTERNAL_SOURCE_II,(uint8_t*)&value_l);
		_cwdm4_207_tosa_m4820_i2c_read_byte(TOSA_PAGEN_EXTERNAL_SOURCE_II+1,(uint8_t*)&value_h);
		buff16[i] = value_l | (value_h << 8);
	}

	return com_average_value16(buff16, AVERAGE_MAX_COUNTER);
}

static uint16_t _cwdm4_207_get_ddm_bias(uint8_t channel)
{
	uint16_t  value16 = 0;
	uint8_t page;

	_cwdm4_207_tosa_m4820_i2c_read_byte(TOSA_PAGE0_INDIRECT_PAGE,(uint8_t*)&page);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,channel+4);
	value16 = _cwdm4_207_get_bias_average_value();
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,page);
	return (uint16_t)((float)value16 * 0.0568);
}

uint16_t CWDM4_207_ddmi_get_bias(uint8_t channel, MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		uint16_t bias = 0;
		float bias_temp_cali = 0.0;
		const DDMI_CALI_PARA_T *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		const MSA_LUT_T	 *laser_stat = MSA_p4_get_laser_state();
		const SFP_DRV_LASER_CFG_T *lo_cfg = (SFP_DRV_LASER_CFG_T *)MSA_p67_get_drv_lut();
		const SFP_DRV_LASER_CFG_T *m_cfg  = (lo_cfg + MSA_CHANNEL_NUM);

		/*
		if(MSA_p67_check_apc_lut_tab(lo_cfg))  //apc paramters is invalid, so there is no need to apc calibration
		{
			const int16_t cur_temp = ADC0_get_mcu_temp();

			if(cur_temp <= (m_cfg + channel)->temp)  //low temp
			{
				bias_temp_cali = (float)(cur_temp - m_cfg->temp) / (float)TEMP_SCALE * 0.1;
			}
			else  //high temp
			{
				bias_temp_cali = (float)(cur_temp - m_cfg->temp) / (float)TEMP_SCALE * 0.56;
			}
		}
		*/

		bias  = (uint16_t)(((float)laser_stat->ch[channel].stat.txbias + ddmi_cali->current[channel].bias.bias_offset2  - ddmi_cali->current[channel].bias.bias_offset1 + bias_temp_cali) * 0.1 * 1000.0 * 0.5); //uint:2uA
		return  bias;//uint:2uA
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return (uint16_t)((_cwdm4_207_get_ddm_bias(channel)) *10);  //0.1mA
	}
	return GOS_FAIL;
}

static uint16_t _cwdm4_207_get_rssi_average_value()
{
	uint8_t i = 0;
	uint16_t buff16[AVERAGE_MAX_COUNTER] = {0};

	for(i = 0; i < AVERAGE_MAX_COUNTER; i++)
	{
		buff16[i] = ADC0_manual_poll(ADC0_RSSI0) * 0.1;   // uint:mv
		Hw_time5_delay_us(2000);
	}

	return com_average_value16(buff16, AVERAGE_MAX_COUNTER);
}

static uint16_t cwdm4_207_ddmi_get_rssi(uint8_t channel)
{
	uint8_t value = 0, i = 0;
	uint16_t rssi_mv = 0;

	channel = 3 - channel;  //rxcdr pin is inverted sequence with qsfp28 pin

	cwdm4_207_rxcdr_m37244_i2c_read_byte(0x3, &value);
	value = SET_BIT(value,6);  //rssi enable
	if(channel == MSA_PWR_CHANNEL_1)
	{
		value = CLEAR_BIT(value,4);
		value = CLEAR_BIT(value,5);
	}
	else if(channel == MSA_PWR_CHANNEL_2)
	{
		value = SET_BIT(value,4);
		value = CLEAR_BIT(value,5);
	}
	else if(channel == MSA_PWR_CHANNEL_3)
	{
		value = CLEAR_BIT(value,4);
		value = SET_BIT(value,5);
	}
	else if(channel == MSA_PWR_CHANNEL_4)
	{
		value = SET_BIT(value,4);
		value = SET_BIT(value,5);
	}

	_cwdm4_207_rxcdr_m37244_i2c_write_byte(0x3, value);

	rssi_mv = _cwdm4_207_get_rssi_average_value();

	return (uint16_t) ((float)rssi_mv / (float)CWDM4_207_RSSI_SERIES_RES * 1000.00);
}


uint16_t CWDM4_207_ddmi_get_rx_power(uint8_t channel, MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		float rxpower_result = 0.0 ,rxpower1 = 0.0, rxpower2 = 0.0, rssi1 = 0.0, rssi2 = 0.0;
		const DDMI_CALI_PARA_T *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		const MSA_LUT_T 		*laser_stat = MSA_p4_get_laser_state();

		//ddmi_cali->tx_rx_pwr[channel].rxpower.rxpower1 uint:0.1uw
		rxpower1 = (float)ddmi_cali->current[channel].rxpower.rxpower1 * 0.1 / 1000.0;  //uint:mW
		rxpower2 = (float)ddmi_cali->current[channel].rxpower.rxpower2 * 0.1 / 1000.0;  //uint:mW

		//ddmi_cali->tx_rx_pwr[channel].rxpower.rssi1 uint:uA
		rssi1 	 = (float)ddmi_cali->current[channel].rxpower.rssi1;
		rssi2 	 = (float)ddmi_cali->current[channel].rxpower.rssi2;

		if(rssi2 != rssi1)
		{
			  rxpower_result = rxpower1 + ((float)laser_stat->ch[channel].stat.rssi - rssi1) * ((rxpower2 - rxpower1) / (rssi2 - rssi1));  //uint: mW
		}

		CHECK_POWER_MIN(rxpower_result);
		CHECK_POWER_MAX(rxpower_result);
		return (uint16_t)(rxpower_result * 10.0 * 1000.0);  //MSA format uint:0.1uW
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return  (uint16_t)cwdm4_207_ddmi_get_rssi(channel);  //uint:ua
	}

	return GOS_FAIL;
}

uint16_t CWDM4_207_ddmi_get_tx_power(uint8_t channel, MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		float  txmon_N = 0.0;
		float txpower_result = 0.0, txpower1 = 0.0, txpower2 = 0.0, bias1 = 0.0, bias2 = 0.0;
		const DDMI_CALI_PARA_T    *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		const MSA_LUT_T 		  *laser_stat = MSA_p4_get_laser_state();

		txmon_N =(float)laser_stat->ch[channel].stat.txpwr*0.1 ;

		//ddmi_cali->tx_rx_pwr[channel].txpower.txpower uint:0.1uw
		txpower1 = (float)ddmi_cali->current[channel].txpower.txpower1 * 0.1 / 1000.0; //uint:mW
		txpower2 = (float)ddmi_cali->current[channel].txpower.txpower2 * 0.1 / 1000.0; //uint:mW

		//ddmi_cali->tx_rx_pwr[channel].txpower.bias uint:0.1mA
		bias1 = (float)ddmi_cali->current[channel].txpower.bias1 * 0.1; //uint:mA
		bias2 = (float)ddmi_cali->current[channel].txpower.bias2 * 0.1; //uint:mA

		if(bias1 != bias2)
		{
			txpower_result = txpower1 + (txpower1 - txpower2)/(bias1 - bias2) * (txmon_N - bias1);  //uint:mW
		}

		CHECK_POWER_MIN(txpower_result);
		CHECK_POWER_MAX(txpower_result);
		return (uint16_t)(txpower_result * 1000.0 * 10.0); //MSA format uint:0.1uW
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return ADC0_get_mpd(channel); // uint:0¨¦¨C??¡ª??¡¤mA
	}
	return 0;
}

void CWDM4_207_drv_adjust_laser_ch(uint8_t ch, SFP_DRV_LASER_CFG_T *cfg)
{
	_cwdm4_207_tasa_m4820_value(ch,cfg);
}

void CWDM4_207_drv_get_laser_full_state(uint8_t ch, MSA_LUT_T *lut)
{

	uint8_t page,bias, mod, cross;
	static code uint8_t mod_reg[][2]   = {{0x05,TOSA_PAGEN_VO1},{0x05,TOSA_PAGEN_VO2},{0x07,TOSA_PAGEN_VO1},{0x07,TOSA_PAGEN_VO2}};
	static code uint8_t cross_reg[][2] = {{0x04,TOSA_PAGEN_VO1},{0x04,TOSA_PAGEN_VO2},{0x06,TOSA_PAGEN_VO1},{0x06,TOSA_PAGEN_VO2}};

	lut->temp = DDM_GET_TEMP(MSA_DRV_VAL_HW);
	_cwdm4_207_tosa_m4820_i2c_read_byte(TOSA_PAGE0_INDIRECT_PAGE,(uint8_t*)&page);

	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,ch + 4);
	_cwdm4_207_tosa_m4820_i2c_read_byte(TOSA_PAGEN_GAIN_ADJUST,&bias);   //VB

	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,mod_reg[(ch)][0]);
	_cwdm4_207_tosa_m4820_i2c_read_byte(mod_reg[(ch)][1],&mod);			//VC

	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,cross_reg[(ch)][0]);
	_cwdm4_207_tosa_m4820_i2c_read_byte(cross_reg[(ch)][1],&cross);		//VX

	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,page);

	lut->ch[ch].config.bias = (uint16_t)bias;
	lut->ch[ch].config.mod  = (uint16_t)mod;
	lut->ch[ch].config.cross = (uint16_t)cross;

}


void CWDM4_207_drv_poll()
{
	ADC0_poll();
}

static void  cwdm4_207_drv_check_laser_ch(SFP_DRV_LASER_CFG_T *cfg)
{
	if(cfg->bias >= 255)
	{
		cfg->bias = 255;
	}
	if(cfg->mod >= 255)
	{
		cfg->mod = 255;
	}
	if(cfg->cross >= 255)
	{
		cfg->cross = 255;
	}
}

void CWDM4_207_drv_ch_calc_cfg(int16_t cur_temp, uint8_t ch, const SFP_DRV_LASER_CFG_T *lut, SFP_DRV_LASER_CFG_T *ret)
{
	SFP_DRV_LASER_CFG_T *lo_cfg = (lut + ch);
	SFP_DRV_LASER_CFG_T *m_cfg	= (lo_cfg + MSA_CHANNEL_NUM);
	SFP_DRV_LASER_CFG_T *hi_cfg = (m_cfg + MSA_CHANNEL_NUM);
	int16_t temp_l = 0, temp_r = 0, temp_h = 0;
	float a1 = 0.0, a2 = 0.0, a3 = 0.0;
	APC_CALI_PARAM para;

	ret->temp = cur_temp;
	ret->bias  = 0;
	ret->mod   = 0;
	ret->cross = 0;


	if(!MSA_p67_check_apc_lut_tab(lut))  //apc paramters is invalid, so there is no need to apc calibration
	{
		return;
	}

	if (cur_temp <= m_cfg->temp)
	{
		if((m_cfg->temp == lo_cfg->temp))
		{
			return;
		}

		ret->bias = (uint16_t )(floor(0.5 + (double)lo_cfg->bias +
						((double)(cur_temp - lo_cfg->temp)) * ((double)(m_cfg->bias - lo_cfg->bias))
						/ ((double)(m_cfg->temp - lo_cfg->temp))));
		ret->mod = (uint16_t)(floor(0.5 + (double)lo_cfg->mod +
						((double)(cur_temp - lo_cfg->temp)) * ((double)(m_cfg->mod - lo_cfg->mod))
						/ ((double)(m_cfg->temp - lo_cfg->temp))));
		ret->cross = (int16_t)(floor((double)lo_cfg->cross +
						((double)(cur_temp - lo_cfg->temp)) * ((double)(m_cfg->cross - lo_cfg->cross)
						/ ((double)(m_cfg->temp - lo_cfg->temp)))));
	}
	else
	{
		if(lo_cfg->temp == m_cfg->temp || lo_cfg->temp == hi_cfg->temp || m_cfg->temp == hi_cfg->temp)
		{
			return;
		}

		temp_l = lo_cfg->temp / TEMP_SCALE;
		temp_r = m_cfg->temp  / TEMP_SCALE;
		temp_h = hi_cfg->temp / TEMP_SCALE;

		a1 = (double)lo_cfg->bias /((double)(temp_l - temp_r) * (double)(temp_l - temp_h));
		a2 = (double)m_cfg->bias  /((double)(temp_r - temp_l) * (double)(temp_r - temp_h));
		a3 = (double)hi_cfg->bias /((double)(temp_h - temp_l) * (double)(temp_h - temp_r));

		//MSA_p2_log("\r\n a1= %f,a2 = %f, a3= %f",a1,a2,a3);

		para.a = a1 + a2 + a3;
		para.b = -a1 * (double)(temp_r + temp_h) - a2 * (double)(temp_l + temp_h) - a3 * (double)(temp_l + temp_r);
		para.c = a1 * (double)(temp_r) * (double)(temp_h) + a2 * (double)(temp_l) * (double)(temp_h) + a3 * (double)(temp_l) * (double)(temp_r);
		ret->bias = (uint16_t)(floor(0.5 + (para.a * pow((float)(cur_temp/TEMP_SCALE), 2.0) + para.b * (float)(cur_temp/TEMP_SCALE) + para.c)));

		//MSA_p2_log("\r\n a= %f,b = %f, c= %f, bias = %hd",para.a, para.b, para.c, ret->bias);

		ret->mod = (uint16_t)(floor((double)m_cfg->mod +
							((double)(cur_temp - m_cfg->temp)) * ((double)(hi_cfg->mod - m_cfg->mod)/ (hi_cfg->temp - m_cfg->temp))));
		ret->cross = (int16_t)(floor((double)m_cfg->cross +
							((double)(cur_temp - m_cfg->temp)) * ((double)(hi_cfg->cross - m_cfg->cross)/(hi_cfg->temp - m_cfg->temp))));

		//MSA_p2_log("\r\n bias= %hd,mod = %hd",ret->bias, ret->mod);

	}

	cwdm4_207_drv_check_laser_ch(ret);
	return ;

}

MSA_INTR_STATUS_T *CWDM4_207_get_drv_intr_status(uint8_t intr_mask)
{
	static MSA_INTR_STATUS_T status;
	uint8_t value = 0, ts = 0;

	if(intr_mask & PORT_INTR_TX_FAULT)
	{
		_cwdm4_207_txcdr_m37049_i2c_read_byte(M3704X_LOL_OR_LOS_LOS_STATUS, &value);  //get tx fault
		status.tx_fault = value & 0x0F;
	}

	if(intr_mask & PORT_INTR_TX_LOS)
	{
		_cwdm4_207_txcdr_m37049_i2c_read_byte(M3704X_LOS_LOL_STATUS, &value);  //get tx los
		status.tx_los = value & 0x0F;
	}

	if(intr_mask & PORT_INTR_TX_LOL)
	{
		_cwdm4_207_txcdr_m37049_i2c_read_byte(M3704X_LOS_LOL_STATUS, &value);  //get tx lol
		status.tx_lol = (value & 0xF0) >> 4;
	}

	if(intr_mask & PORT_INTR_RX_LOL)
	{
		_cwdm4_207_rxcdr_m37244_i2c_read_byte(0x17, &value);  //get rx lol
		status.rx_lol = REVERSE_LSB_4BITS(((value & 0xF0) >> 4));
	}

	if(intr_mask & PORT_INTR_RX_LOS)
	{
		_cwdm4_207_rxcdr_m37244_i2c_read_byte(0x17, &value);  //get rx los
		status.rx_los = REVERSE_LSB_4BITS(value & 0x0F);
	}
	return &status;
}

//MSA Lower Page byte86 for tx disable
void CWDM4_207_set_tx_enable(uint8_t enale)
{
	uint8_t value = 0,page;
//	enale = REVERSE_LSB_4BITS(enale);
	_cwdm4_207_tosa_m4820_i2c_read_byte(TOSA_PAGE0_INDIRECT_PAGE,&page);

	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,0);
	value = (enale&0x01)|((enale&0x02)<<1)|((enale&0x04)<<2)|((enale&0x08)<<3);
	value = ~((value<<1)|value);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_CHX_SHUTDOW,value);
	_cwdm4_207_tosa_m4820_i2c_read_byte(TOSA_PAGE0_CHX_SHUTDOW,&value);
	value = ~(value&0x01)|((value&0x02)>>1)|((value&0x04)>>2)|((value&0x08)>>3);
	_cwdm4_207_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,page);
}

//MSA Lower Page byte93 for power override
void CWDM4_207_set_low_power(uint8_t enable)
{
	if(enable)
	{
		MSA_set_low_power_mode(0);
		IT01CF &= ~IT01CF_IN0PL__ACTIVE_HIGH;
		//TX3V3_EN = 0;  //close tx3v3 in order to realize low power mode
		TX3V3_EN = 0;
		RX3V3_EN = 0;
		TOSA_RST = 0;
		TX_DIS = 1;
		SDA_M = 0; 	//make SDA_M = 0 to prevent mcu provide voltage to cdr chip
		SCL_M = 0;
	}
	else
	{
		MSA_set_low_power_mode(1);
		IT01CF |= IT01CF_IN0PL__ACTIVE_HIGH;
		//TX3V3_EN = 1; //open tx3v3 in order to realize high power mode
		TX3V3_EN = 1;
		RX3V3_EN = 1;
		TOSA_RST = 1;
		TX_DIS = 0;
		SDA_M = 1;
		SCL_M = 1;
	}
}

//MSA Lower Page byte98 for tx and rx cdr control
void CWDM4_207_set_cdr_control(uint8_t value)
{
	uint8_t tx_cdr_control = 0, rx_cdr_control = 0, tmp = 0,channel;

	//msa value is reverse with cdr value
	tx_cdr_control = ~(((value) & 0xF0) >> 4);
	rx_cdr_control = REVERSE_LSB_4BITS(~((value) & 0x0F));

	for(channel = 0; channel < MSA_CHANNEL_NUM; channel++)
	{
		_cwdm4_207_txcdr_m37049_i2c_read_byte(M3704X_MODE_CH(channel),&tmp);
		tmp = CLEAR_BIT(tmp, 5) | (GET_BIT(tx_cdr_control, channel) << 5);
		_cwdm4_207_txcdr_m37049_i2c_write_byte(M3704X_MODE_CH(channel),tmp);
	}

	_cwdm4_207_rxcdr_m37244_i2c_read_byte(0x10,&tmp);
	tmp = CLEAR_LSB_4BITS(tmp)| rx_cdr_control;
	_cwdm4_207_rxcdr_m37244_i2c_write_byte(0x10, tmp);  //set rx cdr control
}

//MSA Page3 byte234 and byte235 for tx eq
void CWDM4_207_set_tx_eq(uint8_t channel, uint8_t value)
{
	uint8_t eq_table[11] = {0,0,1,2,2,3,4,4,4,6,7};
	if(value != 0xff)
	{
		value = (eq_table[value & 0x0f]) << 4;
	}
	else
	{
		value = 0x50;
	}

	_cwdm4_207_txcdr_m37049_i2c_write_byte(M3704X_SLA_CH(channel),value);
}

//MSA Page3 byte236 and byte237 for rx emphasis
void CWDM4_207_set_rx_emphasis(uint8_t channel, uint8_t value)
{
	uint8_t reg_addr[] = {0x46, 0x46,0x47,0x47};
	uint8_t rx_emphasis[] = {0x0, 0x3, 0x5, 0x7,0x9, 0xb, 0xd, 0xf};
	uint8_t tmp = 0;

	if(value > RX_EMPHASIS_MAX_VALID)
	{
		return ;
	}

	//rx channel reverse
	channel = 3 - channel;

	_cwdm4_207_rxcdr_m37244_i2c_read_byte(reg_addr[channel], &tmp); //rxcdr pin is inverted sequence with qsfp28 pin

	if(channel == 0x0 || channel == 0x2)
	{
		value = CLEAR_LSB_4BITS(tmp) | rx_emphasis[value];
	}
	else if(channel == 0x1 || channel == 0x3)
	{
		value = CLEAR_MSB_4BITS(tmp) | (rx_emphasis[value] << 4);
	}

	_cwdm4_207_rxcdr_m37244_i2c_write_byte(reg_addr[channel], value);//rxcdr pin is inverted sequence with qsfp28 pin
}

//MSA Page3 byte238 and byte239 rx amplitude
//0x3 900  --0x3c
//0x2 600  --0x1e
//0x1 450  --0x0f
//0x0 250  --0x00
void CWDM4_207_set_rx_amplitude(uint8_t channel, uint8_t value)
{
	uint8_t reg_addr[] = {0x42,0x43,0x44,0x45};
	uint8_t rx_amplitude[] = {0x00, 0x0f, 0x1e, 0x3c};
	uint8_t tmp = 0;

	if(value > RX_AMPLITUDE_MAX_VALID)
	{
		return;
	}

	//rx channel reverse, rxcdr pin is inverted sequence with qsfp28 pin
	channel = 3 - channel;

	_cwdm4_207_rxcdr_m37244_i2c_read_byte(reg_addr[channel], &tmp);

	value = (tmp & 0xC0) | rx_amplitude[value];

	_cwdm4_207_rxcdr_m37244_i2c_write_byte(reg_addr[channel], value);
}


//MSA Page3 byte240 sq disable
void CWDM4_207_set_cdr_sq_disable(uint8_t value)
{
	uint8_t tmp = 0,channel, rx_sq = 0, tx_sq = 0;

	//rx channel reverse
	rx_sq  = REVERSE_LSB_4BITS((value & 0xF0) >> 4);
	tx_sq  = (value & 0x0F);

	for(channel = 0; channel < MSA_CHANNEL_NUM; channel++)
	{
		_cwdm4_207_txcdr_m37049_i2c_read_byte(M3704X_OUTPUT_SWING_CH(channel),&tmp);
		tmp = CLEAR_BIT(tmp, 7) | (GET_BIT(tx_sq, channel) << 7);
		_cwdm4_207_txcdr_m37049_i2c_write_byte(M3704X_OUTPUT_SWING_CH(channel),tmp);
	}

	_cwdm4_207_rxcdr_m37244_i2c_read_byte(0x40,&tmp);
	tmp = CLEAR_MSB_4BITS(tmp)| (rx_sq << 4);
	_cwdm4_207_rxcdr_m37244_i2c_write_byte(0x40, tmp);  //set rx cdr control

}

//MSA Page3 byte241 for rx output disable
void CWDM4_207_set_rx_output_disable(uint8_t value)
{
	uint8_t rx_output_disable = 0,  tmp = 0;

	//rx channel reverse
	rx_output_disable = REVERSE_LSB_4BITS((value & 0xF0) >> 4);

	_cwdm4_207_rxcdr_m37244_i2c_read_byte(0x10, &tmp);
	tmp = CLEAR_MSB_4BITS(tmp)| (rx_output_disable << 4);
	_cwdm4_207_rxcdr_m37244_i2c_write_byte(0x10, tmp);
}

//MSA Page3 byte241 for tx adapt eq
void  CWDM4_207_set_tx_adapt_eq(uint8_t value1, uint8_t value2,uint8_t value3)
{
	uint8_t tmp = 0, tx_adapt_eq = 0,channel;

	tx_adapt_eq = (value1 & 0x0F);

	for(channel = 0; channel < MSA_CHANNEL_NUM; channel ++)
	{
		if(GET_BIT(tx_adapt_eq, channel) == 0x1)
		{
			CWDM4_207_set_tx_eq(channel,0xff);
		}
		else
		{
			if(channel >= 0x0 && channel < 0x2)
			{
				tmp = ((value2) >> (4 * ((channel + 1) % 2)))& 0x0f;
			}
			else if(channel >= 0x2 && channel < MSA_CHANNEL_NUM)
			{
				tmp = ((value3) >> (4 * ((channel + 1) % 2)))& 0x0f;
			}
			CWDM4_207_set_tx_eq(channel,tmp& 0x0f);
		}
	}
}

#endif
