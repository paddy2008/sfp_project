//-----------------------------------------------------------------------------
//      Copyright (c) 2017-2018,
//-----------------------------------------------------------------------------
//
// file name  :       InitDevice.c
//
// description:       mcu pin define
//
// Target     :       EFM8LB12F32ES0
//
// version    :       V100.001
//					  2019/07/30 by roy zhou
//
//-----------------------------------------------------------------------------
// USER INCLUDES
#include "Include.h"

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures Init Pin
//
//-----------------------------------------------------------------------------
void	Port_Init(void)
{
	uint8_t SFRPAGE_save = SFRPAGE;

	// push-pull = 1, Open-Drain = 0
	// Digital   = 1, Analog     = 0

	// IO    -  Skip,		    Port, 	MDOUT,      	   MDIN			  USE

	// P0.0  -  Skipped,        0		Open-Drain, 	   Digital        none
	// P0.1  -  Skipped,        0		Open-Drain, 	   Digital        none
	// P0.2  -  Skipped,        1		Open-Drain,        Digital        MSDA
	// P0.3  -  Skipped,        1		Open-Drain, 	   Digital        MSCL
	// P0.4  -  Skipped,        0		Open-Drain,        Digital        none
	// P0.5  -  Skipped,        0		Open-Drain,        Analog         RSSI
	// P0.6  -  Skipped,        0		Open-Drain,        Digital        TX_FAULT_IN
	// P0.7  -  Skipped,        0		Open-Drain,        Analog         VCCR_2
	SFRPAGE = 0x20;
	P0      = 0x0C;
	P0MDIN  = 0x9F;
	P0MDOUT = 0x00;
	P0SKIP  = 0xFF;
	P0MASK  = 0x00;
	P0MAT   = 0x00;

	// P1.0  -  Skipped,        1		push-pull,         Digital        VCCT_1V8EN
	// P1.1  -  Skipped,    	0		push-pull, 	       Digital        RESETB
	// P1.2  -  Skipped,    	1		push-pull, 	   	   Digital        VCCR_1V8EN
	// P1.3  -  Skipped,        1		Open-Drain, 	   Digital        SDA
	// P1.4  -  Skipped,        1		Open-Drain, 	   Digital        SCL
	// P1.5  -  Skipped,        1		push-pull, 	       Digital        TX_DIS_LD
	// P1.6  -  Skipped,        1		Open-Drain, 	   Digital        TX_DIS
	// P1.7  -  Skipped,        0		Open-Drain, 	   Digital        none
	SFRPAGE = 0x20;
	P1      = 0x7D;
	P1MDIN  = 0xFF;
	P1MDOUT = 0x27;
	P1SKIP  = 0xFF;
	P1MASK  = 0x00;
	P1MAT   = 0x00;

	// P2.0  -  Skipped,        0		Open-Drain, 	   Digital        none
	// P2.1  -  Skipped,        0		Open-Drain, 	   Digital        none
	// P2.2  -  Skipped,        0		Open-Drain,        Digital        none
	// P2.3  -  Skipped,        0		push-pull,         Digital        TX_FAULT_OUT
	// P2.4  -  Skipped,        0		Open-Drain,        Digital        none
	// P2.5  -  Skipped,        0		Open-Drain,        Digital        none
	// P2.6  -  Skipped,        0		Open-Drain,        Digital        none
	// P2.7  -  Skipped,        0		Open-Drain,        Digital        none
	SFRPAGE = 0x20;
	P2      = 0x00;
	P2MDIN  = 0xFF;
	P2MDOUT = 0x08;
	P2SKIP  = 0xFF;
	P2MASK  = 0x00;
	P2MAT   = 0x00;

	// P3.0  -  Skipped,        0		Open-Drain, 	   Digital        none
	// P3.1  -  Skipped,        0		Open-Drain, 	   Digital        none
	// P3.2  -  Skipped,        0		Open-Drain,        Digital        none
	// P3.3  -  Skipped,        0		Open-Drain,        Digital        none
	// P3.4  -  Skipped,        0		Open-Drain,        Digital        none
	// P3.5  -  Skipped,        0		Open-Drain,        Digital        none
	// P3.6  -  Skipped,        0		Open-Drain,        Digital        none
	// P3.7  -  Skipped,        0		Open-Drain,        Digital        none
	SFRPAGE = 0x20;
	P3      = 0x00;
	P3MDIN  = 0xFF;
	P3MDOUT = 0x00;

	SFRPAGE = 0x20;
	XBR0 = 0x00;
	XBR1 = 0x00;
	XBR2 = XBR2_XBARE__ENABLED; // Enable the crossbar

	SFRPAGE = 0x00;
	EIE2 = EIE2_CL0__DISABLED | EIE2_EI2C0__ENABLED | EIE2_ET4__ENABLED
			| EIE2_ET5__DISABLED | EIE2_ES1__DISABLED;

	IE = IE_EA__ENABLED | IE_EX0__DISABLED | IE_EX1__DISABLED
			| IE_ESPI0__DISABLED | IE_ET0__DISABLED | IE_ET1__DISABLED
			| IE_ET2__DISABLED | IE_ES0__DISABLED;

	// Restore the SFRPAGE
	SFRPAGE = SFRPAGE_save;
}































