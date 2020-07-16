//====================================================
//
//	File			: 100G_sfp_efm8lb1_pid_defines.h
//
//	Hardware		: Applicable to ADuC7023 32CSP EVB
//
//	C Compiler	: uVision V4.70.0.0; Armcc V4.0.0.524 [Evalution]
//
//====================================================
#ifndef __sfp_EFM8LB1_pid_h__
#define __sfp_EFM8LB1_pid_h__
#include <stdint.h>
#include "dac_3.h"
#include "version.h"

#ifdef SUPPORT_TEC_FUNC

#define		SUCCEED		(0)
#define		FAILED		(1)

#define		OFF			(0)
#define		ON			(1)

#define TEC_CONTROL_LIMITER_MAX 	(15500)	// 1544//16000
#define TEC_CONTROL_LIMITER_MIN 	(9500)	// 990//9600
#define TEC_CONTROL_BALANCE_VAL 	(12500)

// 12 bit DAC
#define DAC_MAX_RESULT  ((1 << 12)-1)


typedef struct
{
	uint16_t	targetPoint; 	// in 0.1mV
	uint16_t	tempSetPoint;	// in 0.1mV
	uint16_t	tempOutPoint;	// in 0.1mV
	int16_t		lastDelta;		// Error[-1]
	int16_t		prevDelta;		// Error[-2]
	int32_t		sumDelta;		// Sums of Errors

	float 	  proportion;		// Proportional
	float     integral;			// Integral
	float     derivative;		// Derivative

} PID_ST;

void		PID_init(void);
uint8_t		PID_cal(void);
uint16_t	PID_get_tempset(void);
uint16_t	PID_get_tempout(void);

#endif
#endif

