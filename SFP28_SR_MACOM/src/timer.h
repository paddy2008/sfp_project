//-----------------------------------------------------------------------------
//      Copyright (c) 2017-2018,
//-----------------------------------------------------------------------------
//
// file name  :       timer.h
//
// description:       timer set file
//
// Target     :       C8051F34x / C8051F38x
//
// version    :       V100.001
//					  2017/11/16 by roy zhou
//
//-----------------------------------------------------------------------------
#ifndef _TIMER_H__
#define _TIMER_H__
 
// -----------------------------------------------------------------------------
// Global Constants
#define BLINK_RATE         500          // ms per blink
#define SYSCLK             72000000     // SYSCLK frequency in Hz
#define PRESCALE           48           // Timer prescaler
#define TIMER_RELOAD 	   -(SYSCLK / PRESCALE / 1000)

#define TIMER_RELOAD_HIGH ((TIMER_RELOAD & 0xFF00)>>8)
#define TIMER_RELOAD_LOW   (TIMER_RELOAD & 0x00FF)

// 1us - 10ms
extern void delay_us( uint32_t delay_us );
extern void TIMER2_Init(void);
extern void TIMER3_Init(void);
extern void TIMER4_Init(void);

#endif


