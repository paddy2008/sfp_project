#include "Efm8lb12_common.h"
#include "Efm8lb12_adc.h"
#include "Efm8lb12_msa.h"
#include "assert.h"

ADC0_NEW_AUTOSCAN_BUFFER_ARRAY(scan_buf,NUM_SCANS,BUFFER_SCAN_TMP_ADDR);

// Calibration value for the temperature sensor at 0 degrees C, stored in CODE space
SI_LOCATED_VARIABLE_NO_INIT(TEMPSENSOR_0C_LOW,  uint8_t const, SI_SEG_CODE, TEMP_CAL_ADDRESS_LOW);
SI_LOCATED_VARIABLE_NO_INIT(TEMPSENSOR_0C_HIGH, uint8_t const, SI_SEG_CODE, TEMP_CAL_ADDRESS_HIGH);

typedef struct {
	ADC0_PositiveInput_t	pin;
	uint16_t				val;
	uint8_t					autoscan;
	uint8_t					enabled;		// enabled or disabled on boot up
}ADC0_INPUT_SCAN_T;

typedef struct {
	ADC0_INPUT_SCAN_T	mcu_temp;
	ADC0_INPUT_SCAN_T	vcc;
	ADC0_INPUT_SCAN_T	rssi[MSA_CHANNEL_NUM];
	ADC0_INPUT_SCAN_T	mpd[MSA_CHANNEL_NUM];
#ifdef SUPPORT_TEC_FUNC
	ADC0_INPUT_SCAN_T	itec;

#if defined(HW_VER_AM1_301) || defined(HW_VER_AM2_301) || defined(HW_VER_AM3_301)
	ADC0_INPUT_SCAN_T	vtec;
#endif

	ADC0_INPUT_SCAN_T	tec_tempout;
#endif
}ADC0_INPUT_SCAN_SET_T;


static ADC0_INPUT_SCAN_SET_T _adc0_scan_set = {
	{ ADC0_MCU_TEMP,	0,	true, true },
	{ ADC0_VCC, 		0,	true, true },
	// rssi (rxpower)
#ifdef EFM8LB12E64_100G_SFP_LR4_301
	{
		{ ADC0_RSSI0,		0,	true, true },
		{ ADC0_RSSI1,		0,	true, true },
		{ ADC0_RSSI2,		0,	true, true },
		{ ADC0_RSSI3,		0,	true, true },
	},
	{	// mpd (txpowre)
		{ ADC0_MPD0, 		0,	true, true },
		{ ADC0_MPD1,		0,	true, true },
		{ ADC0_MPD2,		0,	true, true },
		{ ADC0_MPD3,		0,	true, true },
	},

	#if defined(HW_VER_AM1_301) || defined(HW_VER_AM2_301) || defined(HW_VER_AM3_301)
		#ifdef SUPPORT_TEC_FUNC
			{ ADC0_ITEC,		0,	true, true },
			{ ADC0_VTEC,		0,	true, true },
			{ ADC0_TEC_TEMPOUT,	0,	true, true },
		#endif
	#endif

	#if defined(HW_VER_AM4_301)
		#ifdef SUPPORT_TEC_FUNC
			{ ADC0_ITEC,		0,	true, true },
			{ ADC0_TEC_TEMPOUT,	0,	true, true },
		#endif
	#endif
#endif

#ifdef EFM8LB12E64_100G_SFP_AOC_101
	{
		{ ADC0_RSSI0,		0,	false, false },
		{ ADC0_RSSI1,		0,	false, false },
		{ ADC0_RSSI2,		0,	false, false },
		{ ADC0_RSSI3,		0,	false, false },
	},
	{	// mpd (txpowre)
		{ ADC0_MPD0,		0,	true, true },
		{ ADC0_MPD1,		0,	true, true },
		{ ADC0_MPD2,		0,	true, true },
		{ ADC0_MPD3,		0,	true, true },
	},
#endif

#ifdef EFM8LB12E64_100G_SFP_CWDM4_202
	{
		{ ADC0_RSSI0,		0,	true, true },
		{ ADC0_RSSI1,		0,	true, true },
		{ ADC0_RSSI2,		0,	true, true },
		{ ADC0_RSSI3,		0,	true, true },
	},
	{	// mpd (txpowre)
		{ ADC0_MPD0,		0,	false, true },
		{ ADC0_MPD1,		0,	false, true },
		{ ADC0_MPD2,		0,	false, true },
		{ ADC0_MPD3,		0,	false, true },
	},
#endif

#ifdef EFM8LB12E64_100G_SFP_CWDM4_206
	{
		{ ADC0_RSSI0,		0,	false, true },
		{ ADC0_RSSI1,		0,	false, true },
		{ ADC0_RSSI2,		0,	false, true },
		{ ADC0_RSSI3,		0,	false, true },
	},
#ifdef HW_VER_AM1_206
	{	// mpd (txpowre)
		{ ADC0_MPD0,		0,	false, true },
		{ ADC0_MPD1,		0,	false, true },
		{ ADC0_MPD2,		0,	false, true },
		{ ADC0_MPD3,		0,	false, true },
	},
#endif

#ifdef HW_VER_AM2_206
	{	// mpd (txpowre)
		{ ADC0_MPD0,		0,	true, true },
		{ ADC0_MPD1,		0,	true, true },
		{ ADC0_MPD2,		0,	true, true },
		{ ADC0_MPD3,		0,	true, true },
	},
#endif
#endif

#ifdef EFM8LB12E64_100G_SFP_CWDM4_207
	{
		{ ADC0_RSSI0,		0,	false, true },
		{ ADC0_RSSI1,		0,	false, true },
		{ ADC0_RSSI2,		0,	false, true },
		{ ADC0_RSSI3,		0,	false, true },
	},

	{
		{ ADC0_MPD0,		0,	true, true },
		{ ADC0_MPD1,		0,	true, true },
		{ ADC0_MPD2,		0,	true, true },
		{ ADC0_MPD3,		0,	true, true },
	},

#endif

#ifdef EFM8LB12E64_100G_SFP_CWDM4_211
	{
		{ ADC0_RSSI0,		0,	false, true },
		{ ADC0_RSSI1,		0,	false, true },
		{ ADC0_RSSI2,		0,	false, true },
		{ ADC0_RSSI3,		0,	false, true },
	},

	{
		{ ADC0_MPD0,		0,	true, true },
		{ ADC0_MPD1,		0,	true, true },
		{ ADC0_MPD2,		0,	true, true },
		{ ADC0_MPD3,		0,	true, true },
	},

#endif
};

static void adc0_clearIntFlags_by_isr(uint8_t flags)
{
  uint8_t savedPage = SFRPAGE;

  SFRPAGE = 0x30;
  // Must pass one of the valid flags
  SLAB_ASSERT(flags & (ADC0_CONVERSION_COMPLETE_IF | ADC0_WINDOW_COMPARE_IF));

  ADC0CN0 &= ~(flags & (ADC0_CONVERSION_COMPLETE_IF | ADC0_WINDOW_COMPARE_IF));

  SFRPAGE = savedPage;
}

/* return voltage in 0.1mV */
static uint16_t adc0_normalization(const uint16_t * buffer, uint8_t size)
{
	uint32_t i, sum = 0;
	
	for (i = 0; i < size; i++)
	  sum += (buffer[i] & ADC_MAX_RESULT);

	return (uint16_t)(((float)sum / (float)size * ADC_VREF_MV * 2) / ADC_MAX_RESULT);
}

static uint16_t adc0_do_measure(ADC0_PositiveInput_t input_pin)
{
	ADC0_setAutoscanInputs(input_pin, 1);
	ADC0_enableAutoscan((ADC0_AutoscanBuffer_t *)scan_buf, NUM_SCANS, ADC0_AUTOSCAN_MODE_SINGLE);
	ADC0_startConversion();
	while(!ADC0_isConversionComplete());  // Wait for scan to complete
	ADC0_clearIntFlags(ADC0_CONVERSION_COMPLETE_IF);
	return adc0_normalization(&scan_buf[0], NUM_SCANS);
}

void ADC0_Init(void)
{
	//PAC_DISABLED
	//ADC0 conversion initiated on write of 1 to ADBUSY.
	ADC0CN2 = ADC0CN2_ADCM__ADBUSY | ADC0CN2_PACEN__PAC_DISABLED;

	//ADC0 operates in 12-bit mode.
	//Right justified. No shifting applied.
	//Perform and Accumulate 1 conversions.
	ADC0CN1 = ADC0CN1_ADBITS__14_BIT | ADC0CN1_ADSJST__RIGHT_NO_SHIFT | ADC0CN1_ADRPT__ACC_1;


	//The ADC0 voltage reference is the internal voltage reference,
	//The reference is nominally 2.4 V.if MCU needs to uses the temperature sensor£¬so internal voltage reference must be selected
	//because the result of calculating is lower deviation(Îó²î)
	//Power Up Delay Time = 1.46us
	ADC0CF2 = ADC0CF2_GNDSL__GND_PIN | ADC0CF2_REFSL__INTERNAL_VREF | (0x04 << ADC0CF2_ADPWR__SHIFT);

	//ADC0CF0 = ADC0CF0_ADCLKSEL__SYSCLK | (0x01 << ADC0CF0_ADSC__SHIFT);
	//ADC0CF0 = ((ADC0_SYSCLK/ADC0SARCLK)-1)<<3;  // Set SAR clock to 12MHz
	 ADC0CF0 = ADC0CF0_ADCLKSEL__SYSCLK | (0x05 << ADC0CF0_ADSC__SHIFT);

	//Enable low power mode.
	//Conversion Tracking Time = 5us
	ADC0CF1 = ADC0CF1_ADLPM__LP_ENABLED | (0x3D << ADC0CF1_ADTK__SHIFT);

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
	ADC0CN0 &= ~ADC0CN0_ADGN__FMASK;
	ADC0CN0 |= ADC0CN0_ADEN__ENABLED | ADC0CN0_ADGN__GAIN_0P5 | ADC0CN0_IPOEN__POWER_DOWN | ADC0CN0_TEMPE__TEMP_ENABLED;

	//EIE1 |= 0x08;  //ADC0 Conversion Complete Interrupt Disable
}


/* return voltage in 0.1mV */
uint16_t ADC0_DealSingleAnalogInputData(uint16_t *buff)
{
	uint8_t i = 0, scan = 0;
	uint32_t sum = 0;
	uint16_t result = 0;

	for (scan = 0; scan < NUM_SCANS; scan++)
	{
		sum += ADC0_ConvertSampleToMillivolts(buff[scan]);
	}
	result = (uint16_t)((sum + (NUM_SCANS >> 1))/NUM_SCANS);
	return result;
}


/* return in 0.1mV */
uint16_t ADC0_ConvertSampleToMillivolts(uint16_t sample)
{
  // The measured voltage is given by:
  //
  //                           Vref (mV)
  //   measurement (mV) =   --------------- * result (bits)
  //                       (2^14)-1 (bits)
  return (uint16_t)(((uint32_t)(sample & ADC_MAX_RESULT) * ADC_VREF_MV * 2) / ADC_MAX_RESULT);
}

void ADC0_poll()
{
	uint8_t i;

	if (_adc0_scan_set.mcu_temp.autoscan && _adc0_scan_set.mcu_temp.enabled)
		_adc0_scan_set.mcu_temp.val = adc0_do_measure(_adc0_scan_set.mcu_temp.pin);
	
	if (_adc0_scan_set.vcc.autoscan && _adc0_scan_set.vcc.enabled)
		_adc0_scan_set.vcc.val = adc0_do_measure(_adc0_scan_set.vcc.pin);

	for (i = 0; i < MSA_CHANNEL_NUM; i++)
	{
		if (_adc0_scan_set.rssi[i].autoscan && _adc0_scan_set.rssi[i].enabled)
			_adc0_scan_set.rssi[i].val = adc0_do_measure(_adc0_scan_set.rssi[i].pin);
	}

	for (i = 0; i < MSA_CHANNEL_NUM; i++)
	{
		if (_adc0_scan_set.mpd[i].autoscan && _adc0_scan_set.mpd[i].enabled)
			_adc0_scan_set.mpd[i].val = adc0_do_measure(_adc0_scan_set.mpd[i].pin);
	}

#ifdef SUPPORT_TEC_FUNC
	if (_adc0_scan_set.itec.autoscan && _adc0_scan_set.itec.enabled)
		_adc0_scan_set.itec.val = adc0_do_measure(_adc0_scan_set.itec.pin);


#if defined(HW_VER_AM1_301) || defined(HW_VER_AM2_301) || defined(HW_VER_AM3_301)
	if (_adc0_scan_set.vtec.autoscan && _adc0_scan_set.vtec.enabled)
		_adc0_scan_set.vtec.val = adc0_do_measure(_adc0_scan_set.vtec.pin);
#endif

#endif

	return;
}

/* return in 1/256 C */
int16_t ADC0_get_mcu_temp()
{
	uint16_t tempsensor_0c = 0;   // Stores the temp sensor cal value at 0C as [high byte][low byte]
	float code_temp = 0;
	tempsensor_0c = (TEMPSENSOR_0C_HIGH << 8) | TEMPSENSOR_0C_LOW;
	code_temp=(float)_adc0_scan_set.mcu_temp.val * ADC_MAX_RESULT/ADC_VREF_MV/2;
	return (int16_t)((code_temp* 2.0 - (float)tempsensor_0c) / (float)28 * DDMI_TEMP_MSA_SCALE);
}

uint16_t ADC0_get_vcc()
{
	return _adc0_scan_set.vcc.val * 2;
}

uint16_t ADC0_get_rssi(uint8_t ch)
{
	return _adc0_scan_set.rssi[ch].val;
}

uint16_t ADC0_get_mpd(uint8_t ch)
{
	return _adc0_scan_set.mpd[ch].val;
}

uint16_t ADC0_manual_poll(ADC0_PositiveInput_t input_pin)
{
	return adc0_do_measure(input_pin);
}

void ADC0_enable_input(ADC0_PositiveInput_t input_pin, uint8_t en)
{
	if(input_pin == ADC0_POSITIVE_INPUT_NONE)
		return;
	
	if(input_pin == ADC0_RSSI0)
		_adc0_scan_set.rssi[0].enabled = en;

	if(input_pin == ADC0_RSSI1)
		_adc0_scan_set.rssi[1].enabled = en;

	if(input_pin == ADC0_RSSI2)
		_adc0_scan_set.rssi[2].enabled = en;

	if(input_pin == ADC0_RSSI3)
		_adc0_scan_set.rssi[3].enabled = en;

	if(input_pin == ADC0_MPD0)
		_adc0_scan_set.mpd[0].enabled = en;

	if(input_pin == ADC0_MPD1)
		_adc0_scan_set.mpd[1].enabled = en;

	if(input_pin == ADC0_MPD2)
		_adc0_scan_set.mpd[2].enabled = en;

	if(input_pin == ADC0_MPD3)
		_adc0_scan_set.mpd[3].enabled = en;

	return;
}

#ifdef SUPPORT_TEC_FUNC

uint16_t ADC0_get_itec()
{
	return _adc0_scan_set.itec.val;
}

uint16_t ADC0_get_vtec()
{
#if defined(HW_VER_AM1_301) || defined(HW_VER_AM2_301) || defined(HW_VER_AM3_301)
	return _adc0_scan_set.vtec.val;
#endif
	return 0;
}

uint16_t ADC0_get_tec_tempout()
{
	_adc0_scan_set.tec_tempout.val = adc0_do_measure(_adc0_scan_set.tec_tempout.pin);
	return _adc0_scan_set.tec_tempout.val;
}
#endif

SI_INTERRUPT (ADC0EOC_ISR, ADC0EOC_IRQn)
{
	adc0_clearIntFlags_by_isr(ADC0_CONVERSION_COMPLETE_IF);
}

