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

volatile uint8_t data_sync_ready = 0;

static void port_external_int0_init(void)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE   = 0x0;
	TCON 	 |= TCON_IT0__EDGE;	 			//edge  trigger
	IT01CF 	 |= IT01CF_IN0SL__P0_7;  		//p0.7 is defined as GPIO
	IT01CF 	 |= IT01CF_IN0PL__ACTIVE_HIGH;	//up edge trigger interrupt
	IP       |= 0x01;   					//set int0 interrupt the highest pri
	SFRPAGE   = 0x10;
	IPH      |= 0x01;
	SFRPAGE   = SFRPAGE_SAVE;
	IE       &= ~IE_EX0__ENABLED ;
}


SI_INTERRUPT (INT0_ISR, INT0_IRQn)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE  = 0x0;
	TCON    &= ~TCON_IE0__SET;  //clear extern interrupt0 flag
	SFRPAGE  = SFRPAGE_SAVE;

	data_sync_ready = 1;
}

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
	P0      = 0xFF;
	P0MDIN  = 0xFF;
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
	P1      = 0xFF;
	P1MDIN  = 0xFF;
	P1MDOUT = 0x00;
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
	P2      = 0xFF;
	P2MDIN  = 0xFF;
	P2MDOUT = 0x00;
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
	P3      = 0xFF;
	P3MDIN  = 0xFF;
	P3MDOUT = 0x00;

	SFRPAGE = 0x20;
	XBR0 = 0x00;
	XBR1 = 0x00;
	XBR2 = XBR2_XBARE__ENABLED; // Enable the crossbar


	port_external_int0_init();

	// Restore the SFRPAGE
	SFRPAGE = SFRPAGE_save;
}































