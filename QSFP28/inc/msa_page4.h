/*
 * msa_page4.h
 *
 *  Created on: 2017-10-4
 *      Author: cuilin
 */

#ifndef MSA_PAGE4_H_
#define MSA_PAGE4_H_

#ifdef MSA_PAGE_4


#define MSA_PAGE4_PIN_STATUS				(0xB4)
	#define MSA_PAGE4_PIN_STATUS_MODSEL		0x01
	#define MSA_PAGE4_PIN_STATUS_RESET		0x02
	#define MSA_PAGE4_PIN_STATUS_LPMODE		0x04
	#define MSA_PAGE4_PIN_STATUS_TXDIS		0x08
	#define MSA_PAGE4_PIN_STATUS_INTL		0x10
	#define MSA_PAGE4_PIN_STATUS_P3V3EN		0x20
	#define MSA_PAGE4_PIN_STATUS_TECEN		0x40
	#define MSA_PAGE4_PIN_STATUS_P1V8EN		0x80
#define MSA_PAGE4_TEC_TEMP_READBACK_MSB		(0xB5)
#define MSA_PAGE4_TEC_TEMP_READBACK_LSB		(0xB6)

#define MSA_PAGE4_TEC_TEMP_SET_MSB			(0xB7)
#define MSA_PAGE4_TEC_TEMP_SET_LSB			(0xB8)

#define MSA_PAGE4_ITEC_MSB					(0xB9)
#define MSA_PAGE4_ITEC_LSB					(0xBA)

#define MSA_PAGE4_VTEC_MSB					(0xBB)
#define MSA_PAGE4_VTEC_LSB					(0xBC)

#define MSA_PAGE4_MCU_RESET_REASON			(0xBD)

#define MSA_PAGE4_DEBUG_CMD					(0xDB)
#define MSA_PAGE4_DEBUG_CMD_DATA_LEN		(0xDC)
	#define MSA_PAGE4_DEBUG_CMD_LEN_MASK	(0xF0)

#define MSA_PAGE4_DEBUG_DEV_BASE			(0xDD)
#define MSA_PAGE4_DEBUG_DEV_OFFSET			(0xDE)

#define MSA_PAGE4_DEBUG_PAGE_NUM			(0xDF)
	#define MSA_PAGE4_DEBUG_OFFSET_MASK		(0x0F)
	#define MSA_PAGE4_DEBUG_PAGE_MASK		(0xF0)

#define MSA_PAGE4_DEBUG_DATA0				(0xE0)
#define MSA_PAGE4_DEBUG_DATA1				(0xE1)
#define MSA_PAGE4_DEBUG_DATA2				(0xE2)
#define MSA_PAGE4_DEBUG_DATA3				(0xE3)
#define MSA_PAGE4_DEBUG_DATA4				(0xE4)
#define MSA_PAGE4_DEBUG_DATA5				(0xE5)
#define MSA_PAGE4_DEBUG_DATA6				(0xE6)
#define MSA_PAGE4_DEBUG_DATA7				(0xE7)
#define MSA_PAGE4_DEBUG_DATA8				(0xE8)
#define MSA_PAGE4_DEBUG_DATA9				(0xE9)
#define MSA_PAGE4_DEBUG_DATA10				(0xEA)
#define MSA_PAGE4_DEBUG_DATA11				(0xEB)
#define MSA_PAGE4_DEBUG_DATA12				(0xEC)
#define MSA_PAGE4_DEBUG_DATA13				(0xED)
#define MSA_PAGE4_DEBUG_DATA14				(0xEE)
#define MSA_PAGE4_DEBUG_DATA15				(0xEF)

#define MSA_PAGE4_DDMI_POLL					(0xF1)
#define MSA_PAGE4_LUT_TAB_POLL				(0xF2)
#define MSA_PAGE4_BUILD_SECOND				(0xF3)
#define MSA_PAGE4_BUILD_MINUTE				(0xF4)
#define MSA_PAGE4_BUILD_HOUR				(0xF5)
#define MSA_PAGE4_BUILD_DAY 				(0xF6)
#define MSA_PAGE4_BUILD_MONTH				(0xF7)
#define MSA_PAGE4_BUILD_YEAR				(0xF8)

#define MSA_PAGE4_VERSION_HI				(0xF9)
#define MSA_PAGE4_VERSION_LO				(0xFA)
#define MSA_PAGE4_BOOT_STATUS				(0xFB)


void		MSA_p4_init(QSFP_MSA_PAGE_T *page);

void     	MSA_p4_debug_cmd_on_poll(void);

void 	    MSA_p4_on_poll(void);

uint8_t 	MSA_p4_update_hw_ddmi(void);

uint8_t 	MSA_p4_read_by_isr(uint8_t offset);

uint8_t    	MSA_p4_write_by_isr(uint8_t offset, uint8_t value);

uint8_t 	MSA_p4_get_auto_poll(void);

uint8_t 	MSA_p4_get_ddmi_poll(void);

MSA_LUT_T *	MSA_p4_get_laser_state(void);

#endif

#endif /* MSA_PAGE4_H_ */
