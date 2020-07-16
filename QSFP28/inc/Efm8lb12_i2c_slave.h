#ifndef __100g_sfp_EFM8LB1_i2c_slave_h_
#define __100g_sfp_EFM8LB1_i2c_slave_h_

#include <SI_EFM8LB1_Register_Enums.h>

// Device addresses (7 bits, lsb is a don't care).Set (SLAVE_ADDR>>1) to sfr I2C0SLAD
#define     MCU_SLAVE_ADDR   (0xA0)

// I2C States
#define  I2C_STATUS_VECTOR_MASK  (0x0F)   // NACK, START, STOP, WR, RD
#define  I2C_ADDR_RD    (0x09)			// Valid Slave Address + Master Read Request
#define  I2C_ADDR_WR    (0x0A)			// Valid Slave Address + Master Write Request
#define  I2C_RD_DATA    (0x01)			// Transfer data from Slave (Also can check B4 to see what ACK we just received from master)
#define  I2C_WR_DATA    (0x02)			// Write data to Slave (Also can check B4 to see what ACK(ACK/NACK) we just sent)
#define  I2C_STO        (0x04)
#define  I2C_START      (0x08)
#define  I2C_STOSTA     (0x0C)
#define  I2C_STOSTARD   (0x0D)
#define  I2C_STOSTAWR   (0x0E)

void I2C_slave_init(void);
void I2C_slave_enable(uint8_t enable) reentrant;

bool I2C_slave_get_status(void);

#endif
