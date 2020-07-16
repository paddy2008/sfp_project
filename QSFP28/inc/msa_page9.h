/*
 * msa_PAGE9.h
 *
 *  Created on: 2018-3-18
 *      Author: cuilin
 */

#ifndef MSA_PAGE9_H_
#define MSA_PAGE9_H_

#ifdef MSA_PAGE_9

#define PREV_AGING_MAX_DURATION  (0xFFFF)
#define ONE_BYTE_INVALID   		 (0xFF)
#define AGING_MAX_COUNT	   		 (0xFE)

typedef enum {
	MSA_AGING_NOT_RUN			= 0,
	MSA_AGING_LOW_TEMP_RUN		= 1,
	MSA_AGING_RUNNING			= 2,
	MSA_AGING_SUCCESS			= 3,
	MSA_AGING_FAIL				= 0xF0,
}MSA_AGING_STATE;

typedef enum {
	MSA_AGING_MOD_FAIL_NONE			= 0x00,
	MSA_AGING_MOD_FAIL_HI_TEMP 		= 0x01,
	MSA_AGING_MOD_FAIL_HI_VCC		= 0x02,
	MSA_AGING_MOD_FAIL_LO_VCC		= 0x04,
}MSA_AGING_MOD_FAIL_REASON;

typedef enum {
	MSA_AGING_CH_FAIL_NONE			= 0x00,
	MSA_AGING_CH_FAIL_HI_BIAS 		= 0x01,
	MSA_AGING_CH_FAIL_LO_BIAS 		= 0x02,
	MSA_AGING_CH_FAIL_HI_RSSI 		= 0x04,
	MSA_AGING_CH_FAIL_LO_RSSI 		= 0x08,
	MSA_AGING_CH_FAIL_HI_MPD 		= 0x10,
	MSA_AGING_CH_FAIL_LO_MPD 		= 0x20,		
}MSA_AGING_CH_FAIL_REASON;

typedef struct{
	uint16_t max;
	uint16_t min;
}UINT16_MAX_MIN_T;

typedef struct{
	int16_t max;
	int16_t min;
}INT16_MAX_MIN_T;

typedef struct{
	uint8_t temp_vcc;
	uint8_t ibias;
	uint8_t rssi;
	uint8_t mpd;
}ERROR_REASON_T;

typedef struct {
	INT16_MAX_MIN_T  temp;
	UINT16_MAX_MIN_T vcc;
	UINT16_MAX_MIN_T ibias[4];
	UINT16_MAX_MIN_T rssi[4];
	UINT16_MAX_MIN_T mpd[4];
}MSA_AGING_DETE_THRESHOLD_T;

#define MSA_PAGE9_AGE_COUNT				(0x80)
#define MSA_PAGE9_AGE_RESULT			(0x81)
#define MSA_PAGE9_AGE_TEMP_VCC_FAIL		(0x82)
#define MSA_PAGE9_AGE_IBIAS_FAIL		(0x83)
#define MSA_PAGE9_AGE_RSSI_FAIL			(0x84)
#define MSA_PAGE9_AGE_MPD_FAIL			(0x85)


#define MSA_PAGE9_PREV_AGE_DURA_MSB		(0x86)
#define MSA_PAGE9_PREV_AGE_DURA_LSB		(0x87)
#define MSA_PAGE9_AGE_START_YEAR		(0x88)
#define MSA_PAGE9_AGE_START_MON			(0x89)
#define MSA_PAGE9_AGE_START_DAY			(0x8A)
#define MSA_PAGE9_AGE_START_HOUR		(0x8B)
#define MSA_PAGE9_AGE_START_MINU		(0x8C)

#define MSA_PAGE9_AGE_END_YEAR			(0x8D)
#define MSA_PAGE9_AGE_END_MON			(0x8E)
#define MSA_PAGE9_AGE_END_DAY			(0x8F)
#define MSA_PAGE9_AGE_END_HOUR			(0x90)
#define MSA_PAGE9_AGE_END_MINU			(0x91)

#define MSA_PAGE9_HI_TEMP_MSB			(0x92)
#define MSA_PAGE9_HI_TEMP_LSB			(0x93)
#define MSA_PAGE9_LO_TEMP_MSB			(0x94)
#define MSA_PAGE9_LO_TEMP_LSB			(0x95)

#define MSA_PAGE9_HI_VCC_MSB			(0x96)
#define MSA_PAGE9_HI_VCC_LSB			(0x97)
#define MSA_PAGE9_LO_VCC_MSB			(0x98)
#define MSA_PAGE9_LO_VCC_LSB			(0x99)

#define MSA_PAGE9_CH0_HI_IBIAS_MSB		(0x9A)
#define MSA_PAGE9_CH0_HI_IBIAS_LSB		(0x9B)
#define MSA_PAGE9_CH0_LO_IBIAS_MSB		(0x9C)
#define MSA_PAGE9_CH0_LO_IBIAS_LSB		(0x9D)
#define MSA_PAGE9_CH1_HI_IBIAS_MSB		(0x9E)
#define MSA_PAGE9_CH1_HI_IBIAS_LSB		(0x9F)
#define MSA_PAGE9_CH1_LO_IBIAS_MSB		(0xA0)
#define MSA_PAGE9_CH1_LO_IBIAS_LSB		(0xA1)
#define MSA_PAGE9_CH2_HI_IBIAS_MSB		(0xA2)
#define MSA_PAGE9_CH2_HI_IBIAS_LSB		(0xA3)
#define MSA_PAGE9_CH2_LO_IBIAS_MSB		(0xA4)
#define MSA_PAGE9_CH2_LO_IBIAS_LSB		(0xA5)
#define MSA_PAGE9_CH3_HI_IBIAS_MSB		(0xA6)
#define MSA_PAGE9_CH3_HI_IBIAS_LSB		(0xA7)
#define MSA_PAGE9_CH3_LO_IBIAS_MSB		(0xA8)
#define MSA_PAGE9_CH3_LO_IBIAS_LSB		(0xA9)

#define MSA_PAGE9_CH0_HI_RSSI_MSB		(0xAA)
#define MSA_PAGE9_CH0_HI_RSSI_LSB		(0xAB)
#define MSA_PAGE9_CH0_LO_RSSI_MSB		(0xAC)
#define MSA_PAGE9_CH0_LO_RSSI_LSB		(0xAD)
#define MSA_PAGE9_CH1_HI_RSSI_MSB		(0xAE)
#define MSA_PAGE9_CH1_HI_RSSI_LSB		(0xAF)
#define MSA_PAGE9_CH1_LO_RSSI_MSB		(0xB0)
#define MSA_PAGE9_CH1_LO_RSSI_LSB		(0xB1)
#define MSA_PAGE9_CH2_HI_RSSI_MSB		(0xB2)
#define MSA_PAGE9_CH2_HI_RSSI_LSB		(0xB3)
#define MSA_PAGE9_CH2_LO_RSSI_MSB		(0xB4)
#define MSA_PAGE9_CH2_LO_RSSI_LSB		(0xB5)
#define MSA_PAGE9_CH3_HI_RSSI_MSB		(0xB6)
#define MSA_PAGE9_CH3_HI_RSSI_LSB		(0xB7)
#define MSA_PAGE9_CH3_LO_RSSI_MSB		(0xB8)
#define MSA_PAGE9_CH3_LO_RSSI_LSB		(0xB9)

#define MSA_PAGE9_CH0_HI_MPD_MSB		(0xBA)
#define MSA_PAGE9_CH0_HI_MPD_LSB		(0xBB)
#define MSA_PAGE9_CH0_LO_MPD_MSB		(0xBC)
#define MSA_PAGE9_CH0_LO_MPD_LSB		(0xBD)
#define MSA_PAGE9_CH1_HI_MPD_MSB		(0xBE)
#define MSA_PAGE9_CH1_HI_MPD_LSB		(0xBF)
#define MSA_PAGE9_CH1_LO_MPD_MSB		(0xC0)
#define MSA_PAGE9_CH1_LO_MPD_LSB		(0xC1)
#define MSA_PAGE9_CH2_HI_MPD_MSB		(0xC2)
#define MSA_PAGE9_CH2_HI_MPD_LSB		(0xC3)
#define MSA_PAGE9_CH2_LO_MPD_MSB		(0xC4)
#define MSA_PAGE9_CH2_LO_MPD_LSB		(0xC5)
#define MSA_PAGE9_CH3_HI_MPD_MSB		(0xC6)
#define MSA_PAGE9_CH3_HI_MPD_LSB		(0xC7)
#define MSA_PAGE9_CH3_LO_MPD_MSB		(0xC8)
#define MSA_PAGE9_CH3_LO_MPD_LSB		(0xC9)

#define MSA_PAGE9_AGE_STATE_START	MSA_PAGE9_AGE_COUNT
#define MSA_PAGE9_AGE_STATE_END		MSA_PAGE9_CH3_LO_MPD_LSB

void 	MSA_p9_init(QSFP_MSA_PAGE_T *page);

void 	MSA_p9_eeprom_init(void);

void 	MSA_p9_aging_eep_init(void);

void 	MSA_p9_update_prev_aging_total_time(void);

void 	MSA_p9_start_aging(void);

void 	MSA_p9_stop_aging(void);

void 	MSA_p9_on_poll(void);

uint8_t MSA_p9_read_by_isr(uint8_t offset);

uint8_t	MSA_p9_write_by_isr(uint8_t offset, uint8_t value);

#endif

#endif /* MSA_PAGE9_H_ */
