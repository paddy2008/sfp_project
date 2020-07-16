#ifndef __100g_sfp_EFM8LB1_adc_h__
#define __100g_sfp_EFM8LB1_adc_h__

#include <SI_EFM8LB1_Register_Enums.h>
#include "adc_0.h"

// Number of samples to take per channel,max value is 64
#define NUM_SCANS 				 (8)

//ADC 的参考电压, in 0.1mV
#define ADC_VREF_MV         	 (16500UL)

//读取MCU上的温度传感器
#define ON_CHIP_TEMP_SENSOR      (ADC0_POSITIVE_INPUT_TEMP)

#define TEMP_SENSOR_NUM_CHANNES  (1)

// 12 bit ADC
#define ADC_MAX_RESULT  		 ((1 << 14)-1)

// SYSCLK frequency in Hz
#define ADC0_SYSCLK   			 (49000000/2)

// Set SAR clock to 18MHz
#define ADC0SARCLK    			 (18000000)

// Address in flash where the temp cal low byte value is stored
#define TEMP_CAL_ADDRESS_LOW     (0xFFD4)

// Address in flash where the temp cal high byte value is stored
#define TEMP_CAL_ADDRESS_HIGH    (0xFFD5)

//auto scan buffer start addr at xdata
#define BUFFER_SCAN_TMP_ADDR 	 (0x0F20)

void		ADC0_Init(void);
uint16_t	ADC0_DealSingleAnalogInputData(uint16_t *buff);
uint16_t	ADC0_ConvertSampleToMillivolts(uint16_t sample);
float		ADC0_GetTempSensor();
// uint16_t	ADC0_do_measure(ADC0_PositiveInput_t input_start, uint8_t input_num, uint16_t *result);

void 		ADC0_poll();
int16_t 	ADC0_get_mcu_temp();
uint16_t 	ADC0_get_vcc();
uint16_t	ADC0_get_rssi(uint8_t ch);
uint16_t	ADC0_get_mpd(uint8_t ch);
uint16_t	ADC0_manual_poll(ADC0_PositiveInput_t input_pin);
void		ADC0_enable_input(ADC0_PositiveInput_t input_pin, uint8_t en);

#ifdef SUPPORT_TEC_FUNC
uint16_t	ADC0_get_itec();
uint16_t	ADC0_get_vtec();
uint16_t	ADC0_get_tec_tempout();
#endif

#endif
