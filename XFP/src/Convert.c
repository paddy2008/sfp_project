//-----------------------------------------------------------------------------
//      Copyright (c) 2019,
//-----------------------------------------------------------------------------
//
// file name  :       Convert.c
//
// Description:       data type convert
//
// Target     :       EFM8LB.....
//
// version    :       V100.001
//					  2019/07/30
//
//-----------------------------------------------------------------------------

#include "include.h"

//-----------------------------------------------------------------------------
// ConvertUnsignedint
//-----------------------------------------------------------------------------
//
// Return Value:  uint16_t
// Parameters:    uint16_t
//
// Description    Convert Unsigned int
//
//-----------------------------------------------------------------------------
uint16_t ConvertUnsignedint ( uint16_t uint_data )
{
	uint16_t return_data;
	uint8_t *p;

	p = (uint8_t *)&uint_data;

	((uint8_t *)&return_data)[0] = *(p+1);
	((uint8_t *)&return_data)[1] = *(p);

	return return_data;
}


//-----------------------------------------------------------------------------
// ConvertSignedint
//-----------------------------------------------------------------------------
//
// Return Value:  int16_t
// Parameters:    int16_t
//
// Description     Convert signed int
//
//-----------------------------------------------------------------------------
int16_t ConvertSignedint ( int16_t int_data )
{
	int16_t return_data;
	uint8_t *p;

	p = (uint8_t *)&int_data;

	((uint8_t *)&return_data)[0] = *(p+1);
	((uint8_t *)&return_data)[1] = *(p);

	return return_data;

}


//-----------------------------------------------------------------------------
// ConvertFloatToUnsignedintWithRangeCheck
//-----------------------------------------------------------------------------
//
// Return Value:  int16_t
// Parameters:    int16_t
//
// Description     Convert Float To Unsigned int and Range Check
//
//-----------------------------------------------------------------------------
uint16_t ConvertFloatToUnsignedintWithRangeCheck ( float float_data )
{
	int32_t  temp_value;
	uint16_t ret;

	temp_value = (int32_t)float_data;

	if ( temp_value > 65535 )
	{
		ret = 0xFFFF;
	}
	else if ( temp_value < 1 )
	{
		ret = 0x0000;
	}
	else
	{
		ret = (uint16_t)temp_value;
	}

	return ret;
}

