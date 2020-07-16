//-----------------------------------------------------------------------------
//      Copyright (c) 2017-2018,
//-----------------------------------------------------------------------------
//
// file name  :       InitDevice.h
//
// description:       mcu pin define
//
// Target     :       EFM8LB12F32ES0
//
// version    :       V100.001
//					  2019/07/30 by roy zhou
//
//-----------------------------------------------------------------------------
#ifndef __INIT_DEVICE_H__
#define __INIT_DEVICE_H__


#include <SI_EFM8LB1_Register_Enums.h>

//--------- SFR define for pin ----------------//

//--------- SFR define for pin ----------------//

// MSDA P0.2
SI_SBIT(Port_MSDA, SFR_P1, 1);

// MSCL P0.3
SI_SBIT(Port_MSCL, SFR_P1, 2);

//P0.7
SI_SBIT(UPDATE_GPIO, SFR_P0, 7);


extern void	Port_Init(void);

#endif

