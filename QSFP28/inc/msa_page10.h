/*
 * msa_page9.h
 *
 *  Created on: 2017-11-16
 *      Author: cuilin
 */

#ifndef MSA_PAGE10_H_
#define MSA_PAGE10_H_

#ifdef MSA_PAGE_10

typedef struct {
	uint8_t		dev_addr;
	uint8_t		value;
	uint16_t	offset;
}MSA_REG_SET_T;

void 	MSA_p10_eeprom_init(void);

void 	MSA_p10_regdata_set(MSA_REG_SET_T *reg);

void 	MSA_p10_init(QSFP_MSA_PAGE_T *page);

uint8_t MSA_p10_write_by_isr(uint8_t offset, uint8_t value);

uint8_t MSA_p10_read_by_isr(uint8_t offset);

#endif

#endif /* MSA_PAGE9_H_ */
