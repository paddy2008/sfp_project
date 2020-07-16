#include "Efm8lb12_common.h"
#include "Efm8lb12_eep_interface.h"

static void _flash_page_hw_erase (FLADDR addr)
{
   bit EA_SAVE = IE_EA;                // Preserve IE_EA

   uint8_t xdata * data pwrite;           // Flash write pointer

   uint8_t i = 0;

   IE_EA = 0;                          // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   pwrite = (uint8_t xdata *) addr;

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x03;                      // PSWE = 1; PSEE = 1

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
   *pwrite = 0;                        // Initiate page erase

   PSCTL &= ~0x03;                     // PSWE = 0; PSEE = 0

   IE_EA = EA_SAVE;                    // Restore interrupts
}

static void _flash_hw_write_byte (FLADDR addr, uint8_t byte)
{
   bit  EA_SAVE = IE_EA;                // Preserve IE_EA
   uint8_t xdata * data pwrite;        // Flash write pointer

   IE_EA = 0;                          // Disable interrupts

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source

   pwrite = (uint8_t xdata *) addr;

   FLKEY  = 0xA5;                      // Key Sequence 1
   FLKEY  = 0xF1;                      // Key Sequence 2
   PSCTL |= 0x01;                      // PSWE = 1 which enables writes

   VDM0CN = 0x80;                      // Enable VDD monitor

   RSTSRC = 0x02;                      // Enable VDD monitor as a reset source
   *pwrite = byte;                     // Write the byte

   PSCTL &= ~0x01;                     // PSWE = 0 which disable writes

   IE_EA = EA_SAVE;                    // Restore interrupts
}


uint8_t FLASH_ByteRead (FLADDR addr)
{
   bit EA_SAVE = IE_EA;                // Preserve IE_EA

   uint8_t code * data pread;          // Flash read pointer
   uint8_t byte;

   IE_EA = 0;                          // Disable interrupts

   pread = (uint8_t code *) addr;

   byte = *pread;                      // Read the byte

   IE_EA = EA_SAVE;                    // Restore interrupts

   return byte;
}


uint8_t FLASH_ByteRead_by_i2c_slave_isr (FLADDR addr)
{
   bit EA_SAVE = IE_EA;                // Preserve IE_EA

   uint8_t code * data pread;          // Flash read pointer
   uint8_t byte;

   IE_EA = 0;                          // Disable interrupts

   pread = (uint8_t code *) addr;

   byte = *pread;                      // Read the byte

   IE_EA = EA_SAVE;                    // Restore interrupts

   return byte;
}

uint8_t * FLASH_Read (uint8_t *dest, FLADDR src, uint16_t numbytes)
{
   FLADDR i;

   for (i = 0; i < numbytes; i++)
   {
      *dest++ = FLASH_ByteRead (src+i);
   }
   return dest;
}


uint8_t * FLASH_Read_by_i2c_slave_isr (uint8_t *dest, FLADDR src, uint16_t numbytes)
{
   FLADDR i;

   for (i = 0; i < numbytes; i++)
   {
      *dest++ = FLASH_ByteRead_by_i2c_slave_isr (src+i);
   }
   return dest;
}

static uint16_t flash_get_page_base_addr(FLADDR addr)
{
	return addr & (~(FLASH_PAGE_SIZE-1));
}

void flash_erase_range(FLADDR from, uint16_t len)
{
	FLADDR	page_base_addr, fl_addr, to;
	uint8_t temp[FLASH_PAGE_SIZE];
	uint16_t i;

	page_base_addr = flash_get_page_base_addr(from);
	to = (FLADDR)(from + len);

	for(i = 0; i < FLASH_PAGE_SIZE; i++)
	{
		temp[i] = FLASH_ByteRead(page_base_addr + i);
	}

	_flash_page_hw_erase(page_base_addr);

	for(i = 0; i < FLASH_PAGE_SIZE; i++)
	{
		fl_addr = page_base_addr + i;
		if (fl_addr < from || fl_addr >= to)
			_flash_hw_write_byte(fl_addr, temp[i]);
	}
}


void flash_write_byte(FLADDR addr, uint8_t value)
{
	uint16_t page_base_addr = 0 , i = 0;
	uint8_t temp[FLASH_PAGE_SIZE];
	uint8_t flash_value = FLASH_ByteRead(addr);

	if(flash_value == value)  //if want to write value is equal to value info flash,there is not need to write it info flash
	{
		return;
	}
	else
	{
		page_base_addr = flash_get_page_base_addr(addr);

		if(flash_value != 0xFF)   //indicate this addr was writen value,so it must be erase page at first
		{
			for(i = 0; i < FLASH_PAGE_SIZE; i++)
			{
				temp[i] = FLASH_ByteRead(page_base_addr + i);
			}

			_flash_page_hw_erase(page_base_addr);

			if(addr - page_base_addr >= 0 && addr - page_base_addr < FLASH_PAGE_SIZE)
			{
				temp[addr - page_base_addr]  = value;
			}

			for(i = 0; i < FLASH_PAGE_SIZE; i++)
			{
				_flash_hw_write_byte(page_base_addr + i, temp[i]);
			}
		}
		else  //indicate this addr was not writen value, it can be writen directly
		{
			_flash_hw_write_byte(addr, value);
		}
	}
}


void flash_write_multi_byte (FLADDR dest, uint8_t *src, uint16_t numbytes)
{
	uint16_t page_base_addr = 0 , i = 0;
	uint8_t temp[512], flash_value = 0;
	uint8_t *ptr = src;

	bool isErasePage = false;

	for (i = dest; i < dest+numbytes; i++)
	{
		flash_value = FLASH_ByteRead(i);

		if((flash_value != 0xFF) && (flash_value != *ptr++))
		{
			isErasePage = true;
			break;
		}
	}

	ptr = src;

	if(isErasePage)
	{
		page_base_addr = flash_get_page_base_addr(dest);

		for(i = 0; i < FLASH_PAGE_SIZE; i++)
		{
			temp[i] = FLASH_ByteRead(page_base_addr + i);
		}

		_flash_page_hw_erase(page_base_addr);

		for (i = dest; i < dest+numbytes; i++)
		{
			if(i - page_base_addr >= 0 && i - page_base_addr < 512)
			{
				temp[i - page_base_addr] = *ptr++;
			}
		}

		for(i = 0; i < FLASH_PAGE_SIZE; i++)
		{
			_flash_hw_write_byte(page_base_addr + i, temp[i]);
		}
	}
	else
	{
		for (i = dest; i < dest+numbytes; i++)
		{
			_flash_hw_write_byte(i, *ptr++);
		}
	}
}
