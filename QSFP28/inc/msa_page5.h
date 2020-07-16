/*
 * msa_page5.h
 *
 *  Created on: 2017-10-28
 *      Author: cuilin
 */

#ifndef MSA_PAGE5_H_
#define MSA_PAGE5_H_

#ifdef MSA_PAGE_5

#define MSA_P5_MODULE_CLASS			(0xDE)
#define MSA_P5_INIT_EEP_SIZE		(0xE0)

#define MSA_P5_AGE_ENANLE_STATUS    (0xE9)
#define MSA_P5_MANU_AGE_CFG_START	(0xEA)

#define MSA_P5_AGING_CONFIG_SIZE 	(23)


typedef struct {
	uint16_t duration;
	int16_t  temp_hi_thrld;
	int16_t  temp_lo_thrld;
	uint16_t vcc_hi_thrld;
	uint16_t vcc_lo_thrld;
	uint16_t ibias_hi_thrld;
	uint16_t ibias_lo_thrld;
	uint16_t rssi_hi_thrld;
	uint16_t rssi_lo_thrld;
	uint16_t mpd_hi_thrld;
	uint16_t mpd_lo_thrld;
}MSA_MANU_AGE_CFG_T;

typedef enum {
	MSA_AGING_ITEM_TEMP		= 0x01,
	MSA_AGING_ITEM_VCC 		= 0x02,
	MSA_AGING_ITEM_IBIAS	= 0x04,
	MSA_AGING_ITEM_RSSI		= 0x08,
	MSA_AGING_ITEM_MPD		= 0x10,
}MSA_AGING_ITEM_ENABLE;

void 	MSA_p5_aging_clear(void);

void 	MSA_p5_eeprom_init(void);

void	MSA_p5_init(QSFP_MSA_PAGE_T *page);

uint8_t MSA_p5_read_by_isr(uint8_t offset);

uint8_t  MSA_p5_write_by_isr(uint8_t offset, uint8_t value);

MSA_MANU_AGE_CFG_T * MSA_p5_get_manu_age_cfg();

#endif

#endif /* MSA_PAGE5_H_ */
