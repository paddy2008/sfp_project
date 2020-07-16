#ifndef __100g_sfp_EFM8LB1_smbus_h__
#define __100g_sfp_EFM8LB1_smbus_h__

#include <SI_EFM8LB1_Register_Enums.h>

typedef enum{
	WRITE = 0x00,
	READ  = 0x01,
}OPT;

typedef enum{
	SMBFREE = 0x00,
	SMBBUSY = 0x01,
}SMBSTATE;

typedef enum
{
	SMB_MASTER_START  = 0xE0,   ///!< start transmitted (master)
	SMB_MASTER_TXDATA = 0xC0,   ///!< data byte transmitted (master)
	SMB_MASTER_RXDATA = 0x80,   ///!< data byte received (master)
	SMB_SLAVE_ADDRESS = 0x20,   ///!< slave address received (slave)
	SMB_SLAVE_RX_STOP = 0x10,   ///!< STOP detected during write (slave)
	SMB_SLAVE_RXDATA  = 0x00,   ///!< data byte received (slave)
	SMB_SLAVE_TXDATA  = 0x40,   ///!< data byte transmitted (slave)
	SMB_SLAVE_TX_STOP = 0x50,   ///!< STOP detected during a write (slave)
} I2C0_State_t;

// Status vector - top 4 bits only
#define  SMB_MTSTA      (0xE0)           // (MT) start transmitted,MT replace Master and Transmitter
#define  SMB_MTDB       (0xC0)           // (MT) data byte transmitted
#define  SMB_MRDB       (0x80)           // (MR) data byte received

void 	   SMB_reset(void);

void 	   SMB_init(void);

uint8_t    SMB_check_bus_status(void);

uint8_t    SMB_write_byte(uint8_t BaseAddr,uint16_t OffsetAddr,uint8_t Data);

uint8_t    SMB_write_multi_byte(uint8_t BaseAddr,uint16_t OffsetAddr,uint8_t *Pdata,uint8_t Length);

uint8_t    SMB_read_byte(uint8_t BaseAddr,uint16_t OffsetAddr,uint8_t *Data);

uint8_t    SMB_read_multi_byte(uint8_t BaseAddr,uint16_t OffsetAddr,uint8_t *Pdata,uint8_t Length);

uint8_t    SMB_read_byte_by_isr(uint8_t BaseAddr,uint16_t OffsetAddr,uint8_t *Pdata,uint8_t Length);

uint8_t	   SMB_write_byte_by_isr(uint8_t BaseAddr,uint16_t OffsetAddr,uint8_t *Data,uint8_t Length);

#endif

