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

#include "include.h"

//-----------------------------------------------------------------------------
// WDT0_start
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters  :  None
//
// Configures  :  WDT0 start
//
//-----------------------------------------------------------------------------
void WDT0_start(void)
{
	// WDTCN SFR Page = ALL;
	WDTCN = 0xA5;
}


//-----------------------------------------------------------------------------
// WDT0_stop
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters  :  None
//
// Configures  :  WDT0 stop
//
//-----------------------------------------------------------------------------
void WDT0_stop(void)
{
    bool val = IE_EA;

    IE_EA = 0;

    // WDTCN SFR Page = ALL;
    WDTCN = 0xDE;
    WDTCN = 0xAD;

    IE_EA = val;
}


//-----------------------------------------------------------------------------
// WDT0_feed
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters  :  None
//
// Configures  :  WDT0 feed
//
//-----------------------------------------------------------------------------
void WDT0_feed(void)
{
	// WDTCN SFR Page = ALL;
	WDTCN = 0xA5;
}


//-----------------------------------------------------------------------------
// WDT0_init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters  :  None
//
// Configures  :  WDT0 init and set over time
//
//-----------------------------------------------------------------------------
void WDT0_init(uint16_t interval_ms)
{
	// WDTCN SFR Page = ALL;
	WDTCN = WDT0_ticksToInterval( (uint32_t)interval_ms * LFOCLK_KHZ );
    WDT0_start();
}

