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
SI_SEGMENT_VARIABLE( ddm_Lower,  extern struct LOWER_MEMORY_MAP_struct, 				SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T00,    extern struct Table_Size_128_struct,   				SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T80,    extern struct Table_General_Control_struct,   			SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T81,    extern struct Table_TX_Cal_Ctrl_struct,       			SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T82,    extern struct Table_RX_Cal_Ctrl_struct,       			SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T83,    extern struct Table_Temperature_Compensation_struct,  	SI_SEG_XDATA );

SI_SEGMENT_VARIABLE( ddm_T84,    extern struct Table_ModSet_Calibration_struct,       	SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T86,    extern struct Table_Device_MALD_37645B_struct,       	SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T87,    extern struct Table_Device_MATA_37644B_struct,       	SI_SEG_XDATA );

SI_SEGMENT_VARIABLE( ddm_T90,    extern struct Table_Monitor_struct,		SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T91,    extern struct Table_Device_Debug_struct,	SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( ddm_T92,    extern struct Table_System_Debug_struct,	SI_SEG_XDATA );

SI_SEGMENT_VARIABLE( I2c_Buffer, extern struct I2C_BUFFER_Struct,	        SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( PrefetchData_A2, uint8_t, SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( PrefetchData_A0, uint8_t, SI_SEG_XDATA );


//-----------------------------------------------------------------------------
// SecurityCheck
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures     SFP Security Check
//
//-----------------------------------------------------------------------------
void SecurityCheck (void)
{
	if ( ddm_Lower.PasswordEntryArea == ddm_T80.PassWord_1 )
	{
		ddm_Lower.SecurityLevel = 0x01;
	}
	else if ( ddm_Lower.PasswordEntryArea == ddm_T80.PassWord_2 )
	{
		ddm_Lower.SecurityLevel = 0x02;
	}
	else
	{
		ddm_Lower.SecurityLevel = 0x00;
	}
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
	if ( A0_7Bits_ADDR == I2c_Buffer.SubAddress )
	{
		PrefetchData_A0 = ((uint8_t SI_SEG_XDATA *)(&ddm_A0Data))[I2c_Buffer.A0_RegAddress];
	}
	else
	{
		if ( I2c_Buffer.A2_RegAddress <= 0x7F )
		{
			PrefetchData_A2 = ((uint8_t SI_SEG_XDATA *)(&ddm_Lower))[I2c_Buffer.A2_RegAddress];
		}
		else
		{
			PrefetchData_A2 = 0xFF;

			if ( ddm_Lower.PageSelect == 0x00 || ddm_Lower.PageSelect == 0x01 )
			{
				PrefetchData_A2 = ((uint8_t SI_SEG_XDATA *)(&ddm_T00))[I2c_Buffer.A2_RegAddress-128];
			}
			else if ( ddm_Lower.PageSelect == 0x80 )
			{
				if ( (I2c_Buffer.A2_RegAddress < (0x80+TABLE80_SIZE)) && (ddm_Lower.SecurityLevel == 0x02) )
					PrefetchData_A2 = ((uint8_t SI_SEG_XDATA *)(&ddm_T80))[I2c_Buffer.A2_RegAddress-128];
			}
			else if ( ddm_Lower.PageSelect == 0x81 )
			{
				if ( (I2c_Buffer.A2_RegAddress < (0x80+TABLE81_SIZE)) && (ddm_Lower.SecurityLevel == 0x02) )
					PrefetchData_A2 = ((uint8_t SI_SEG_XDATA *)(&ddm_T81))[I2c_Buffer.A2_RegAddress-128];
			}
			else if ( ddm_Lower.PageSelect == 0x82 )
			{
				if ( (I2c_Buffer.A2_RegAddress < (0x80+TABLE82_SIZE)) && (ddm_Lower.SecurityLevel == 0x02) )
					PrefetchData_A2 = ((uint8_t SI_SEG_XDATA *)(&ddm_T82))[I2c_Buffer.A2_RegAddress-128];
			}
			else if ( ddm_Lower.PageSelect == 0x83 )
			{
				if ( (I2c_Buffer.A2_RegAddress < (0x80+TABLE83_SIZE)) && (ddm_Lower.SecurityLevel == 0x02) )
					PrefetchData_A2 = ((uint8_t SI_SEG_XDATA *)(&ddm_T83))[I2c_Buffer.A2_RegAddress-128];
			}
			else if ( ddm_Lower.PageSelect == 0x84 )
			{
				if ( (I2c_Buffer.A2_RegAddress < (0x80+TABLE84_SIZE)) && (ddm_Lower.SecurityLevel == 0x02) )
					PrefetchData_A2 = ((uint8_t SI_SEG_XDATA *)(&ddm_T84))[I2c_Buffer.A2_RegAddress-128];
			}
			else if ( ddm_Lower.PageSelect == 0x86 )
			{
				if ( (I2c_Buffer.A2_RegAddress < (0x80+TABLE86_SIZE)) && (ddm_Lower.SecurityLevel == 0x02) )
					PrefetchData_A2 = ((uint8_t SI_SEG_XDATA *)(&ddm_T86))[I2c_Buffer.A2_RegAddress-128];
			}
			else if ( ddm_Lower.PageSelect == 0x87 )
			{
				if ( (I2c_Buffer.A2_RegAddress < (0x80+TABLE87_SIZE)) && (ddm_Lower.SecurityLevel == 0x02) )
					PrefetchData_A2 = ((uint8_t SI_SEG_XDATA *)(&ddm_T87))[I2c_Buffer.A2_RegAddress-128];
			}
			else if ( ddm_Lower.PageSelect == 0x90 )
			{
				if ( (I2c_Buffer.A2_RegAddress < (0x80+TABLE90_SIZE)) && (ddm_Lower.SecurityLevel == 0x02) )
					PrefetchData_A2 = ((uint8_t SI_SEG_XDATA *)(&ddm_T90))[I2c_Buffer.A2_RegAddress-128];
			}
			else if ( ddm_Lower.PageSelect == 0x91 )
			{
				if ( (I2c_Buffer.A2_RegAddress < (0x80+TABLE91_SIZE)) && (ddm_Lower.SecurityLevel == 0x02) )
					PrefetchData_A2 = ((uint8_t SI_SEG_XDATA *)(&ddm_T91))[I2c_Buffer.A2_RegAddress-128];
			}
			else if ( ddm_Lower.PageSelect == 0x92 )
			{
				if ( (I2c_Buffer.A2_RegAddress < (0x80+TABLE92_SIZE)) && (ddm_Lower.SecurityLevel == 0x02) )
					PrefetchData_A2 = ((uint8_t SI_SEG_XDATA *)(&ddm_T92))[I2c_Buffer.A2_RegAddress-128];
			}
			else
			{
				PrefetchData_A2 = 0xFF;
			}
		}
	}
}
