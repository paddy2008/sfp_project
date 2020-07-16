#ifndef __100g_sfp_EFM8LB1_eep_interface_h_
#define __100g_sfp_EFM8LB1_eep_interface_h_

#include <SI_EFM8LB1_Register_Enums.h>

typedef uint16_t FLADDR;

#define  FLASH_PAGE_SIZE  (uint16_t)0x200  /* Page size = 512Bytes */

uint8_t  FLASH_ByteRead (FLADDR addr) ;

uint8_t *FLASH_Read (uint8_t *dest, FLADDR src, uint16_t numbytes) ;

void 	 FLASH_PageErase (FLADDR addr) ;

void 	 FLASH_ByteWrite (FLADDR addr, uint8_t byte) ;

void 	 FLASH_Write (FLADDR dest, uint8_t *src, uint16_t numbytes) ;

void 	 flash_erase_range(FLADDR from, uint16_t len) ;

void 	 flash_write_byte(FLADDR addr, uint8_t value) ;

void 	 flash_write_multi_byte (FLADDR dest, uint8_t *src, uint16_t numbytes) ;

uint8_t * FLASH_Read_by_i2c_slave_isr (uint8_t *dest, FLADDR src, uint16_t numbytes);

uint8_t  FLASH_ByteRead_by_i2c_slave_isr (FLADDR addr);



#endif
