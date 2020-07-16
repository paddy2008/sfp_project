#include "Efm8lb12_smbus.h"
#include "Efm8lb12_common.h"
#include "Efm8lb12_timer.h"

SI_SBIT (SDA, SFR_P1, 2);                 // SMBus on P1.2
SI_SBIT (SCL, SFR_P1, 3);                 // and P1.3

uint8_t* pSMB_DATA_IN = NULL;             // Global pointer for SMBus data All receive data is written here

uint8_t SMB_SINGLEBYTE_OUT = 0;           // Global holder for single byte writes.

uint8_t* pSMB_DATA_OUT = NULL;             // Global pointer for SMBus data.
                                       // All transmit data is read from here

uint8_t SMB_DATA_LEN = 0;                       // Global holder for number of bytes
                                       // to send or receive in the current
                                       // SMBus transfer.

uint8_t WORD_ADDR = 0;                  // Global holder for the EEPROM word
                                       // address that will be accessed in
                                       // the next transfer

volatile uint8_t TARGET = 0;                             // Target SMBus slave address


volatile bit SMB_BUSY = 0;             // Software flag to indicate when the
                                       // EEPROM_ByteRead() or
                                       // EEPROM_ByteWrite()
                                       // functions have claimed the SMBus

volatile bit SMB_RW = 0;                   // Software flag to indicate the
                                       // direction of the current transfer

volatile bit SMB_SENDWORDADDR = 0;         // When set, this flag causes the ISR
                                       // to send the 8-bit <WORD_ADDR>
                                       // after sending the slave address.


volatile bit SMB_RANDOMREAD = 0;           // When set, this flag causes the ISR
                                       // to send a START signal after sending
                                       // the word address.
                                       // For the 24LC02B EEPROM, a random read
                                       // (a read from a particular address in
                                       // memory) starts as a write then
                                       // changes to a read after the repeated
                                       // start is sent. The ISR handles this
                                       // switchover if the <SMB_RANDOMREAD>
                                       // bit is set.

volatile bit SMB_ACKPOLL = 0;          // When set, this flag causes the ISR
                                       // to send a repeated START until the
                                       // slave has acknowledged its address

volatile bit SEND_START = 0;          // Send a start


volatile uint8_t WORD_ADDR_HIGH = 0;
volatile uint8_t WORD_ADDR_LOW = 0;
volatile uint8_t COUNTER  = 0;
volatile bool    gSingleByteOffsetAddr = true;

uint8_t SMB_check_bus_status(void)
{
	uint8_t i = 0;

	for(i = 0; i < 50; i++)
	{
		if(SDA && SCL)
		{
			return GOS_OK;
		}
		TIMER_soft_delay_ms(10);
	}
	return GOS_FAIL;
}


void SMB_init (void)
{
   uint8_t SFRPAGE_SAVE = SFRPAGE;

   SMB_reset();

   SFRPAGE = 0x0;

   SMB0CF = 0x5C;                      // Use Timer0 overflows as SMBus clock source;
                                       // enable slave mode;
                                       // Enable setup & hold time extensions;
                                       // enable SMBus Free timeout detect;
                                       // enable SCL low timeout detect;

   SMB0CF |= 0x80;                     // Enable SMBus;

   EIE1 |= 0x01;                 	   // Enable the SMBus interrupt

   SFRPAGE  = 0x10;
   EIP1  &= ~0x01;				   	   // Make smbus  high priority2,priority3 is the highest priority!
   EIP1H |= 0x01;

   SFRPAGE = SFRPAGE_SAVE;
}

uint8_t SMB_write_byte(uint8_t BaseAddr,uint16_t OffsetAddr,uint8_t Data)
{
   return SMB_write_multi_byte( BaseAddr, OffsetAddr, &Data,1);
}

uint8_t SMB_write_multi_byte(uint8_t BaseAddr,uint16_t OffsetAddr,uint8_t *Data,uint8_t Length)
{
	uint16_t i = 0;

	for(i = 0; SMB_BUSY & (i < 0xFFFF);i++); // Wait for SMBus to be free.
	SMB_BUSY = 1;                       // Claim SMBus (set to busy)

   // Set SMBus ISR parameters
   TARGET = BaseAddr;                   // Set target slave address
   SMB_RW = WRITE;                      // Mark next transfer as a write
   SMB_SENDWORDADDR = 1;                // Send Word Address after Slave Address
   SMB_RANDOMREAD = 0;                  // Do not send a START signal after
										// the word address
   SMB_ACKPOLL = 1;                     // Enable Acknowledge Polling (The ISR
										// will automatically restart the
										// transfer if the slave does not
										// acknoledge its address.


	if(BaseAddr == 0xa8 || BaseAddr == 0xb8 || BaseAddr == 0x24) //12bit offsetaddr
	{
		WORD_ADDR_HIGH = (OffsetAddr & 0xFF00) >> 8;
		WORD_ADDR_LOW = OffsetAddr & 0xFF;
		gSingleByteOffsetAddr = false;
	}
	else
	{
		WORD_ADDR_LOW = OffsetAddr & 0xFF;
		gSingleByteOffsetAddr = true;
	}


   // The outgoing data pointer points to the <dat> variable
   pSMB_DATA_OUT = Data;

   SMB_DATA_LEN =  Length;                   // Specify to ISR that the next transfer
									   // will contain one data byte

   // Initiate SMBus Transfer
   SMB0CN0_STA = 1;
   for (i = 0;SMB_BUSY;i++)    // Wait until data write OK
   {
		if (i >= 0xFFFF)
		{
			SMB0CF &= ~0x80;                 // Reset communication
			SMB0CF |= 0x80;
			SMB0CN0_STA = 0;
			SMB0CN0_STO = 0;
			SMB0CN0_ACK = 0;
			SMB_BUSY = 0;                    // Free SMBus
			SMB_ACKPOLL = 0;
			return GOS_FAIL;
		}
	}
   return GOS_OK;
}

uint8_t SMB_write_byte_by_isr(uint8_t BaseAddr,uint16_t OffsetAddr,uint8_t *Data,uint8_t Length)
{
	uint16_t i = 0;

	for(i = 0; SMB_BUSY & (i < 0xFFFF);i++); // Wait for SMBus to be free.
	SMB_BUSY = 1;                       // Claim SMBus (set to busy)

   // Set SMBus ISR parameters
   TARGET = BaseAddr;                   // Set target slave address
   SMB_RW = WRITE;                      // Mark next transfer as a write
   SMB_SENDWORDADDR = 1;                // Send Word Address after Slave Address
   SMB_RANDOMREAD = 0;                  // Do not send a START signal after
										// the word address
   SMB_ACKPOLL = 1;                     // Enable Acknowledge Polling (The ISR
										// will automatically restart the
										// transfer if the slave does not
										// acknoledge its address.


	if(BaseAddr == 0xa8 || BaseAddr == 0xb8 || BaseAddr == 0x24) //12bit offsetaddr
	{
		WORD_ADDR_HIGH = (OffsetAddr & 0xFF00) >> 8;
		WORD_ADDR_LOW = OffsetAddr & 0xFF;
		gSingleByteOffsetAddr = false;
	}
	else
	{
		WORD_ADDR_LOW = OffsetAddr & 0xFF;
		gSingleByteOffsetAddr = true;
	}


   // The outgoing data pointer points to the <dat> variable
   pSMB_DATA_OUT = Data;

   SMB_DATA_LEN =  Length;                   // Specify to ISR that the next transfer
									   // will contain one data byte

   // Initiate SMBus Transfer
   SMB0CN0_STA = 1;
   for (i = 0;SMB_BUSY;i++)    // Wait until data write OK
   {
		if (i >= 0xFFFF)
		{
			SMB0CF &= ~0x80;                 // Reset communication
			SMB0CF |= 0x80;
			SMB0CN0_STA = 0;
			SMB0CN0_STO = 0;
			SMB0CN0_ACK = 0;
			SMB_BUSY = 0;                    // Free SMBus
			SMB_ACKPOLL = 0;
			return GOS_FAIL;
		}
	}
   return GOS_OK;
}
uint8_t SMB_read_byte(uint8_t BaseAddr,uint16_t OffsetAddr,uint8_t *Data)
{
	return SMB_read_multi_byte(BaseAddr,OffsetAddr,Data,1);
}

uint8_t SMB_read_multi_byte(uint8_t BaseAddr,uint16_t OffsetAddr,uint8_t *Pdata,uint8_t Length)
{
	uint16_t i = 0;

	for (i = 0; SMB_BUSY & (i < 0xFFFF);i++); // Wait for SMBus to be free.
	SMB_BUSY = 1;                       // Claim SMBus (set to busy)

	// Set SMBus ISR parameters
	TARGET = BaseAddr;               	// Set target slave address
	SMB_RW = WRITE;                     // A random read starts as a write
									    // then changes to a read after
									    // the repeated start is sent. The
									    // ISR handles this switchover if
									    // the <SMB_RANDOMREAD> bit is set.
	SMB_SENDWORDADDR = 1;               // Send Word Address after Slave Address
	SMB_RANDOMREAD = 1;                 // Send a START after the word address
	SMB_ACKPOLL = 1;                    // Enable Acknowledge Polling

	if(BaseAddr == 0xa8 || BaseAddr == 0xb8 || BaseAddr == 0x24) //12bit offsetaddr
    {
    	WORD_ADDR_HIGH = (OffsetAddr & 0xFF00) >> 8;
    	WORD_ADDR_LOW = OffsetAddr & 0xFF;
    	gSingleByteOffsetAddr = false;
    }
    else
    {
    	WORD_ADDR_LOW = OffsetAddr & 0xFF;
    	gSingleByteOffsetAddr = true;
    }

	// Set the the incoming data pointer
	pSMB_DATA_IN = Pdata;
	SMB_DATA_LEN = Length;                // Specify to ISR that the next transferwill contain <len> data bytes

	// Initiate SMBus Transfer
	SMB0CN0_STA = 1;
	for(i = 0;SMB_BUSY;i++)                       // Wait until data is read
	{
		if (i >= 0xFFFF)
		{
			SMB0CF &= ~0x80;           // Reset communication
			SMB0CF |= 0x80;
			SMB0CN0_STA = 0;
			SMB0CN0_STO = 0;
			SMB0CN0_ACK = 0;
			SMB_BUSY = 0;              // Free SMBus
			SMB_ACKPOLL = 0;
			return GOS_FAIL;
		}
	}
	return GOS_OK;
}

uint8_t SMB_read_byte_by_isr(uint8_t BaseAddr,uint16_t OffsetAddr,uint8_t *Pdata,uint8_t Length)
{
	uint16_t i = 0;

	for (i = 0;SMB_BUSY & (i < 0xFFFF);i++);
	SMB_BUSY = 1;                       // Claim SMBus (set to busy)

	// Set SMBus ISR parameters
	TARGET = BaseAddr;               	// Set target slave address
	SMB_RW = WRITE;                     // A random read starts as a write
									    // then changes to a read after
									    // the repeated start is sent. The
									    // ISR handles this switchover if
									    // the <SMB_RANDOMREAD> bit is set.
	SMB_SENDWORDADDR = 1;               // Send Word Address after Slave Address
	SMB_RANDOMREAD = 1;                 // Send a START after the word address
	SMB_ACKPOLL = 1;                    // Enable Acknowledge Polling

	if(BaseAddr == 0xa8 || BaseAddr == 0xb8 || BaseAddr == 0x24) //12bit offsetaddr
    {
    	WORD_ADDR_HIGH = (OffsetAddr & 0xFF00) >> 8;
    	WORD_ADDR_LOW = OffsetAddr & 0xFF;
    	gSingleByteOffsetAddr = false;
    }
    else
    {
    	WORD_ADDR_LOW = OffsetAddr & 0xFF;
    	gSingleByteOffsetAddr = true;
    }

	// Set the the incoming data pointer
	pSMB_DATA_IN = Pdata;
	SMB_DATA_LEN = Length;                // Specify to ISR that the next transferwill contain <len> data bytes

	// Initiate SMBus Transfer
	SMB0CN0_STA = 1;
	for(i = 0; SMB_BUSY; i++)                       // Wait until data is read
	{
		if (i >= 0xFFFF)
		{
			SMB0CF &= ~0x80;           // Reset communication
			SMB0CF |= 0x80;
			SMB0CN0_STA = 0;
			SMB0CN0_STO = 0;
			SMB0CN0_ACK = 0;
			SMB_BUSY = 0;              // Free SMBus
			SMB_ACKPOLL = 0;
			return GOS_FAIL;
		}
	}
	return GOS_OK;
}


static void SMB_bit_out()
{
   while(!SDA)
   {
	   SCL = 1;

	   TIMER5_hw_delay_us(2);

	   SCL = 0;

	   TIMER5_hw_delay_us(2);
   }

   //Send stop singal
   SCL = 1;

   TIMER5_hw_delay_us(2);

   if(SCL == 1)
   {
	   SDA = 0;
	   TIMER5_hw_delay_us(2);
	   SDA = 1;
   }
}


void SMB_reset(void)
{
	uint8_t i = 0;

	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;

	XBR0   &= ~XBR0_SMB0E__ENABLED; 	  //disable SMB0 I/O
	SMB0CF &= ~0x80;                      //Enable SMBus;

	SFRPAGE = SFRPAGE_SAVE;

	SMB_bit_out();

	SFRPAGE = 0x0;
	XBR0   |= XBR0_SMB0E__ENABLED; 		// Enable SMB0 I/O
	SMB0CF |= 0x80;                     // Enable SMBus;

	SFRPAGE = SFRPAGE_SAVE;
}

SI_INTERRUPT(SMBUS0_ISR, SMBUS0_IRQn)
{
	bit FAIL = 0;                		// Used by the ISR to flag failed transfers
	static uint8_t i = 0;       		// Used by the ISR to count the number of data bytes sent or received

   if (SMB0CN0_ARBLOST == 0)            // Check for errors
   {
	   switch (SMB0CN0 & 0xF0)             // Status vector
	   {
		  // Master Transmitter/Receiver: START condition transmitted.
		  case SMB_MTSTA:
			 SMB0DAT = TARGET;             // Load address of the target slave
			 SMB0DAT &= 0xFE;              // Clear the LSB of the address for the
										   // R/W bit
			 SMB0DAT |= SMB_RW;            // Load R/W bit
			 SMB0CN0_STA = 0;               // Manually clear START bit
			 i = 0;                        // Reset data byte counter
			 break;

		  // Master Transmitter: Data byte (or Slave Address) transmitted
		  case SMB_MTDB:
 			 if (SMB0CN0_ACK)               // Slave Address or Data Byte Acknowledged
			 {
				 if (SEND_START)
				{
				   SMB0CN0_STA = 1;
				   SEND_START = 0;
				   break;
				}
				if(SMB_SENDWORDADDR)       // Are we sending the word address?
				{
				   if(gSingleByteOffsetAddr)
				   {
					   SMB_SENDWORDADDR = 0;   		// Clear flag
					   SMB0DAT = WORD_ADDR_LOW;     // Send word address

					   if (SMB_RANDOMREAD)
					   {
						  SEND_START = 1;      // Send a START after the next SMB0CN_ACK cycle,因为已经把baseaddr和offsetaddr发送结束了，所以下个循环重新发送一个Start信号
						  SMB_RW = READ;
					   }
					   break;
				   }
				   else
				   {
					   COUNTER++;
					   if(COUNTER == 1)
					   {
						   SMB0DAT = WORD_ADDR_HIGH;    // Send word address high byte
					   }
					   if(COUNTER == 2)
					   {
						   COUNTER = 0;
						   SMB_SENDWORDADDR = 0;       // Clear flag
						   SMB0DAT = WORD_ADDR_LOW;    // Send word address low byte
						   if (SMB_RANDOMREAD)
						   {
							  SEND_START = 1;          // Send a START after the next SMB0CN_ACK cycle,因为已经把baseaddr和offsetaddr发送结束了，所以下个循环重新发送一个Start信号
							  SMB_RW = READ;
						   }
					   }
					   break;
				   }
				}

				if (SMB_RW == WRITE)         // Is this transfer a WRITE?
				{
				   if (i < SMB_DATA_LEN)   // Is there data to send?
				   {
					  // send data byte
					  SMB0DAT = *pSMB_DATA_OUT;

					  // increment data out pointer
					  pSMB_DATA_OUT++;

					  // increment number of bytes sent
					  i++;
				   }
				   else        			  // This is the last byte
				   {
					 SMB0CN0_STO = 1;      // Set SMB0CN_STO to terminte transfer
					 SMB_BUSY = 0;         // Clear software busy flag
				   }
				}
				else {}                    // If this transfer is a READ,
										   // then take no action. Slave
										   // address was transmitted. A
										   // separate 'case' is defined
										   // for data byte recieved.
			 }
			 else                          // If slave NACK,
			 {
				if(SMB_ACKPOLL)
				{
				   SMB0CN0_STA = 1;        // Restart transfer
				   //由于在i2c通信的过程中，对方设备正在处理其他的事情，可能不能及时回i2c的ack，从而导致通信失败，为了保持通信的正确性，这里应该反复请求,如果只
				   //只访问50次，实验证明，会存在大量的地址写入失败,但是这里又不能一直等待，这样会造成程序卡死，然后不能依次读取基地址,解决方案这里一直去请求，在读写函数里
				   //去做判断，如果超过3s没有回复，即将SMB0CN_STA = 0
				}
				else
				{
				   FAIL = 1;               // Indicate failed transfer
				}                          // and handle at end of ISR
			 }
			 break;

		  // Master Receiver: byte received
		  case SMB_MRDB:
			 if (++i < SMB_DATA_LEN)        // Is there any data remaining?
			 {
				*pSMB_DATA_IN = SMB0DAT;   // Store received byte
				pSMB_DATA_IN++;            // Increment data in pointer                    // Increment number of bytes received
				SMB0CN0_ACK = 1;           // Set SMB0CN_ACK bit (may be cleared later in the code)
			 }
			 else        				   // This is the last byte
			 {
				*pSMB_DATA_IN = SMB0DAT;   // Store received byte
				SMB_BUSY = 0;              // Free SMBus interface
				SMB0CN0_ACK = 0;           // Send NACK to indicate last byte of this transfer
				SMB0CN0_STO = 1;           // Send STOP to terminate transfer
			 }
			 break;
		  default:
			 FAIL = 1;                     // Indicate failed transfer  and handle at end of ISR;
			 break;
	   }
   }
   else
   {
		 FAIL = 1;                     // Indicate failed transfer  and handle at end of ISR;
   }

   if (FAIL)                           // If the transfer failed,
   {
	  SMB0CF &= ~0x80;                 // Reset communication
	  SMB0CF |= 0x80;
	  SMB0CN0_STA = 0;
	  SMB0CN0_STO = 0;
	  SMB0CN0_ACK = 0;
	  SMB_BUSY = 0;                    // Free SMBus
	  FAIL = 0;
   }
	 SMB0CN0_SI = 0;                             // Clear interrupt flag
	// EIE1 |= 0x01;
}













