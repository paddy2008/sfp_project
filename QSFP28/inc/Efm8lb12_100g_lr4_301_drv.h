#ifndef __sfp_EFM8LB1_100g_lr4_h__
#define __sfp_EFM8LB1_100g_lr4_h__

#include "Efm8lb12_adc.h"
#include "Efm8lb12_pid.h"
#include "version.h"

#ifdef EFM8LB12E64_100G_SFP_LR4_301

//rxcdr i2c dev addr
#define LR4_RXCDR_BASE_ADDR  	 (0x16)

//txcdr i2c dev addr
#define LR4_TXCDR_BASE_ADDR  	 (0x18)

//tosa i2c dev addr
#define LR4_TOSA_BASE_ADDR   	 (0x20)

#define RSSI_SERIES_RES 	 	 (750)

#define LR4_TOSA_PAGE_SELECT_REG (0xFF)

#ifdef HW_VER_AM1_301
//Lr4 pin define
SI_SBIT (MODSEL, SFR_P3, 2);   		//slave i2c piece choose single
SI_SBIT (RESET, SFR_P3, 3);			//qsfp28 reset single
SI_SBIT (LPMODE, SFR_P3, 0);		//qsfp28 low power mode
SI_SBIT (TX_DIS, SFR_P1, 1);		//TX close single
SI_SBIT (INTL, SFR_P0, 7);			//qsfp28 interrupt output single
SI_SBIT (TOSA_RST, SFR_P2, 2);		//tosa reset single
SI_SBIT (TEC_EN, SFR_P0, 3);		//TEC enable or disable
SI_SBIT (TX1V8_EN, SFR_P3, 4);		//tx circul voltage enable or disable
SI_SBIT (TX3V3_EN, SFR_P0, 0);		//tx circul voltage enable or disable
#endif

#if defined(HW_VER_AM2_301) || defined(HW_VER_AM3_301)
//Lr4 pin define
SI_SBIT (MODSEL, SFR_P0, 0);   		//slave i2c piece choose single
SI_SBIT (RESET, SFR_P0, 2);			//qsfp28 reset single
SI_SBIT (LPMODE, SFR_P0, 1);		//qsfp28 low power mode
SI_SBIT (TX_DIS, SFR_P1, 1);		//TX close single
SI_SBIT (INTL, SFR_P3, 0);			//qsfp28 interrupt output single
SI_SBIT (TOSA_RST, SFR_P3, 4);		//tosa reset single
SI_SBIT (TEC_EN, SFR_P0, 3);		//TEC enable or disable
SI_SBIT (TX1V8_EN, SFR_P3, 2);		//tx circul voltage enable or disable
SI_SBIT (TX3V3_EN, SFR_P3, 1);		//tx circul voltage enable or disable
SI_SBIT (SDA_M, SFR_P1, 2);		//tx circul voltage enable or disable
SI_SBIT (SCL_M, SFR_P1, 3);		//tx circul voltage enable or disable
#endif

#if defined(HW_VER_AM4_301)
//Lr4 pin define
SI_SBIT (MODSEL, SFR_P0, 0);   		//slave i2c piece choose single
SI_SBIT (RESET, SFR_P0, 2);			//qsfp28 reset single
SI_SBIT (LPMODE, SFR_P0, 1);		//qsfp28 low power mode
SI_SBIT (TX_DIS, SFR_P1, 1);		//TX close single
SI_SBIT (INTL, SFR_P3, 0);			//qsfp28 interrupt output single
SI_SBIT (TOSA_RST, SFR_P3, 4);		//tosa reset single
SI_SBIT (TEC_EN, SFR_P0, 3);		//TEC enable or disable
SI_SBIT (RX_LOS, SFR_P3, 2);		//rx los interrupt
SI_SBIT (TX3V3_EN, SFR_P3, 1);		//tx circul voltage enable or disable
SI_SBIT (SDA_M, SFR_P1, 2);			//tx circul voltage enable or disable
SI_SBIT (SCL_M, SFR_P1, 3);			//tx circul voltage enable or disable
#endif

#ifdef SUPPORT_TEC_FUNC

#define M3704X_CHIPID 					(0x00)
#define M3704X_REVID 					(0x01)
#define M3704X_RESET 					(0x02)
#define M3704X_ALARM_TYPE				(0x03)
#define M3704X_CDRBIAS					(0x04)
#define M3704X_I2C_ADDRESS_MODE 		(0x05)
#define M3704X_CDR_LD  					(0x0C)
#define M3704X_PRBSGEN					(0x10)
#define M3704X_PRBSGEN_PLL				(0x11)
#define M3704X_PRBSGEN_PLL_LOCK			(0x12)
#define M3704X_LOS_LOL_STATUS			(0x14)
#define M3704X_LOL_OR_LOS_LOS_STATUS 	(0x15)
#define M3704X_LOS_LOL_ALARM 			(0x16)
#define M3704X_MODE 					(0x30)
#define M3704X_LOS_LOL_MASK				(0x31)
#define M3704X_LOS_THRS					(0x32)
#define M3704X_SLA						(0x33)   //M37046
#define M3704X_CTLE						(0x33)	//M37049
#define M3704X_OUTPUT_SWING				(0x34)
#define M3704X_OUTPUT_DEEMPH  			(0x35)
#define M3704X_PRBSCHK 					(0x39)
#define M3704X_PRBSCHK_BANK				(0x3A)
#define M3704X_PRBSCHK_MODE				(0x3B)
#define M3704X_PRBSCHK_EYE				(0x3C)
#define M3704X_PRBSCHK_DELAY			(0x3D)
#define M3704X_MODE_CH(N)				(uint8_t)((0x40)+(N)*(0x10))
#define M3704X_LOS_LOL_MASK_CH(N)		(uint8_t)(M3704X_MODE_CH(N)+(0x01)
#define M3704X_LOS_THRS_CH(N)			(uint8_t)(M3704X_MODE_CH(N)+(0x02))
#define M3704X_SLA_CH(N)				(uint8_t)(M3704X_MODE_CH(N)+(0x03))
#define M3704X_OUTPUT_SWING_CH(N)		(uint8_t)(M3704X_MODE_CH(N)+(0x04))
#define M3704X_OUTPUT_DEEMPH_CH(N)		(uint8_t)(M3704X_MODE_CH(N)+(0x05))
#define M3704X_PRBSCHK_CH(N)			(uint8_t)(M3704X_MODE_CH(N)+(0x09))
#define M3704X_PRBSCHK_BANK_CH(N)		(uint8_t)(M3704X_MODE_CH(N)+(0x0A))
#define M3704X_PRBSCHK_MODE_CH(N)		(uint8_t)(M3704X_MODE_CH(N)+(0x0B))
#define M3704X_PRBSCHK_EYE_CH(N)		(uint8_t)(M3704X_MODE_CH(N)+(0x0C))
#define M3704X_PRBSCHK_DELAY_CH(N)		(uint8_t)(M3704X_MODE_CH(N)+(0x0D))
#define M3704X_PRBSCHK_ERROP1_CH(N)		(uint8_t)(M3704X_MODE_CH(N)+(0x0E))
#define M3704X_PRBSCHK_ERROP2_CH(N)		(uint8_t)(M3704X_MODE_CH(N)+(0x0F))

#define TOSA_PAGE0_VENDOR_ID_II			(0x00)
#define TOSA_PAGE0_RESET				(0x02)
#define TOSA_PAGE0_TEMP_II				(0x04)
#define TOSA_PAGE0_APP					(0x06)
#define TOSA_PAGE0_LOAD_SWITCH  		(0x08)
#define TOSA_PAGE0_VERSION_II			(0x0A)
#define TOSA_PAGE0_YEAR_II				(0x0C)
#define TOSA_PAGE0_MONTH_II				(0x0E)
#define TOSA_PAGE0_SERIAL_IV			(0x10)
#define TOSA_PAGE0_CHX_SHUTDOW			(0x14)
#define TOSA_PAGE0_ANALOG_CTRL_1 		(0x1E)
#define TOSA_PAGE0_ANALOG_CTRL_2 		(0x1F)
#define TOSA_PAGE0_ANALOG_TEST 			(0x21)
#define TOSA_PAGE0_CNT_TEMP_CAL 		(0x22)
#define TOSA_PAGE0_CNT_BG				(0x23)
#define TOSA_PAGE0_ADC_ENABLE_II		(0x30)
#define TOSA_PAGE0_ADC_UPDATE_II		(0x32)
#define TOSA_PAGE0_ADC_RECOVERY_FIRST	(0x34)
#define TOSA_PAGE0_ADC_RECOVERY_REST	(0x35)
#define TOSA_PAGE0_ADC_CTRL				(0x36)
#define TOSA_PAGE0_ADC_DAC_II 			(0x38)
#define TOSA_PAGE0_ADC_COMP_IN 			(0x37)
#define TOSA_PAGE0_EEPROM_III			(0xF0)
#define TOSA_PAGE0_EEPROM_RUN 			(0xF3)
#define TOSA_PAGE0_I2C_ADDRESS_MODE 	(0xF4)
#define TOSA_PAGE0_INDIRECT_ADDRESS 	(0xFE)
#define TOSA_PAGE0_INDIRECT_PAGE		(0xFF)
#define TOSA_PAGEN_PEAK_INDICATOR_II	(0x00)
#define TOSA_PAGEN_AMPLITUDE_SETPOINT 	(0x02)
#define TOSA_PAGEN_GAIN_ADJUST			(0x04)
#define TOSA_PAGEN_AGC_ENABLE			(0x06)
#define TOSA_PAGEN_ID					(0x08)
#define TOSA_PAGEN_VO1					(0x0A)
#define TOSA_PAGEN_VO2					(0x0C)
#define TOSA_PAGEN_APPMODE1_ID			(0x10)
#define TOSA_PAGEN_APPMODE1_VO1			(0x12)
#define TOSA_PAGEN_APPMODE1_VO2			(0x14)
#define TOSA_PAGEN_APPMODE2_ID			(0x18)
#define TOSA_PAGEN_APPMODE2_VO1			(0x1A)
#define TOSA_PAGEN_APPMODE2_VO2			(0x1C)
#define TOSA_PAGEN_BIAS_MON_II			(0x20)
#define TOSA_PAGEN_APC_POLARITY			(0x22)
#define TOSA_PAGEN_TEMP_MON_II			(0x24)
#define TOSA_PAGEN_EXTERNAL_SOURCE_II	(0x26)

#endif
/******************************************** DRV Interface API *****************************************/
void 		LR4_301_chip_reset();

void 		LR4_301_chip_init();

void 		LR4_301_port_init();

void 		LR4_301_drv_adjust_laser_ch(uint8_t ch, SFP_DRV_LASER_CFG_T *cfg);

void 		LR4_301_drv_get_laser_full_state(uint8_t ch, MSA_LUT_T *lut);

void 		LR4_301_drv_poll();

void 		LR4_301_drv_ch_calc_cfg(int16_t cur_temp, uint8_t ch, const SFP_DRV_LASER_CFG_T *lut, SFP_DRV_LASER_CFG_T *ret);

void		LR4_301_set_low_power(uint8_t enable);

int16_t  	LR4_301_ddmi_get_temp(MSA_DRV_VAL_TYPE type);

uint16_t 	LR4_301_ddmi_get_vcc(MSA_DRV_VAL_TYPE type);

uint16_t 	LR4_301_ddmi_get_bias(uint8_t channel, MSA_DRV_VAL_TYPE type);

uint16_t 	LR4_301_ddmi_get_rx_power(uint8_t channel, MSA_DRV_VAL_TYPE type);

uint16_t 	LR4_301_ddmi_get_tx_power(uint8_t channel, MSA_DRV_VAL_TYPE type);

void	 	LR4_301_set_tx_enable(uint8_t enale);

void 		LR4_301_set_tec_enable(uint8_t enale);

void 	 	LR4_301_set_cdr_control(uint8_t value);

void 	 	LR4_301_set_tx_eq(uint8_t channel, uint8_t value);

void 	 	LR4_301_set_rx_emphasis(uint8_t channel, uint8_t value);

void 	 	LR4_301_set_rx_amplitude(uint8_t channel, uint8_t value);

void 	 	LR4_301_set_cdr_sq_disable(uint8_t value);

void 	 	LR4_301_set_rx_output_disable(uint8_t value);

void 	 	LR4_301_set_tx_adapt_eq(uint8_t value1, uint8_t value2, uint8_t value3);

void 		LR4_301_update_dev_ready_status(void);

MSA_INTR_STATUS_T *LR4_301_get_drv_intr_status(uint8_t intr_mask);

#ifdef SUPPORT_TEC_FUNC

uint16_t 	LR4_301_get_tec_tempout();
uint16_t 	LR4_301_get_tec_tempset();
uint16_t 	LR4_301_get_itec();
uint16_t 	LR4_301_get_vtec();

#endif

#endif
#endif
