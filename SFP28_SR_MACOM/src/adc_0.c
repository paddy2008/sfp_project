//-----------------------------------------------------------------------------
//      Copyright (C) 2019
//-----------------------------------------------------------------------------
//
// file name  :       adc_0.c
//
// description:       adc Setup
//
// Target     :       EFM8LB12F32ES0
//
// version    :       V100.001
//					  2019/07/30 by
//
//-----------------------------------------------------------------------------

#include "include.h"

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures ADC0 initialization
//
//-----------------------------------------------------------------------------
void	ADC0_Init(void)
{
	uint8_t SFRPAGE_save = SFRPAGE;

	SFRPAGE = 0x00;

	//PAC_DISABLED
	//ADC0 conversion initiated on write of 1 to ADBUSY.
	ADC0CN2 = ADC0CN2_ADCM__ADBUSY | ADC0CN2_PACEN__PAC_DISABLED;

	//ADC0 operates in 12-bit mode.
	//Right justified. No shifting applied.
	//Perform and Accumulate 1 conversions.
	ADC0CN1 = ADC0CN1_ADBITS__14_BIT | ADC0CN1_ADSJST__RIGHT_NO_SHIFT | ADC0CN1_ADRPT__ACC_1;

	//The ADC0 voltage reference is the internal voltage reference,
	//The reference is nominally 2.4 V.if MCU needs to uses the temperature sensor£¬so internal voltage reference must be selected
	//because the result of calculating is lower deviation
	//Power Up Delay Time = 1.46us
	ADC0CF2 = ADC0CF2_GNDSL__GND_PIN | ADC0CF2_REFSL__INTERNAL_VREF | (0x04 << ADC0CF2_ADPWR__SHIFT);

	//ADC0CF0 = ADC0CF0_ADCLKSEL__SYSCLK | (0x01 << ADC0CF0_ADSC__SHIFT);
	//ADC0CF0 = ((ADC0_SYSCLK/ADC0SARCLK)-1)<<3;  // Set SAR clock to 12MHz
	 ADC0CF0 = ADC0CF0_ADCLKSEL__SYSCLK | (0x05 << ADC0CF0_ADSC__SHIFT);

	//Enable low power mode.
	//Conversion Tracking Time = 5us
	 ADC0CF1 = ADC0CF1_ADLPM__LP_DISABLED | (0x03 << ADC0CF1_ADTK__SHIFT);

	//Autoscan Single Trigger Enable
	SFRPAGE = 0x30;
	ADC0ASCF |= ADC0ASCF_STEN__SINGLE_TRIGGER;

	//Disable ADC0 (low-power shutdown).
	//Power down when ADC is idle (not converting).
	//clear Conversion Complete Interrupt Flag
	//clear ADC Busy
	//clear Window Compare Interrupt Flag
	//The on-chip PGA gain is 1.0.
	//enable the Temperature Sensor
	SFRPAGE = 0x00;
	// ADC0CN0 &= ~ADC0CN0_ADGN__FMASK;
	ADC0CN0 |= ADC0CN0_ADEN__ENABLED | ADC0CN0_ADGN__GAIN_0P25 | ADC0CN0_IPOEN__POWER_DOWN | ADC0CN0_TEMPE__TEMP_ENABLED;

	// EIE1 |= 0x08;  // ADC0 Conversion Complete Interrupt Disable

	// Restore the SFRPAGE
	SFRPAGE = SFRPAGE_save;
}


//-----------------------------------------------------------------------------
// ADC0_Read
//-----------------------------------------------------------------------------
//
// Return Value:  None
// Parameters:    None
//
// Configures:    ADC0_Read
//
//-----------------------------------------------------------------------------
uint16_t ADC0_Read(ADC0_PositiveInput_t input)
{
	uint8_t  ADCM_save;
	uint16_t result;

	DECL_PAGE;
	SET_PAGE(ADC0_SFRPAGE);

    // Set the ADC input
    ADC0MX  = input;

	// Start a conversion
	// ADC0_startConversion();
	// Save the conversion source and set to ADBUSY
	ADCM_save = ADC0CN2 & ADC0CN2_ADCM__FMASK;
	ADC0CN2   = (ADC0CN2 & ~ADC0CN2_ADCM__FMASK) | ADC0CN2_ADCM__ADBUSY;

	// Clear the conversion complete flag
	ADC0CN0_ADINT = 0;

	// Start a conversion by setting ADBUSY
	ADC0CN0_ADBUSY = 1;

	// Restore the conversion source
	ADC0CN2 |= ADCM_save;

	// Wait for conversion to complete
	// while(!ADC0_isConversionComplete());
	while(!ADC0CN0_ADINT);

	// Get the result
	// return ADC0_getResult();
	result = ADC0;

	RESTORE_PAGE;

	return (result);
}
