//-----------------------------------------------------------------------------
//      Copyright (c)
//-----------------------------------------------------------------------------
//
// file name  :       adc_0.h
//
// description:       adc Setup
//
// Target     :       EFM8LB12F32ES0
//
// version    :       V100.001
//					  2019/07/30/ by
//
//-----------------------------------------------------------------------------
#ifndef __ADC_0_H__
#define __ADC_0_H__
 
#include "SI_EFM8LB1_Register_Enums.h"

/// Positive input selection enums.
typedef enum {
    ADC0_POSITIVE_INPUT_P0, //!< ADC0P0 is ADC Positive Input P0.1
    ADC0_POSITIVE_INPUT_P1, //!< ADC0P1 is ADC Positive Input P0.2
    ADC0_POSITIVE_INPUT_P2, //!< ADC0P2 is ADC Positive Input P0.4
    ADC0_POSITIVE_INPUT_P3, //!< ADC0P3 is ADC Positive Input P0.5
    ADC0_POSITIVE_INPUT_P4, //!< ADC0P4 is ADC Positive Input P0.6
    ADC0_POSITIVE_INPUT_P5, //!< ADC0P5 is ADC Positive Input P0.7
    ADC0_POSITIVE_INPUT_P6, //!< ADC0P6 is ADC Positive Input P1.0
    ADC0_POSITIVE_INPUT_P7, //!< ADC0P7 is ADC Positive Input P1.1
    ADC0_POSITIVE_INPUT_P8, //!< ADC0P8 is ADC Positive Input P1.2
    ADC0_POSITIVE_INPUT_P9, //!< ADC0P9 is ADC Positive Input P1.3
    ADC0_POSITIVE_INPUT_P10, //!< ADC0P10 is ADC Positive Input P1.4
    ADC0_POSITIVE_INPUT_P11, //!< ADC0P11 is ADC Positive Input P1.5
    ADC0_POSITIVE_INPUT_P12, //!< ADC0P12 is ADC Positive Input P1.6
    ADC0_POSITIVE_INPUT_P13, //!< ADC0P13 is ADC Positive Input P1.7
    ADC0_POSITIVE_INPUT_P14, //!< ADC0P14 is ADC Positive Input P2.1
    ADC0_POSITIVE_INPUT_P15, //!< ADC0P15 is ADC Positive Input P2.2
    ADC0_POSITIVE_INPUT_P16, //!< ADC0P16 is ADC Positive Input P2.3
    ADC0_POSITIVE_INPUT_P17, //!< ADC0P17 is ADC Positive Input P2.4
    ADC0_POSITIVE_INPUT_P18, //!< ADC0P18 is ADC Positive Input P2.5
    ADC0_POSITIVE_INPUT_P19, //!< ADC0P19 is ADC Positive Input P2.6
    ADC0_POSITIVE_INPUT_TEMP, //!< Internal Temperature Sensor is ADC Positive Input
    ADC0_POSITIVE_INPUT_LDO_OUT, //!< Internal 1.8 V LDO Output is ADC Positive Input
    ADC0_POSITIVE_INPUT_VDD, //!< VDD Supply Pin is ADC Positive Input
    ADC0_POSITIVE_INPUT_GND, //!< GND Supply Pin is ADC Positive Input
    ADC0_POSITIVE_INPUT_NONE = 31, //!< ADC Positive Input is disconnected
}ADC0_PositiveInput_t;

#define ADC0_SFRPAGE 0x30

// Address in flash where the temp cal low byte value is stored
#define TEMP_CAL_ADDRESS_LOW     (0xFFD4)

// Address in flash where the temp cal high byte value is stored
#define TEMP_CAL_ADDRESS_HIGH    (0xFFD5)


extern void	ADC0_Init(void);
extern uint16_t ADC0_Read(ADC0_PositiveInput_t input);

#endif

