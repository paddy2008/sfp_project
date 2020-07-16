#ifndef __sfp_EFM8LB1_100g_aoc_101_h__
#define __sfp_EFM8LB1_100g_aoc_101_h__

#include <SI_EFM8LB1_Register_Enums.h>
#include "version.h"

#ifdef EFM8LB12E64_100G_SFP_AOC_101

//txcdr i2c dev addr
#define AOC_101_TXCDR_BASE_ADDR  	(0x18)

//rxcdr i2c dev addr
#define AOC_101_RXCDR_BASE_ADDR  	(0x16)

#define AOC_101_RSSI_SERIES_RES    	(2000)

SI_SBIT (SDA_M, SFR_P1, 2);			//i2c sda
SI_SBIT (SCL_M, SFR_P1, 3);			//i2c scl

#ifdef HW_VER_AM3_101
/*Aoc macom inner pin define*/
SI_SBIT (TX_DIS, SFR_P1, 1);		//TX close single
SI_SBIT (RX_INT, SFR_P2, 3);		//rx interrupt
SI_SBIT (TX_INT, SFR_P1, 5);		//tx interrupt
/*Aoc macom qsfp28 pin define*/
SI_SBIT (MODSEL, SFR_P3, 2);   		//qsfp28 slave i2c piece choose single
SI_SBIT (RESET, SFR_P0, 2);			//qsfp28 reset single
SI_SBIT (LPMODE, SFR_P3, 0);		//qsfp28 low power mode
SI_SBIT (INTL, SFR_P0, 7);			//qsfp28 interrupt output single
SI_SBIT (TX3V3_EN, SFR_P0, 1);		//tx circul voltage enable or disable

#endif

#ifdef HW_VER_AM4_101
/*Aoc macom inner pin define*/
SI_SBIT (TX_DIS, SFR_P1, 1);		//TX close single
SI_SBIT (RX_INT, SFR_P0, 6);		//rx interrupt
SI_SBIT (TX_INT, SFR_P0, 7);		//tx interrupt
/*Aoc macom qsfp28 pin define*/
SI_SBIT (MODSEL, SFR_P1, 0);   		//qsfp28 slave i2c piece choose single
SI_SBIT (RESET, SFR_P0, 2);			//qsfp28 reset single
SI_SBIT (LPMODE, SFR_P0, 1);		//qsfp28 low power mode
SI_SBIT (INTL, SFR_P3, 0);			//qsfp28 interrupt output single
SI_SBIT (TX3V3_EN, SFR_P3, 1);		//tx circul voltage enable or disable
#endif

/************************************** DRV Interface API ************************************/
void 	 AOC_101_chip_reset();

void 	 AOC_101_chip_init();

void 	 AOC_101_port_init();

void 	 AOC_101_polling();

void 	 AOC_101_drv_adjust_laser_ch(uint8_t ch, SFP_DRV_LASER_CFG_T *cfg);

void 	 AOC_101_drv_get_laser_full_state(uint8_t channel, MSA_LUT_T *lut);

void 	 AOC_101_drv_ch_calc_cfg (int16_t cur_temp, uint8_t ch, const SFP_DRV_LASER_CFG_T *lut, SFP_DRV_LASER_CFG_T *ret);

void	 AOC_101_set_low_power(uint8_t enable);

void 	 AOC_101_set_cdr_control(uint8_t value);

void 	 AOC_101_set_tx_eq(uint8_t channel, uint8_t value);

void 	 AOC_101_set_rx_emphasis(uint8_t channel, uint8_t value)         ;

void 	 AOC_101_set_rx_amplitude(uint8_t channel, uint8_t value);

void 	 AOC_101_set_cdr_sq_disable(uint8_t value);

void 	 AOC_101_set_rx_output_disable(uint8_t value);

void 	 AOC_101_set_tx_adapt_eq(uint8_t value1, uint8_t value2, uint8_t value3);

void     AOC_101_set_tx_enable(uint8_t enale);

int16_t  AOC_101_ddmi_get_temp(MSA_DRV_VAL_TYPE type);

uint16_t AOC_101_ddmi_get_vcc(MSA_DRV_VAL_TYPE type);

uint16_t AOC_101_ddmi_get_bias(uint8_t channel, MSA_DRV_VAL_TYPE type);

uint16_t AOC_101_ddmi_get_rx_power(uint8_t channel, MSA_DRV_VAL_TYPE type);

uint16_t AOC_101_ddmi_get_tx_power(uint8_t channel, MSA_DRV_VAL_TYPE type);

MSA_INTR_STATUS_T *AOC_101_get_drv_intr_status(uint8_t intr_mask);

#endif

#endif
