/*
 * msa_page2.h
 *
 *  Created on: 2017-10-13
 *      Author: cuilin
 */

#ifndef MSA_PAGE2_H_
#define MSA_PAGE2_H_

#if defined(SUPPORT_LOG_FUNC)
#define MSA_DBG_LOG_ENABLE	(1)
#define MSA_DBG_LOG_SIZE	MSA_PAGE_SIZE

void 	MSA_p2_log(uint8_t *fmt,...);

void 	MSA_p2_log_isr(uint8_t *fmt,...);

uint8_t MSA_p2_handler(MSA_ACTION action, uint8_t offset, uint8_t *pValue);

#endif

void 	MSA_p2_init(QSFP_MSA_PAGE_T *page);

void 	MSA_p2_eeprom_init(void);

uint8_t MSA_p2_read_by_isr(uint8_t offset);

uint8_t MSA_p2_write_by_isr(uint8_t offset, uint8_t value);


#endif /* MSA_PAGE2_H_ */
