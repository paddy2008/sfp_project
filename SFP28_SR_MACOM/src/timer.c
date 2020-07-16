//-----------------------------------------------------------------------------
//      Copyright (c) 2017-2018,
//-----------------------------------------------------------------------------
//
// file name  :       timer.c
//
// description:       timer set file
//
// Target     :
//
// version    :       V100.001
//					  2019/06/16
//-----------------------------------------------------------------------------
#include "include.h"


SI_SEGMENT_VARIABLE( Module_Status_s,   extern struct Module_Status_struct,   SI_SEG_DATA  );
// -----------------------------------------------------------------------------
// Variable Declarations

//-----------------------------------------------------------------------------
// delay_us
//-----------------------------------------------------------------------------
//
// Return Value:  ADC positive input
// Parameters:    delay us
//
// Configures     delay us
//
//-----------------------------------------------------------------------------
void delay_us( uint32_t delay_us )
{
	volatile int32_t i = 0;

	for( i = delay_us; i > 0; i-- )
	{
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	}
}




void TIMER2_init(void)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE   = 0x0;

	TMR2CN0 &= ~0x04;					//disable timer2
	TMR2CN0 &= ~0x08;					//set timer2 into 16 bit modes

	CKCON0 &= ~0x30;					//Use system clock divided by 12
	TMR2CN0 &= ~0x03;

    TMR2RL = -(SYSCLK/12/50);           // Reload value to be used in Timer2
    TMR2 = TMR2RL;                      // Init the Timer2 register,Every 1 milliseconds£¨1ms >>> 3*(1/18000000)£© overflow a time

    TMR2CN0 |= 0x04;                    // Enable Timer2 in auto-reload mode
    IE_ET2 = 1;                         // Timer2 interrupt enable
    SFRPAGE = SFRPAGE_SAVE;
}


//-----------------------------------------------------------------------------
// delay_us
//-----------------------------------------------------------------------------
//
// Return Value:  ADC positive input
// Parameters:    delay us
//
// Configures     delay us
//
//-----------------------------------------------------------------------------
void TIMER3_Init(void)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE   = 0x0;

	TMR3CN0 &= ~0x04;					//disable timer3
	TMR3CN0 &= ~0x08;					//set timer3 into 16 bit modes

	CKCON0  &= ~0xC0;					//Use system clock divided by 12
	TMR3CN0 &= ~0x03;

	TMR3RL = -(SYSCLK/12/50);           // Reload value to be used in Timer2
	TMR3 = TMR3RL;                      // Init the Timer3 register,Every 20 milliseconds 1ms >>> 3*(1/18000000) overflow a time

	TMR3CN0 |= 0x04;                    // Enable Timer3 in auto-reload mode
	EIE1 |= 0x80;                       // Timer3 interrupt enable

	TMR3CN0 &= ~0x04;					//disable timer3
	SFRPAGE = SFRPAGE_SAVE;
}



void TIMER4_Init(void)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	uint8_t TMR4CN0_TR4_save;

	SFRPAGE = 0x10;

	TMR4CN0_TR4_save = TMR4CN0 & TMR4CN0_TR4__BMASK;

	// Stop Timer
	TMR4CN0 &= ~(TMR4CN0_TR4__BMASK);
	TMR4RLH  = (0x38 << TMR4RLH_TMR4RLH__SHIFT);
	TMR4RLL  = (0x9E << TMR4RLL_TMR4RLL__SHIFT);
	TMR4CN0 |= TMR4CN0_TR4__RUN;
	TMR4CN0 |= TMR4CN0_TR4_save;
	SFRPAGE = SFRPAGE_SAVE;
}


//-----------------------------------------------------------------------------
// TIMER2_ISR
//-----------------------------------------------------------------------------
//
// TIMER2 ISR Content goes here. Remember to clear flag bits:
// TMR2CN0::TF2H (Timer # High Byte Overflow Flag)
// TMR2CN0::TF2L (Timer # Low Byte Overflow Flag)
//
//-----------------------------------------------------------------------------
SI_INTERRUPT_USING(TIMER2_ISR, TIMER2_IRQn, 2)
{
	Module_Status_s.Flush_DOMData = 1;
	TMR2CN0_TF2H = 0;  // Clear the interrupt flag
}









