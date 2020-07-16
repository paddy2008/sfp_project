#ifndef _MATA_37644B_USER_H
#define _MATA_37644B_USER_H

#define MATA_37644B_ADDR  0x1C

#define ADDR_RXSETOMALOS  0x36
#define ADDR_RXOMALOSHYST 0x37

#define ADDR_RXSETOMALOS  0x36
#define ADDR_RXOMALOSHYST 0x37

#define ADDR_TXFDTCTRL1   0x44

#define ADDR_SETBIAS      0x4D
#define ADDR_SETMOD       0x4E

#define ADDR_XCVRCTRL     0x54

#define ADDR_RXSTAT2      0x5A
#define ADDR_TXSTAT2      0x61


#define  I2C_MATA_37644B_Write_Byte( address, buf )  I2C_Master_write_byte( ((uint8_t)MATA_37644B_ADDR), address, buf )
#define  I2C_MATA_37644B_Read_Byte( address, buf )   I2C_Master_read_byte(  ((uint8_t)MATA_37644B_ADDR), address, buf )


#define  I2C_MATA_37644B_Write_Word( address, buf )  I2C_Master_write_word( ((uint8_t)MATA_37644B_ADDR), address, buf )
#define  I2C_MATA_37644B_Read_Word( address, buf )   I2C_Master_read_word( ((uint8_t)MATA_37644B_ADDR),  address, buf )

#endif
