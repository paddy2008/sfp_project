#ifndef __sfp_EFM8LB1_100g_cwdm4_206_h__
#define __sfp_EFM8LB1_100g_cwdm4_206_h__

#include <SI_EFM8LB1_Register_Enums.h>
#include "version.h"

#ifdef EFM8LB12E64_100G_SFP_CWDM4_206

//txcdr i2c dev addr
#define CWDM4_206_TXCDR_BASE_ADDR  (0x24)

//rxcdr i2c dev addr
#define CWDM4_206_RXCDR_BASE_ADDR  (0x16)

#define CWDM4_206_RSSI_SERIES_RES  (2000)

#if defined(HW_VER_AM2_206)
#define CWDM4_206_MPD_SERIES_RES   (1000)
#endif

//cwdm4 206
SI_SBIT (MODSEL, SFR_P1, 0);   		//slave i2c piece choose single
SI_SBIT (RESET, SFR_P0, 2);			//qsfp28 reset single
SI_SBIT (LPMODE, SFR_P0, 1);		//qsfp28 low power mode
SI_SBIT (TX_DIS, SFR_P1, 1);		//TX close single
SI_SBIT (TX_RST, SFR_P1, 4);		//TX reset single
SI_SBIT (INTL, SFR_P3, 0);			//qsfp28 interrupt output single
SI_SBIT (TX3V3_EN, SFR_P3, 1);		//tx circul voltage enable or disable

SI_SBIT (TX_LOSL, SFR_P0, 7);		//tx unlock alarm
SI_SBIT (TX_FAULT, SFR_P0, 3);		//tx lose efficacy alarm
SI_SBIT (SDA_M, SFR_P1, 2);			//i2c sda
SI_SBIT (SCL_M, SFR_P1, 3);			//i2c scl

typedef struct
{
	float a;
	float b;
	float c;
}APC_CALI_PARAM;

void  	 CWDM4_206_chip_reset(void);

void 	 CWDM4_206_chip_init(void);

void 	 CWDM4_206_port_init(void);

void 	 CWDM4_206_drv_adjust_laser_ch(uint8_t ch, SFP_DRV_LASER_CFG_T *cfg);

void 	 CWDM4_206_drv_get_laser_full_state(uint8_t channel, MSA_LUT_T *lut);

void 	 CWDM4_206_drv_ch_calc_cfg (int16_t cur_temp, uint8_t ch, const SFP_DRV_LASER_CFG_T *lut, SFP_DRV_LASER_CFG_T *ret);

void 	 CWDM4_206_drv_poll(void);

void 	 CWDM4_206_set_low_power(uint8_t enable);

void 	 CWDM4_206_set_cdr_control(uint8_t value);

void 	 CWDM4_206_set_tx_eq(uint8_t channel, uint8_t value);

void 	 CWDM4_206_set_rx_emphasis(uint8_t channel, uint8_t value);

void 	 CWDM4_206_set_rx_amplitude(uint8_t channel, uint8_t value);

void 	 CWDM4_206_set_cdr_sq_disable(uint8_t value);

void 	 CWDM4_206_set_rx_output_disable(uint8_t value);

void 	 CWDM4_206_set_tx_adapt_eq(uint8_t value1, uint8_t value2, uint8_t value3);

void	 CWDM4_206_set_tx_enable(uint8_t enable);

int16_t  CWDM4_206_ddmi_get_temp(MSA_DRV_VAL_TYPE type);

uint16_t CWDM4_206_ddmi_get_vcc(MSA_DRV_VAL_TYPE type);

uint16_t CWDM4_206_ddmi_get_bias(uint8_t channel, MSA_DRV_VAL_TYPE type);

uint16_t CWDM4_206_ddmi_get_rx_power(uint8_t channel, MSA_DRV_VAL_TYPE type);

uint16_t CWDM4_206_ddmi_get_tx_power(uint8_t channel, MSA_DRV_VAL_TYPE type);

MSA_INTR_STATUS_T *CWDM4_206_get_drv_intr_status(uint8_t intr_mask);


#endif
#endif









