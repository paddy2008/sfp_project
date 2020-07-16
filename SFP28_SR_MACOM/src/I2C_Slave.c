
#include "include.h"

//-----------------------------------------------------------------------------
// InitI2cSlave
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures    Init I2C SLAVE
//
//-----------------------------------------------------------------------------
void  I2C_Slave_Init (void)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE  = 0x20;

	//set slave addr
	I2C0SLAD = ((0xA0>>1) << I2C0SLAD_I2C0SLAD__SHIFT);
	I2C0ADM  = 0x8E;

	//Set the I2C0 Slave pins in I2C mode and Enable I2C SCL low timeout detection using Timer 4
	//when the SCL is high,Timer4 reload value;when the SCL is low,Timer4 start to count
	I2C0CN0 |= I2C0CN0_PINMD__I2C_MODE | I2C0CN0_TIMEOUT__ENABLED | I2C0CN0_PINDRV__HIGH_DRIVE | I2C0CN0_ADDRCHK__ENABLED;

	//Enable the I2C0 Slave module.
	I2C0CN0 |= I2C0CN0_I2C0EN__ENABLED;
	I2C0FCN0 = I2C0FCN0_RFRQE__ENABLED; // | I2C0FCN0_TFRQE__ENABLED;

	SFRPAGE  = 0x0;
	EIE2    |= 0x02;   				    // enabel I2C slave interrupt

    SFRPAGE  = 0x10;
	EIP2    |= 0x02;				   	// Make i2c slave  high priority3,priority3 is the highest priority!
    EIP2H   |= 0x02;

	SFRPAGE   = 0x20;
	I2C0CN0  &= ~I2C0CN0_BUSY__BMASK;   // Clear BUSY bit,and Device will acknowledge an I2C master ack
	I2C0FCN0 |= I2C0FCN0_RFLSH__FLUSH | I2C0FCN0_TFLSH__FLUSH;   //These bits flushes the RX/TX FIFO,When firmware sets this bit to 1, the internal FIFO counters will be reset, and any remaining data will be lost

	SFRPAGE = SFRPAGE_SAVE;
}

//-----------------------------------------------------------------------------
// InitI2cSlave
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures    Init I2C SLAVE
//
//-----------------------------------------------------------------------------
void I2C_slave_enable( bool enable )
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE  = 0x20;

    I2C0CN0  &= ~I2C0CN0_BUSY__BMASK;		// Clear BUSY bit
    I2C0FCN0 |= I2C0FCN0_RFLSH__FLUSH | I2C0FCN0_TFLSH__FLUSH;

	if( enable )  //module is not selected
	{
		//I2C0CN0 &= ~I2C0CN0_PINMD__I2C_MODE;	//set i2c slave pin to gpio mode
		I2C0CN0 &= ~I2C0CN0_I2C0EN__ENABLED;	//disable i2c slave
	}
	else  // module is selected
	{
		I2C0CN0 |= I2C0CN0_PINMD__I2C_MODE;  //set i2c slave pin to i2c mode
		I2C0CN0 |= I2C0CN0_I2C0EN__ENABLED;  //enable i2c slave
	}
	SFRPAGE = SFRPAGE_SAVE;
}


//-----------------------------------------------------------------------------
// I2C0_ISR
//-----------------------------------------------------------------------------
//
// I2C0 ISR Content goes here. Remember to clear flag bits:
// I2C0STAT::I2C0INT (I2C Interrupt)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT (I2C0_ISR, I2C0_IRQn)
{
	//  Number of bytes in the transmit FIFO and shift register
	uint8_t i2c_status;

	i2c_status = I2C0STAT;

	switch (i2c_status & I2C_STATUS_VECTOR_MASK)
	{
		case I2C_ADDR_RD:  // START+ADDR+R
			I2C0STAT &= ~(I2C0STAT_START__BMASK | I2C0STAT_RD__BMASK);
			I2c_Slave_Process_Address_Read(); // Write data to buffer for transmitting
			break;

		case I2C_ADDR_WR: // START+ADDR+W
			I2C0STAT &= ~(I2C0STAT_START__BMASK | I2C0STAT_WR__BMASK);
			I2c_Slave_Process_Address_Write();
			break;

		case I2C_RD_DATA:
			// Check for NACK
			if ((i2c_status & I2C0STAT_NACK__BMASK) == I2C0STAT_NACK__SET)
			{
				I2C0STAT &= ~I2C0STAT_NACK__BMASK;
				I2c_Slave_Process_Data_Read_NACK();
			}
			else
			{
				I2c_Slave_Process_Data_Read_ACK();
			}
			I2C0STAT &= ~I2C0STAT_RD__BMASK;
			break;

		case I2C_WR_DATA:  // FIFO is full, whatever slave ACK/NACK master
			while((I2C0FCN1 & I2C0FCN1_RXE__BMASK) == I2C0FCN1_RXE__NOT_EMPTY)
			{
				I2c_Slave_Process_Data_Write();
			}
			I2C0STAT &= ~I2C0STAT_WR__BMASK;
			break;

		case I2C_STOSTA:
		case I2C_STOSTARD:
		case I2C_STOSTAWR:
		case I2C_STO:
			I2C0STAT &= ~I2C0STAT_STOP__BMASK;
			I2C0STAT &= ~I2C0STAT_I2C0INT__BMASK;
			I2C0CN0  &= ~I2C0CN0_BUSY__BMASK;
			I2C0FCN0 |= I2C0FCN0_RFRQE__ENABLED;
			//
			//	STOP Condition received.
			//
			I2c_Slave_Process_Stop();
			break;
		default:
			while((I2C0FCN1 & I2C0FCN1_RXE__BMASK) == I2C0FCN1_RXE__NOT_EMPTY)
			{
				I2c_Slave_Process_Data_Write();
			}
			break;
	}

	// Clear I2C interrupt flag
	I2C0STAT &= ~I2C0STAT_I2C0INT__BMASK;
}

//-----------------------------------------------------------------------------
// TIMER4_ISR
//-----------------------------------------------------------------------------
//
// TIMER4 ISR Content goes here. Remember to clear flag bits:
// TMR4CN0::TF4H (Timer # High Byte Overflow Flag)
// TMR4CN0::TF4L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT (TIMER4_ISR, TIMER4_IRQn)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = PG3_PAGE;

	I2C0CN0 &= ~I2C0CN0_I2C0EN__BMASK;			// Disable I2C module
	I2C0CN0 |= I2C0CN0_I2C0EN__ENABLED;			// Re-enable I2C module

	SFRPAGE = PG2_PAGE;
	// TMR4CN0 &= ~TMR4CN0_TF4H__BMASK;			// Clear Timer4 interrupt-pending flag
	TMR4CN0 &= ~0xC0;

	SFRPAGE = SFRPAGE_SAVE;
}



















