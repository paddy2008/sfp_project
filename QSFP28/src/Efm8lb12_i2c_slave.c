#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_i2c_slave.h"
#include "Efm8lb12_task_sched.h"

volatile bool 	 rd_ready = false;
volatile bool 	 wr_ready = false;
volatile uint8_t gRevByteNum = 0;

void I2C_slave_init(void)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE  = 0x20;

	//set slave addr
	I2C0SLAD = ((MCU_SLAVE_ADDR >> 1) << I2C0SLAD_I2C0SLAD__SHIFT);

	I2C0ADM = (0x7F << I2C0ADM_SLVM__SHIFT) | I2C0ADM_FACS__FORCE_STRETCH;

	//I2C0CN0 &= ~I2C0CN0_PRELOAD__BMASK;

	//Set the I2C0 Slave pins in I2C mode and Enable I2C SCL low timeout detection using Timer 4
	//when the SCL is high,Timer4 reload value;when the SCL is low,Timer4 start to count
	I2C0CN0 |= I2C0CN0_PINMD__I2C_MODE | I2C0CN0_TIMEOUT__ENABLED | I2C0CN0_PINDRV__HIGH_DRIVE;

	//Enable the I2C0 Slave module.
	I2C0CN0 |= I2C0CN0_I2C0EN__ENABLED;

	I2C0FCN0 = I2C0FCN0_RFRQE__ENABLED; // | I2C0FCN0_TFRQE__ENABLED;

	SFRPAGE  = 0x0;

	EIE2 |= 0x02;   				//enabel I2C slave interrupt

    SFRPAGE  = 0x10;

	EIP2  |= 0x02;				   	// Make i2c slave  high priority3,priority3 is the highest priority!
    EIP2H |= 0x02;

	SFRPAGE = PG3_PAGE;
	I2C0CN0  &= ~I2C0CN0_BUSY__BMASK;   // Clear BUSY bit,and Device will acknowledge an I2C master ack
	I2C0FCN0 |= I2C0FCN0_RFLSH__FLUSH | I2C0FCN0_TFLSH__FLUSH;   //These bits flushes the RX/TX FIFO,When firmware sets this bit to 1, the internal FIFO counters will be reset, and any remaining data will be lost

	SFRPAGE   = SFRPAGE_SAVE;

	//I2C0 interrupts will be generated if RFRQ is set
	//I2C0FCN0 = I2C0FCN0_RFRQE__ENABLED; // | I2C0FCN0_TFRQE__ENABLED;
}

// return:
//		0:	read offset
//		1:	read normal data
//	   -1:	no data read
static int8_t i2c_slave_input_byte()
{
	if ((I2C0FCN1 & I2C0FCN1_RXE__BMASK) == I2C0FCN1_RXE__NOT_EMPTY)
	{
		uint8_t rb = I2C0DIN;

		if ((msa_drv_data.op != MSA_OP_READ_WITH_OFFSET)
				&& (msa_drv_data.op != MSA_OP_WRITE_WITH_OFFSET))
		{
		   // Store incoming offset addr
			msa_drv_data.offset = rb;
			if (msa_drv_data.op == MSA_OP_READ)
				msa_drv_data.op = MSA_OP_READ_WITH_OFFSET;
			else
				msa_drv_data.op = MSA_OP_WRITE_WITH_OFFSET;
			gRevByteNum ++;
			return 0;
		}
		else
		{
			//Store incoming data
			if (msa_drv_data.op == MSA_OP_WRITE_WITH_OFFSET)
			{
				MSA_write(msa_drv_data.offset, rb);
			}
			return 1;
		}
	}
	else
		return -1;
}

void I2C_slave_enable(uint8_t enable) reentrant
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE  = PG3_PAGE;

	if(enable)  //module is not selected
	{
		I2C0CN0 &= ~I2C0CN0_I2C0EN__ENABLED;	//disable i2c slave
	}
	else	   //module is selected
	{
		I2C0CN0 |= I2C0CN0_PINMD__I2C_MODE;  //set i2c slave pin to i2c mode
		I2C0CN0 |= I2C0CN0_I2C0EN__ENABLED;  //enable i2c slave
	}
	SFRPAGE = SFRPAGE_SAVE;

}

bool I2C_slave_get_status(void)
{
	return (rd_ready || wr_ready);
}

SI_INTERRUPT (I2C0_ISR, I2C0_IRQn)
{
	static uint8_t SendByteNum = 0;
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	uint8_t d, i = 0;
	int8_t  len = 0;
	uint8_t intr;
	uint8_t fcn0;

	SFRPAGE = PG3_PAGE;
	intr = I2C0STAT;
	fcn0 = I2C0FCN0;

	switch (intr & I2C_STATUS_VECTOR_MASK)
	{
		case I2C_ADDR_RD:  // START+ADDR+R
		{
			  I2C0STAT &= ~(I2C0STAT_START__BMASK | I2C0STAT_RD__BMASK);
			  if (msa_drv_data.op == MSA_OP_WRITE_WITH_OFFSET
					  || msa_drv_data.op == MSA_OP_READ_WITH_OFFSET)
				  msa_drv_data.op = MSA_OP_READ_WITH_OFFSET;
			  else
				  msa_drv_data.op = MSA_OP_READ;
			  rd_ready = true;
			  SendByteNum ++;
			  d = MSA_read(msa_drv_data.offset);
			  I2C0DOUT = d; //msa_drv_data.lower_page.read(msa_drv_data.offset++);
			  msa_drv_data.offset++;
			  break;
		}
		case I2C_ADDR_WR: // START+ADDR+W
		{
			  I2C0STAT &= ~(I2C0STAT_START__BMASK); // | I2C0STAT_WR__BMASK);
			  gRevByteNum = 0;
			  wr_ready = true;

			  if (msa_drv_data.op == MSA_OP_WRITE_WITH_OFFSET
					  || msa_drv_data.op == MSA_OP_READ_WITH_OFFSET)
				  msa_drv_data.op = MSA_OP_WRITE_WITH_OFFSET;
			  else
				  msa_drv_data.op = MSA_OP_WRITE;
			  break;
		}
		case I2C_RD_DATA:
		{
			  // Check for NACK
			  if ((I2C0STAT & I2C0STAT_NACK__BMASK) == I2C0STAT_NACK__SET)
			  {
				  // Master did not NACK
				  // Stopping transfer of data
				  // Flush FIFO if there is data in it
				  // UART0_buffer_output("NACK\r\n");
				  I2C0STAT &= ~I2C0STAT_NACK__BMASK;
			  }
			  // Check for ACK
			  else
			  {
				  //if (MSA_read(msa_drv_data.offset, &d))
				  d = MSA_read(msa_drv_data.offset);
				  I2C0DOUT = d; //msa_drv_data.lower_page.read(msa_drv_data.offset++);
				  msa_drv_data.offset++;
			  }
			  I2C0STAT &= ~I2C0STAT_RD__BMASK;
			  break;
		}
		case I2C_WR_DATA:  // FIFO is full, whatever slave ACK/NACK master
		{
			i = 0;
			while(i < 3)
			{
				len = i2c_slave_input_byte();
				if (len >= 0)
					msa_drv_data.offset += len;
				else
					break; // no data read
				i++;
			}
			I2C0STAT &= ~I2C0STAT_WR__BMASK;
			break;
		}

		case I2C_STOSTA:   // STOP Condition received.
		case I2C_STOSTARD:
		case I2C_STOSTAWR:
		case I2C_STO:  		// STOP Condition received.
		{
			  msa_drv_data.op = MSA_OP_IDLE;
			  I2C0STAT &= ~I2C0STAT_STOP__BMASK;
			  I2C0STAT &= ~I2C0STAT_I2C0INT__BMASK;
			  I2C0CN0  &= ~I2C0CN0_BUSY__BMASK;   // Clear BUSY bit,and Device will acknowledge an I2C master ack
			  I2C0FCN0 |= I2C0FCN0_RFLSH__FLUSH | I2C0FCN0_TFLSH__FLUSH;   //These bits flushes the RX/TX FIFO,When firmware sets this bit to 1, the internal FIFO counters will be reset, and any remaining data will be lost
			  rd_ready = false;
			  wr_ready = false;
			  SendByteNum = 0;  		//when i2c communication end,init SendByteNum again
			  gRevByteNum = 0;
			  I2C0FCN0 |= I2C0FCN0_RFRQE__ENABLED;
			  MSA_lower_page_update_intl_pin_staus();
			  TASK_fast_sched(TASK_ID_WRITE_EEP_ON_POLL);
			  TASK_fast_sched(TASK_ID_CONTROL_BYTE_ON_POLL);
			  break;  	 //when receive the stop Condition,clear RX/TX FIFO,and direct return in order to avoid  reassignment  after
		}
		default:
		{
			i = 0;
			while(i < 3)
			{
				len = i2c_slave_input_byte();
				if (len >= 0)
					msa_drv_data.offset += len;
				else
					break;
				i++;
			}
			break;
		}
    }
	I2C0STAT &= ~I2C0STAT_I2C0INT__BMASK;  	// Clear I2C interrupt flag
	SFRPAGE   = SFRPAGE_SAVE;
}

