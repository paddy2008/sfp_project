//-----------------------------------------------------------------------------
//      Copyright (c) 2019,
//-----------------------------------------------------------------------------
//
// file name  :       Convert.h
//
// Description:       data type convert
//
// Target     :       EFM8LB.....
//
// version    :       V100.001
//					  2019/07/30
//
//-----------------------------------------------------------------------------
#ifndef CONVERT_H
#define CONVERT_H


extern uint16_t ConvertUnsignedint(uint16_t uint_data );
extern int16_t  ConvertSignedint(int16_t int_data);
extern uint16_t ConvertFloatToUnsignedintWithRangeCheck ( float float_data );


#endif
