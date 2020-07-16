//-----------------------------------------------------------------------------
//      Copyright (c) 2017-2018,
//-----------------------------------------------------------------------------
//
// file name  :       I2cSlavePrefetch.h
//
// description:       I2c Slave Prefetch
//
// Target     :       EFM8LB12F32ES0
//
// version    :       V100.001
//
//
//-----------------------------------------------------------------------------

#include "include.h"

// ------------------------------------ data struct table -------------------------------------------//
SI_SEGMENT_VARIABLE( ddm_A0Data, extern struct Table_Size_256_struct,   				SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_Lower,  extern struct LOWER_MEMORY_128_struct, 				SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T00,    extern struct Table_Size_128_struct,   				SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( dom_backup, extern struct LOWER_MEMORY_DOM_struct,   				SI_SEG_XDATA );	 //dom 5 physical quantity
SI_SEGMENT_VARIABLE( I2c_Buffer, extern struct I2C_BUFFER_Struct,	        			SI_SEG_XDATA );


SI_SEGMENT_VARIABLE( PrefetchData_A2, uint8_t, SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( PrefetchData_A0, uint8_t, SI_SEG_XDATA );

SI_SEGMENT_VARIABLE( ddm_T02,    extern struct Table2_Size_128_struct,   				SI_SEG_XDATA );	 //
SI_SEGMENT_VARIABLE( ddm_T80,    extern struct Page_80H_Module_Information_struct,	 	SI_SEG_XDATA );

extern volatile uint8_t sem;

extern volatile uint8_t max_offset;

uint8_t code password[] = {0x43, 0x49, 0x47, 0x2D, 0x53, 0x46, 0x50};  //CIG-SFP

uint8_t SecurityCheck (void)
{
	uint8_t i = 0;

	for(i = 0x0; i < 0x7; i++)
	{
		if(ddm_Lower.lower_memory.table_data[i + 0x78] != password[i])
		{
			return false;
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
// Prefetch_Data
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     Prefetch_Data
//
//-----------------------------------------------------------------------------
void Prefetch_Data (void)
{
	static  bool  update_dom = false;

	if (A0_7Bits_ADDR == I2c_Buffer.SubAddress)
	{
		PrefetchData_A0 = ((uint8_t SI_SEG_XDATA *)(&ddm_A0Data))[I2c_Buffer.A0_RegAddress];

		if(I2c_Buffer.A0_RegAddress > max_offset)
		{
			I2C_slave_enable(0);
		}
	}
	else
	{
		if (I2c_Buffer.A2_RegAddress <= 0x7F)
		{
			if(I2c_Buffer.A2_RegAddress >= A2_DOM_START_ADDR && I2c_Buffer.A2_RegAddress <= A2_DOM_END_ADDR)
			{
				if(COM_GET_LOCK(sem) == 0x0)  //if lock
				{
					PrefetchData_A2 = dom_backup.dom.table_data[I2c_Buffer.A2_RegAddress - A2_DOM_START_ADDR];
					update_dom = true;
					if(I2c_Buffer.A2_RegAddress > max_offset)
					{
						I2C_slave_enable(0);
					}
					return;
				}
				else  //if not lock
				{
					if(update_dom)
					{
						PrefetchData_A2 = dom_backup.dom.table_data[I2c_Buffer.A2_RegAddress - A2_DOM_START_ADDR];
						update_dom = false;
						if(I2c_Buffer.A2_RegAddress > max_offset)
						{
							I2C_slave_enable(0);
						}
						return;
					}
				}
			}
			else if	(I2c_Buffer.A2_RegAddress >= 0x78 && I2c_Buffer.A2_RegAddress <= 0x7E)  //passwd area, always return 0 when read
			{
				PrefetchData_A2 = 0x0;
				if(I2c_Buffer.A2_RegAddress > max_offset)
				{
					I2C_slave_enable(0);
				}
				return;
			}
			else if	((I2c_Buffer.A2_RegAddress == 0x7F) && (SecurityCheck() == false))
			{
				PrefetchData_A2 = 0x0;
				if(I2c_Buffer.A2_RegAddress > max_offset)
				{
					I2C_slave_enable(0);
				}
				return;
			}

			if(I2c_Buffer.A2_RegAddress > max_offset)
			{
				I2C_slave_enable(0);
			}
			PrefetchData_A2 = ((uint8_t SI_SEG_XDATA *)(&ddm_Lower))[I2c_Buffer.A2_RegAddress];
		}
		else
		{
			PrefetchData_A2 = 0xFF;

			if (ddm_Lower.lower_memory.struct_data.PageSelect == 0x00 || ddm_Lower.lower_memory.struct_data.PageSelect == 0x01)
			{
				PrefetchData_A2 = ((uint8_t SI_SEG_XDATA *)(&ddm_T00))[I2c_Buffer.A2_RegAddress-128];
				if(I2c_Buffer.A2_RegAddress > max_offset)
				{
					I2C_slave_enable(0);
				}
			}
			else if ((ddm_Lower.lower_memory.struct_data.PageSelect == 0x2) && SecurityCheck())
			{
				PrefetchData_A2 = ((uint8_t SI_SEG_XDATA *)(&ddm_T02.table_data))[I2c_Buffer.A2_RegAddress-128];
				if(I2c_Buffer.A2_RegAddress > max_offset)
				{
					I2C_slave_enable(0);
				}
			}
			else if ((ddm_Lower.lower_memory.struct_data.PageSelect == 0x80) && SecurityCheck())
			{
				PrefetchData_A2 = ((uint8_t SI_SEG_XDATA *)(&ddm_T80))[I2c_Buffer.A2_RegAddress-128];
				if(I2c_Buffer.A2_RegAddress > max_offset)
				{
					I2C_slave_enable(0);
				}
			}
			else
			{
				PrefetchData_A2 = 0xFF;
				if(I2c_Buffer.A2_RegAddress > max_offset)
				{
					I2C_slave_enable(0);
				}
			}
		}
	}
}
