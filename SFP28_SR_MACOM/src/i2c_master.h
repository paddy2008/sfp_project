//-----------------------------------------------------------------------------------
//      Copyright (c) 2005-2018,
//-----------------------------------------------------------------------------------
//
// file name  :       i2c_master.h
//
// description:       I2C Master Driver
//
// Target     :       EFM8LB1
//
// version    :       V100.001
//
//
//-----------------------------------------------------------------------------------

#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#define   I2C_Master_Speed             100  // KHZ
#define   Rise_Time_Delay              2
#define   I2C_Master_HoldTime          10
#define   I2C_Master_Delay             ((int)( 500 / I2C_Master_Speed - Rise_Time_Delay ))
#define   I2C_Master_HaftCycle         ((int)( 500 / I2C_Master_Speed ))


#define   SCL_Stretch_Timeout          8                    // SCL最多延迟8个半周期，即4个周期

#define   SCL_HIZ()                    Port_MSCL = 1       // 设置SCL为高阻态
#define   SCL_L()                      Port_MSCL = 0       // 设置SCL为低
#define   Get_SCL()                    Port_MSCL


#define   SDA_HIZ()                    Port_MSDA = 1       // 设置SDA为高阻态
#define   SDA_L()                      Port_MSDA = 0       // 设置SDA为低
#define   SDA_OUT(b)                   if(b){SDA_HIZ();}else{SDA_L();}
#define   Get_SDA()                    Port_MSDA



//------------------------- Internal Function ------------------------/
uint8_t I2C_Master_start           (void);
uint8_t I2C_Master_stop            (void);
uint8_t I2C_Master_bit_out         (uint8_t bit_out);       // send one bit
uint8_t I2C_Master_bit_in          (uint8_t *bit_in);       // receive one bit
uint8_t I2C_Master_byte_out        (uint8_t byte_out);      // send one byte
uint8_t I2C_Master_byte_in         (uint8_t ack_out, uint8_t *byte_in); // receive one byte and return ACK or NOACK

//--------------------- I2C BUS Init Function -----------------------/
extern   uint8_t   I2C_Master_Init         (void);

//----------------- Single Byte address & Single Byte Data Function -----------------------/
extern   uint8_t   I2C_Master_write_byte   (uint8_t device, uint8_t address, uint8_t buf);
extern   uint8_t   I2C_Master_read_byte    (uint8_t device, uint8_t address, uint8_t *buf);

extern   uint8_t   I2C_Master_writeBytes   (uint8_t device,
                                            uint8_t address,
                                            uint16_t length,
                                            uint8_t *bufPtr);

extern   uint8_t   I2C_Master_readBytes    (uint8_t device,
                                            uint8_t address,
                                            uint16_t length,
                                            uint8_t *bufPtr);

//----------------- Single Byte address & Word Data Function  ---------------------------/
extern   uint8_t   I2C_Master_write_word  (uint8_t device, uint8_t address, uint16_t  buf);
extern   uint8_t   I2C_Master_read_word   (uint8_t device, uint8_t address, uint16_t  *buf);


//----------------------------- word register address Function ---------------------------------------/
extern   uint8_t   I2C_Master_DRegAdr_readBytes ( uint8_t device,
                                            	  uint16_t address,
												  uint16_t length,
												  uint8_t *bufPtr);

extern   uint8_t   I2C_Master_DRegAdr_writeBytes ( uint8_t device,
                                            	   uint16_t address,
												   uint16_t length,
												   uint8_t *bufPtr);

#endif
