#ifndef MSA_PAGE1_H_
#define MSA_PAGE1_H_

void 	MSA_p1_init(QSFP_MSA_PAGE_T *page);

void 	MSA_p1_eeprom_init(void);

uint8_t MSA_p1_read_by_isr(uint8_t offset);

uint8_t MSA_p1_write_by_isr(uint8_t offset, uint8_t value);


#endif /* MSA_PAGE1_H_ */
