//-----------------------------------------------------------------------------
//      Copyright (c) 2017-2018,
//-----------------------------------------------------------------------------
//
// file name  :       wdt_0.c
//
// description:       WDT Setup
//
// Target     :       EFM8LB12F32ES0
//
// version    :       V100.001
//					  2019/06/16
//
//-----------------------------------------------------------------------------
#ifndef __WDT_0_H__
#define __WDT_0_H__
 
//#include "SI_EFM8LB1_Register_Enums.h"

#define LFOCLK_KHZ 80UL   // LFOSC default frequency, in KHz

#define WDT0_ticksToInterval(ticks) (\
 (((ticks) > 262144) ? 7 : \
 (((ticks) > 65536 ) ? 6 : \
 (((ticks) > 16384 ) ? 5 : \
 (((ticks) > 4096  ) ? 4 : \
 (((ticks) > 1024  ) ? 3 : \
 (((ticks) > 256   ) ? 2 : \
 (((ticks) > 64    ) ? 1 : \
  0 ))))))))
  
extern void WDT0_init(uint16_t interval_ms);
extern void WDT0_feed(void);

#endif


