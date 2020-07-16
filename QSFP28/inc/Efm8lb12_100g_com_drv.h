#ifndef __sfp_EFM8LB1_100g_com_drv_h__
#define __sfp_EFM8LB1_100g_com_drv_h__

#include <SI_EFM8LB1_Register_Enums.h>
#include "Efm8lb12_port.h"
#include "version.h"

#ifdef EFM8LB12E64_100G_SFP_LR4_301
#include "Efm8lb12_100g_lr4_301_drv.h"
#endif

#ifdef EFM8LB12E64_100G_SFP_AOC_101
#include "Efm8lb12_100g_aoc_101_drv.h"
#endif


#ifdef EFM8LB12E64_100G_SFP_CWDM4_202
#include "Efm8lb12_100g_cwdm4_202_drv.h"
#endif

#ifdef EFM8LB12E64_100G_SFP_CWDM4_206
#include "Efm8lb12_100g_cwdm4_206_drv.h"
#endif

#ifdef EFM8LB12E64_100G_SFP_CWDM4_207
#include "Efm8lb12_100g_cwdm4_207_drv.h"
#endif

#ifdef EFM8LB12E64_100G_SFP_CWDM4_211
#include "Efm8lb12_100g_cwdm4_211_drv.h"
#endif

#define TX_EQ_MAX_VALID  	   (10)
#define RX_EMPHASIS_MAX_VALID  (7)
#define RX_AMPLITUDE_MAX_VALID (3)
#define TEMP_SCALE			   (0x100)

typedef enum {
	SFP_DRV_TEMP_LOW,
	SFP_DRV_TEMP_MIDDLE,
	SFP_DRV_TEMP_HIGH,
	SFP_DRV_TEMP_MAX,
}SFP_DRV_TEMP_LEVEL;


typedef enum {
	SFP_DRV_INTR_TX_LOS		= 0x01,
	SFP_DRV_INTR_RX_LOS 	= 0x02,
	SFP_DRV_INTR_TX_EQF 	= 0x04,
	SFP_DRV_INTR_TX_FAULT 	= 0x08,
	SFP_DRV_INTR_TX_LOL 	= 0x10,
	SFP_DRV_INTR_RX_LOL 	= 0x20,
}SFP_DRV_INTR_TYPE;

typedef enum
{
	TX_READY   = 0x1,
	RX_READY   = 0x2,
	TOSA_READY = 0x4,
}I2C_DEV;

/*
 * DDM_GET_TEMP    :    1/256 C
 * DDM_GET_VCC     :    0.1mV
 * DDM_GET_BIAS    :    0.1mA
 * DDM_GET_RX_POWER:    0.1uW or 0.1uA
 * DDM_GET_TX_POWER:    0.1uW or 0.1mA
 */
#ifdef EFM8LB12E64_100G_SFP_AOC_101

#define CHIP_RESET								AOC_101_chip_reset
#define CHIP_INIT								AOC_101_chip_init
#define PORT_INIT								AOC_101_port_init
#define DDM_GET_TEMP							AOC_101_ddmi_get_temp
#define DDM_GET_VCC								AOC_101_ddmi_get_vcc
#define DDM_GET_BIAS							AOC_101_ddmi_get_bias
#define DDM_GET_RX_POWER						AOC_101_ddmi_get_rx_power
#define DDM_GET_TX_POWER						AOC_101_ddmi_get_tx_power
#define SFP_DRV_POLL							AOC_101_polling
#define SFP_DRV_ADJUST_LASER_CH					AOC_101_drv_adjust_laser_ch
#define SFP_DRV_GET_LASER_FULL_STATE			AOC_101_drv_get_laser_full_state
#define SFP_DRV_CH_CALCULATE_CFG				AOC_101_drv_ch_calc_cfg
#define SFP_DRV_SET_LOW_POWER					AOC_101_set_low_power
#define SFP_DRV_SET_TX_ENABLE					AOC_101_set_tx_enable
#define SFP_DRV_SET_TX_BYPASS					AOC_101_set_tx_bypass
#define SFP_DRV_SET_RX_BYPASS					AOC_101_set_rx_bypass
#define SFP_DRV_INTR_STATUS			    		AOC_101_get_drv_intr_status
#define SFP_DRV_SET_CDR_CONTROL         		AOC_101_set_cdr_control
#define SFP_DRV_SET_TX_EQ						AOC_101_set_tx_eq
#define SFP_DRV_SET_RX_EMPHASIS					AOC_101_set_rx_emphasis
#define SFP_DRV_SET_RX_AMPLITUDE				AOC_101_set_rx_amplitude
#define SFP_DRV_SET_CDR_SQ_DISABLE      		AOC_101_set_cdr_sq_disable
#define SFP_DRV_SET_RX_OUTPUT_DISABLE			AOC_101_set_rx_output_disable
#define SFP_DRV_SET_TX_ADAPT_EQ					AOC_101_set_tx_adapt_eq
#define SFP_DRV_UPDATE_DEV_READY_STATUS         AOC_101_update_dev_ready_status
#endif

#ifdef EFM8LB12E64_100G_SFP_LR4_301

#define CHIP_RESET								LR4_301_chip_reset
#define CHIP_INIT								LR4_301_chip_init
#define PORT_INIT								LR4_301_port_init
#define DDM_GET_TEMP							LR4_301_ddmi_get_temp
#define DDM_GET_VCC								LR4_301_ddmi_get_vcc
#define DDM_GET_BIAS							LR4_301_ddmi_get_bias
#define DDM_GET_RX_POWER						LR4_301_ddmi_get_rx_power
#define DDM_GET_TX_POWER						LR4_301_ddmi_get_tx_power
#define SFP_DRV_POLL							LR4_301_drv_poll
#define SFP_DRV_ADJUST_LASER_CH					LR4_301_drv_adjust_laser_ch
#define SFP_DRV_GET_LASER_FULL_STATE			LR4_301_drv_get_laser_full_state
#define SFP_DRV_CH_CALCULATE_CFG				LR4_301_drv_ch_calc_cfg
#define SFP_DRV_SET_LOW_POWER					LR4_301_set_low_power
#define SFP_DRV_SET_TX_ENABLE					LR4_301_set_tx_enable
#define SFP_DRV_SET_TX_BYPASS					LR4_301_set_tx_bypass
#define SFP_DRV_SET_RX_BYPASS					LR4_301_set_rx_bypass
#define SFP_DRV_INTR_STATUS			    		LR4_301_get_drv_intr_status
#define SFP_DRV_SET_CDR_CONTROL         		LR4_301_set_cdr_control
#define SFP_DRV_SET_TX_EQ						LR4_301_set_tx_eq
#define SFP_DRV_SET_RX_EMPHASIS					LR4_301_set_rx_emphasis
#define SFP_DRV_SET_RX_AMPLITUDE				LR4_301_set_rx_amplitude
#define SFP_DRV_SET_CDR_SQ_DISABLE      		LR4_301_set_cdr_sq_disable
#define SFP_DRV_SET_RX_OUTPUT_DISABLE			LR4_301_set_rx_output_disable
#define SFP_DRV_SET_TX_ADAPT_EQ					LR4_301_set_tx_adapt_eq
#define SFP_DRV_UPDATE_DEV_READY_STATUS         LR4_301_update_dev_ready_status

#endif

#ifdef EFM8LB12E64_100G_SFP_CWDM4_202
#define CHIP_RESET								CWDM4_202_chip_reset
#define CHIP_INIT								CWDM4_202_chip_init
#define PORT_INIT								CWDM4_202_port_init
#define DDM_GET_TEMP							CWDM4_202_ddmi_get_temp
#define DDM_GET_VCC								CWDM4_202_ddmi_get_vcc
#define DDM_GET_BIAS							CWDM4_202_ddmi_get_bias
#define DDM_GET_RX_POWER						CWDM4_202_ddmi_get_rx_power
#define DDM_GET_TX_POWER						CWDM4_202_ddmi_get_tx_power
#define SFP_DRV_POLL							CWDM4_202_drv_poll
#define SFP_DRV_ADJUST_LASER_CH					CWDM4_202_drv_adjust_laser_ch
#define SFP_DRV_GET_LASER_FULL_STATE			CWDM4_202_drv_get_laser_full_state
#define SFP_DRV_CH_CALCULATE_CFG				CWDM4_202_drv_ch_calc_cfg
#define SFP_DRV_SET_LOW_POWER					CWDM4_202_set_low_power
#define SFP_DRV_SET_TX_ENABLE					CWDM4_202_set_tx_enable
#define SFP_DRV_SET_TX_BYPASS					CWDM4_202_set_tx_bypass
#define SFP_DRV_SET_RX_BYPASS					CWDM4_202_set_rx_bypass
#define SFP_DRV_INTR_STATUS			    		CWDM4_202_get_drv_intr_status
#define SFP_DRV_SET_CDR_CONTROL         		CWDM4_202_set_cdr_control
#define SFP_DRV_SET_TX_EQ						CWDM4_202_set_tx_eq
#define SFP_DRV_SET_RX_EMPHASIS					CWDM4_202_set_rx_emphasis
#define SFP_DRV_SET_RX_AMPLITUDE				CWDM4_202_set_rx_amplitude
#define SFP_DRV_SET_CDR_SQ_DISABLE      		CWDM4_202_set_cdr_sq_disable
#define SFP_DRV_SET_RX_OUTPUT_DISABLE			CWDM4_202_set_rx_output_disable
#define SFP_DRV_SET_TX_ADAPT_EQ					CWDM4_202_set_tx_adapt_eq
#define SFP_DRV_UPDATE_DEV_READY_STATUS         CWDM4_202_update_dev_ready_status
#endif

#ifdef EFM8LB12E64_100G_SFP_CWDM4_206
#define CHIP_RESET								CWDM4_206_chip_reset
#define CHIP_INIT								CWDM4_206_chip_init
#define PORT_INIT								CWDM4_206_port_init
#define DDM_GET_TEMP							CWDM4_206_ddmi_get_temp
#define DDM_GET_VCC								CWDM4_206_ddmi_get_vcc
#define DDM_GET_BIAS							CWDM4_206_ddmi_get_bias
#define DDM_GET_RX_POWER						CWDM4_206_ddmi_get_rx_power
#define DDM_GET_TX_POWER						CWDM4_206_ddmi_get_tx_power
#define SFP_DRV_POLL							CWDM4_206_drv_poll
#define SFP_DRV_ADJUST_LASER_CH					CWDM4_206_drv_adjust_laser_ch
#define SFP_DRV_GET_LASER_FULL_STATE			CWDM4_206_drv_get_laser_full_state
#define SFP_DRV_CH_CALCULATE_CFG				CWDM4_206_drv_ch_calc_cfg
#define SFP_DRV_SET_LOW_POWER					CWDM4_206_set_low_power
#define SFP_DRV_SET_TX_ENABLE					CWDM4_206_set_tx_enable
#define SFP_DRV_SET_TX_BYPASS					CWDM4_206_set_tx_bypass
#define SFP_DRV_SET_RX_BYPASS					CWDM4_206_set_rx_bypass
#define SFP_DRV_INTR_STATUS			    		CWDM4_206_get_drv_intr_status
#define SFP_DRV_SET_CDR_CONTROL         		CWDM4_206_set_cdr_control
#define SFP_DRV_SET_TX_EQ						CWDM4_206_set_tx_eq
#define SFP_DRV_SET_RX_EMPHASIS					CWDM4_206_set_rx_emphasis
#define SFP_DRV_SET_RX_AMPLITUDE				CWDM4_206_set_rx_amplitude
#define SFP_DRV_SET_CDR_SQ_DISABLE      		CWDM4_206_set_cdr_sq_disable
#define SFP_DRV_SET_RX_OUTPUT_DISABLE			CWDM4_206_set_rx_output_disable
#define SFP_DRV_SET_TX_ADAPT_EQ					CWDM4_206_set_tx_adapt_eq
#define SFP_DRV_UPDATE_DEV_READY_STATUS         CWDM4_206_update_dev_ready_status

#endif

#ifdef EFM8LB12E64_100G_SFP_CWDM4_207
#define CHIP_RESET								CWDM4_207_chip_reset
#define CHIP_INIT								CWDM4_207_chip_init
#define PORT_INIT								CWDM4_207_port_init
#define DDM_GET_TEMP							CWDM4_207_ddmi_get_temp
#define DDM_GET_VCC								CWDM4_207_ddmi_get_vcc
#define DDM_GET_BIAS							CWDM4_207_ddmi_get_bias
#define DDM_GET_RX_POWER						CWDM4_207_ddmi_get_rx_power
#define DDM_GET_TX_POWER						CWDM4_207_ddmi_get_tx_power
#define SFP_DRV_POLL							CWDM4_207_drv_poll
#define SFP_DRV_ADJUST_LASER_CH					CWDM4_207_drv_adjust_laser_ch
#define SFP_DRV_GET_LASER_FULL_STATE			CWDM4_207_drv_get_laser_full_state
#define SFP_DRV_CH_CALCULATE_CFG				CWDM4_207_drv_ch_calc_cfg
#define SFP_DRV_SET_LOW_POWER					CWDM4_207_set_low_power
#define SFP_DRV_SET_TX_ENABLE					CWDM4_207_set_tx_enable
#define SFP_DRV_SET_TX_BYPASS					CWDM4_207_set_tx_bypass
#define SFP_DRV_SET_RX_BYPASS					CWDM4_207_set_rx_bypass
#define SFP_DRV_INTR_STATUS			    		CWDM4_207_get_drv_intr_status
#define SFP_DRV_SET_CDR_CONTROL         		CWDM4_207_set_cdr_control
#define SFP_DRV_SET_TX_EQ						CWDM4_207_set_tx_eq
#define SFP_DRV_SET_RX_EMPHASIS					CWDM4_207_set_rx_emphasis
#define SFP_DRV_SET_RX_AMPLITUDE				CWDM4_207_set_rx_amplitude
#define SFP_DRV_SET_CDR_SQ_DISABLE      		CWDM4_207_set_cdr_sq_disable
#define SFP_DRV_SET_RX_OUTPUT_DISABLE			CWDM4_207_set_rx_output_disable
#define SFP_DRV_SET_TX_ADAPT_EQ					CWDM4_207_set_tx_adapt_eq
#define SFP_DRV_UPDATE_DEV_READY_STATUS         CWDM4_207_update_dev_ready_status

#endif

#ifdef EFM8LB12E64_100G_SFP_CWDM4_211
#define CHIP_RESET								CWDM4_211_chip_reset
#define CHIP_INIT								CWDM4_211_chip_init
#define PORT_INIT								CWDM4_211_port_init
#define DDM_GET_TEMP							CWDM4_211_ddmi_get_temp
#define DDM_GET_VCC								CWDM4_211_ddmi_get_vcc
#define DDM_GET_BIAS							CWDM4_211_ddmi_get_bias
#define DDM_GET_RX_POWER						CWDM4_211_ddmi_get_rx_power
#define DDM_GET_TX_POWER						CWDM4_211_ddmi_get_tx_power
#define SFP_DRV_POLL							CWDM4_211_drv_poll
#define SFP_DRV_ADJUST_LASER_CH					CWDM4_211_drv_adjust_laser_ch
#define SFP_DRV_GET_LASER_FULL_STATE			CWDM4_211_drv_get_laser_full_state
#define SFP_DRV_CH_CALCULATE_CFG				CWDM4_211_drv_ch_calc_cfg
#define SFP_DRV_SET_LOW_POWER					CWDM4_211_set_low_power
#define SFP_DRV_SET_TX_ENABLE					CWDM4_211_set_tx_enable
#define SFP_DRV_SET_TX_BYPASS					CWDM4_211_set_tx_bypass
#define SFP_DRV_SET_RX_BYPASS					CWDM4_211_set_rx_bypass
#define SFP_DRV_INTR_STATUS			    		CWDM4_211_get_drv_intr_status
#define SFP_DRV_SET_CDR_CONTROL         		CWDM4_211_set_cdr_control
#define SFP_DRV_SET_TX_EQ						CWDM4_211_set_tx_eq
#define SFP_DRV_SET_RX_EMPHASIS					CWDM4_211_set_rx_emphasis
#define SFP_DRV_SET_RX_AMPLITUDE				CWDM4_211_set_rx_amplitude
#define SFP_DRV_SET_CDR_SQ_DISABLE      		CWDM4_211_set_cdr_sq_disable
#define SFP_DRV_SET_RX_OUTPUT_DISABLE			CWDM4_211_set_rx_output_disable
#define SFP_DRV_SET_TX_ADAPT_EQ					CWDM4_211_set_tx_adapt_eq
#define SFP_DRV_UPDATE_DEV_READY_STATUS         CWDM4_211_update_dev_ready_status

#endif


#endif
