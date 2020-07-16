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

// MSDA P0.2
SI_SBIT(Port_MSDA, SFR_P0, 2);

// MSCL P0.3
SI_SBIT(Port_MSCL, SFR_P0, 3);

// RSSI P0.5
SI_SBIT(Port_RSSI, SFR_P0, 5);

// TX_FAULT_IN P0.6
SI_SBIT(Port_TX_FAULT_IN, SFR_P0, 6);

// VCCT_1V8EN P1.0
SI_SBIT(Port_VCCT_1V8EN, SFR_P1, 0);

// RESETB P1.1
SI_SBIT(Port_RESETB, SFR_P1, 1);

// VCCR_1V8EN P1.2
SI_SBIT(Port_VCCR_1V8EN, SFR_P1, 2);

// TX_DIS_LD P1.5
SI_SBIT(Port_TX_DIS_LD, SFR_P1, 5);

// TX_DIS P1.6
SI_SBIT(Port_TX_DIS, SFR_P1, 6);

// TX_FAULT_OUT P2.3
SI_SBIT(Port_TX_FAULT_OUT, SFR_P2, 3);

extern void	Port_Init(void);

#endif

