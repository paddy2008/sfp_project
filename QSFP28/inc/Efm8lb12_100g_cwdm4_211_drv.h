#ifndef __sfp_EFM8LB1_100g_cwdm4_211_h__
#define __sfp_EFM8LB1_100g_cwdm4_211_h__

#include <SI_EFM8LB1_Register_Enums.h>
#include "version.h"

#ifdef EFM8LB12E64_100G_SFP_CWDM4_211


#if defined(HW_VER_SEMTEC_AM1_211)
//txcdr i2c dev addr
#define CWDM4_211_TXCDR_BASE_ADDR  (0x24)

#endif

#if defined(HW_VER_MAXIM_AM1_211) || defined(HW_VER_MAXIM_AM2_211) || defined(HW_VER_MAXIM_AM3_211)
//txcdr i2c dev addr
#define CWDM4_211_TXCDR_BASE_ADDR  (0xC0)

#endif


//rxcdr i2c dev addr
#define CWDM4_211_RXCDR_BASE_ADDR  (0x16)

#define CWDM4_211_RSSI_SERIES_RES  (2000)

#define CWDM4_211_MPD_SERIES_RES   (1000)

//cwdm4 211
#if defined(HW_VER_SEMTEC_AM1_211)
SI_SBIT (MODSEL, SFR_P1, 0);   		//slave i2c piece choose single
SI_SBIT (RESET, SFR_P0, 2);			//qsfp28 reset single
SI_SBIT (LPMODE, SFR_P0, 1);		//qsfp28 low power mode
SI_SBIT (TX_DIS, SFR_P1, 1);		//TX close single
SI_SBIT (TX_RST, SFR_P1, 4);		//TX reset single
SI_SBIT (INTL, SFR_P3, 0);			//qsfp28 interrupt output single
SI_SBIT (P1V8_EN, SFR_P3, 1);		//rx circul voltage enable or disable

SI_SBIT (TX_LOSL, SFR_P0, 7);		//tx unlock alarm
SI_SBIT (TX_FAULT, SFR_P0, 3);		//tx lose efficacy alarm
SI_SBIT (SDA_M, SFR_P1, 2);			//i2c sda
SI_SBIT (SCL_M, SFR_P1, 3);			//i2c scl

#endif

#if defined(HW_VER_MAXIM_AM1_211)
SI_SBIT (MODSEL, SFR_P1, 0);   		//slave i2c piece choose single
SI_SBIT (RESET, SFR_P0, 2);			//qsfp28 reset single
SI_SBIT (LPMODE, SFR_P0, 1);		//qsfp28 low power mode
SI_SBIT (TX_DIS, SFR_P1, 1);		//TX close single
SI_SBIT (INTL, SFR_P3, 0);			//qsfp28 interrupt output single
SI_SBIT (P1V8_EN, SFR_P3, 1);		//rx circul voltage enable or disable

SI_SBIT (TX_LOSL, SFR_P0, 7);		//tx unlock alarm
SI_SBIT (SDA_M, SFR_P1, 2);			//i2c sda
SI_SBIT (SCL_M, SFR_P1, 3);			//i2c scl

#endif

#if defined(HW_VER_MAXIM_AM2_211)
SI_SBIT (MODSEL, SFR_P0, 3);   		//slave i2c piece choose single
SI_SBIT (RESET, SFR_P0, 4);			//qsfp28 reset single
SI_SBIT (LPMODE, SFR_P0, 1);		//qsfp28 low power mode
SI_SBIT (TX_DIS, SFR_P1, 1);		//TX close single
SI_SBIT (INTL, SFR_P3, 0);			//qsfp28 interrupt output single
SI_SBIT (P1V8_EN, SFR_P3, 1);		//rx circul voltage enable or disable

SI_SBIT (TX_LOSL, SFR_P0, 7);		//tx unlock alarm
SI_SBIT (SDA_M, SFR_P1, 2);			//i2c sda
SI_SBIT (SCL_M, SFR_P1, 3);			//i2c scl

#endif

#if defined(HW_VER_MAXIM_AM3_211)
SI_SBIT (MODSEL, SFR_P0, 3);   		//slave i2c piece choose single
SI_SBIT (RESET, SFR_P0, 4);			//qsfp28 reset single
SI_SBIT (LPMODE, SFR_P0, 1);		//qsfp28 low power mode
SI_SBIT (VCCTO_EN, SFR_P1, 0);		//VCC enable
SI_SBIT (TX_DIS, SFR_P1, 1);		//TX close single
SI_SBIT (INTL, SFR_P3, 7);			//qsfp28 interrupt output single
SI_SBIT (P1V8_EN, SFR_P3, 1);		//rx circul voltage enable or disable

SI_SBIT (TX_LOSL, SFR_P0, 7);		//tx unlock alarm
SI_SBIT (SDA_M, SFR_P1, 2);			//i2c sda
SI_SBIT (SCL_M, SFR_P1, 3);			//i2c scl

#endif

typedef struct
{
	float a;
	float b;
	float c;
}APC_CALI_PARAM;

void  	 CWDM4_211_chip_reset(void);

void 	 CWDM4_211_chip_init(void);

void 	 CWDM4_211_port_init(void);

void 	 CWDM4_211_drv_adjust_laser_ch(uint8_t ch, SFP_DRV_LASER_CFG_T *cfg);

void 	 CWDM4_211_drv_get_laser_full_state(uint8_t channel, MSA_LUT_T *lut);

void 	 CWDM4_211_drv_ch_calc_cfg (int16_t cur_temp, uint8_t ch, const SFP_DRV_LASER_CFG_T *lut, SFP_DRV_LASER_CFG_T *ret);

void 	 CWDM4_211_drv_poll(void);

void 	 CWDM4_211_set_low_power(uint8_t enable);

void 	 CWDM4_211_set_cdr_control(uint8_t value);

void 	 CWDM4_211_set_tx_eq(uint8_t channel, uint8_t value);

void 	 CWDM4_211_set_rx_emphasis(uint8_t channel, uint8_t value);

void 	 CWDM4_211_set_rx_amplitude(uint8_t channel, uint8_t value);

void 	 CWDM4_211_set_cdr_sq_disable(uint8_t value);

void 	 CWDM4_211_set_rx_output_disable(uint8_t value);

void 	 CWDM4_211_set_tx_adapt_eq(uint8_t value1, uint8_t value2, uint8_t value3);

void	 CWDM4_211_set_tx_enable(uint8_t enable);

void 	 CWDM4_211_update_dev_ready_status(void);

void     CWDM4_211_get_drv_intr_status(MSA_INTR_STATUS_T *status);

int16_t  CWDM4_211_ddmi_get_temp(MSA_DRV_VAL_TYPE type);

uint16_t CWDM4_211_ddmi_get_vcc(MSA_DRV_VAL_TYPE type);

uint16_t CWDM4_211_ddmi_get_bias(uint8_t channel, MSA_DRV_VAL_TYPE type);

uint16_t CWDM4_211_ddmi_get_rx_power(uint8_t channel, MSA_DRV_VAL_TYPE type);

uint16_t CWDM4_211_ddmi_get_tx_power(uint8_t channel, MSA_DRV_VAL_TYPE type);





#endif
#endif









