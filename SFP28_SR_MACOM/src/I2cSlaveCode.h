#ifndef I2C_SLAVE_CODE_H
#define I2C_SLAVE_CODE_H


extern  void I2c_Slave_Process_Address_Write(void);
extern  void I2c_Slave_Process_Address_Read(void);

extern  void I2c_Slave_Process_Data_Write (void);
extern  void I2c_Slave_Process_Data_Read_ACK (void);
extern  void I2c_Slave_Process_Data_Read_NACK (void);

extern  void I2c_Slave_Process_Stop (void);
extern  void I2c_Slave_Process_BusError(void);

#endif
