#ifndef __sfp_EFM8LB1_100g_cwdm4_202_h__
#define __sfp_EFM8LB1_100g_cwdm4_202_h__

#include <SI_EFM8LB1_Register_Enums.h>
#include "version.h"

#ifdef EFM8LB12E64_100G_SFP_CWDM4_202

//txcdr i2c dev addr
#define CWDM4_202_TXCDR_BASE_ADDR  (0x24)
//rxcdr i2c dev addr
#define CWDM4_202_RXCDR_BASE_ADDR  (0x16)

#define CWDM4_202_MIN_NUM_CHANNES  (0x1)

//the num of scan,the max is 4
#define RSSI_NUM_CHANNELS  			(4)

#define RSSI_START_CHANNEL 			(ADC0_POSITIVE_INPUT_P16)

#define RSSI_SERIES_RES    			(2000)

#define MODULE_VOLTAGE     			(ADC0_POSITIVE_INPUT_P6)

//cwdm4 pin define    //cwdm4 201
#if defined(HW_VER_AM1_202)
SI_SBIT (MODSEL, SFR_P3, 2);   		//slave i2c piece choose single
SI_SBIT (RESET, SFR_P3, 3);			//qsfp28 reset single
SI_SBIT (LPMODE, SFR_P3, 0);		//qsfp28 low power mode
SI_SBIT (TX_DIS, SFR_P1, 1);		//TX close single
SI_SBIT (INTL, SFR_P0, 7);			//qsfp28 interrupt output single
SI_SBIT (TX3V3_EN, SFR_P0, 1);		//tx circul voltage enable or disable

SI_SBIT (TX_RESET, SFR_P1, 4);		//txcdr reset single
SI_SBIT (TX_LOSL, SFR_P1, 5);		//tx unlock alarm
SI_SBIT (TX_FAULT, SFR_P1, 6);		//tx lose efficacy alarm
#endif

//cwdm4 202
#if defined(HW_VER_AM2_202)
SI_SBIT (MODSEL, SFR_P1, 0);   		//slave i2c piece choose single
SI_SBIT (RESET, SFR_P0, 2);			//qsfp28 reset single
SI_SBIT (LPMODE, SFR_P0, 1);		//qsfp28 low power mode
SI_SBIT (TX_DIS, SFR_P1, 1);		//TX close single
SI_SBIT (INTL, SFR_P3, 0);			//qsfp28 interrupt output single
SI_SBIT (TX3V3_EN, SFR_P3, 1);		//tx circul voltage enable or disable

SI_SBIT (TX_RESET, SFR_P1, 4);		//txcdr reset single
SI_SBIT (TX_LOSL, SFR_P0, 7);		//tx unlock alarm
SI_SBIT (TX_FAULT, SFR_P0, 3);		//tx lose efficacy alarm
#endif

void CWDM4_202_chip_reset(void);

void CWDM4_202_chip_init(void);

void CWDM4_202_port_init(void);

int16_t  CWDM4_202_ddmi_get_temp(MSA_DRV_VAL_TYPE type);

uint16_t CWDM4_202_ddmi_get_vcc(MSA_DRV_VAL_TYPE type);

uint16_t CWDM4_202_ddmi_get_bias(uint8_t channel, MSA_DRV_VAL_TYPE type);

uint16_t CWDM4_202_ddmi_get_rx_power(uint8_t channel, MSA_DRV_VAL_TYPE type);

uint16_t CWDM4_202_ddmi_get_tx_power(uint8_t channel, MSA_DRV_VAL_TYPE type);

void 	 CWDM4_202_drv_adjust_laser_ch(uint8_t ch, SFP_DRV_LASER_CFG_T *cfg);

void 	 CWDM4_202_drv_get_laser_full_state(uint8_t channel, MSA_LUT_T *lut);

void 	 CWDM4_202_drv_ch_calc_cfg (int16_t cur_temp, uint8_t ch, const SFP_DRV_LASER_CFG_T *lut, SFP_DRV_LASER_CFG_T *ret);

void 	 CWDM4_202_drv_poll(void);

void	 CWDM4_202_set_tx_enable(uint8_t enable);

void 	 CWDM4_202_set_low_power(uint8_t enable);

void 	 CWDM4_202_set_cdr_control(uint8_t value);

void 	 CWDM4_202_set_tx_eq(uint8_t channel, uint8_t value);

void 	 CWDM4_202_set_rx_emphasis(uint8_t channel, uint8_t value);

void 	 CWDM4_202_set_rx_amplitude(uint8_t channel, uint8_t value);

void 	 CWDM4_202_set_cdr_sq_disable(uint8_t value);

void 	 CWDM4_202_set_rx_output_disable(uint8_t value);

void 	 CWDM4_202_set_tx_adapt_eq(uint8_t value1, uint8_t value2, uint8_t value3);

MSA_INTR_STATUS_T *CWDM4_202_get_drv_intr_status(uint8_t intr_mask);

#endif
#endif
