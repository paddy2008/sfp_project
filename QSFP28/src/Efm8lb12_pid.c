#include <stdio.h>
#include <string.h>
#include "adc_0.h"
#include "Efm8lb12_pid.h"
#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_100g_com_drv.h"
#include "Efm8lb12_adc.h"
#include "version.h"
#include "Efm8lb12_eep_interface.h"

#ifdef SUPPORT_TEC_FUNC

#define TEC_TARGET_DIV	(50)


volatile uint8_t gPriEn = 0;
volatile PID_ST gPidData;
uint16_t dac_verf_mv = 0;

static double pidCalculateFunc(void)
{
	int16_t  delta, delta2;
	uint16_t p1mv;  // in 0.1mv	
	uint16_t ret;

	delta = gPidData.targetPoint - gPidData.tempOutPoint;	// deviation   -20000   ----  +20000
	if(abs(delta)>10000)  //+-10000
	{
		if(gPidData.targetPoint > gPidData.tempOutPoint)
			delta=10000;
		else
			delta=-10000;
	}
	gPidData.sumDelta += delta;						// integral
	if(gPidData.sumDelta>500000)
		gPidData.sumDelta=500000;
	if(gPidData.sumDelta<-500000)
		gPidData.sumDelta=-500000;
	delta2 = gPidData.lastDelta - gPidData.prevDelta;		// differential

	gPidData.prevDelta = gPidData.lastDelta;
	gPidData.lastDelta = delta;

	p1mv = TEC_CONTROL_BALANCE_VAL +
		   (uint16_t)(gPidData.proportion * delta   //+-4000   8500-16500
		     + gPidData.integral * gPidData.sumDelta //3500 - 21500
		     + gPidData.derivative * delta2);

	if(p1mv > TEC_CONTROL_LIMITER_MAX)
		p1mv = TEC_CONTROL_LIMITER_MAX;
	else if(p1mv < TEC_CONTROL_LIMITER_MIN)
		p1mv = TEC_CONTROL_LIMITER_MIN;



	ret = (uint16_t)((float)p1mv/dac_verf_mv * DAC_MAX_RESULT) & DAC_MAX_RESULT;

	return ret;
}

void PID_init(void)
{
    uint8_t  tmp[2] = {0};

	memset(&gPidData, 0, sizeof(gPidData));
	gPidData.proportion = 0.3; //0.076;	//0.5 //2 //0.1 //1 //0.7;		// Set PID Coefficients
	gPidData.integral = 0.01; //0.01;		//0.5
	gPidData.derivative = 0.1; //0.2;	//0.01	//0.05	//0.5	//0.9	//0.75	//*0.1	//*0.15

	tmp[0] = FLASH_ByteRead(MSA_FLASH_ADDR_PAGE_07H + OFFSET_INDEX(0xfe));//SFP_PID_TARGET_MSB_FLASH_ADDR
	tmp[1] = FLASH_ByteRead(MSA_FLASH_ADDR_PAGE_07H + OFFSET_INDEX(0xff));//SFP_PID_TARGET_LSB_FLASH_ADDR
    gPidData.targetPoint = (tmp[0] << 8) | tmp[1];

    dac_verf_mv = ADC0_get_vcc();
	if(gPidData.targetPoint > 0 && gPidData.targetPoint < 20000)
	{
		TEC_EN = 1;
	}
	else
	{
		TEC_EN = 0;
	}
}

uint8_t PID_cal(void)
{
    static uint8_t diffNum = 0;
    uint16_t val_max,val_min;
    uint8_t  tmp[2] = {0};

    if (!(TX_DIS == 0) && (TEC_EN == 1))
    {
    	return 1;
    }

	tmp[0] = FLASH_ByteRead(MSA_FLASH_ADDR_PAGE_07H + OFFSET_INDEX(MSA_P7_TEC_PID_TARGET));//SFP_PID_TARGET_MSB_FLASH_ADDR
	tmp[1] = FLASH_ByteRead(MSA_FLASH_ADDR_PAGE_07H + OFFSET_INDEX(MSA_P7_TEC_PID_TARGET+1));//SFP_PID_TARGET_LSB_FLASH_ADDR
    gPidData.targetPoint = (tmp[0] << 8) | tmp[1];
    dac_verf_mv = ADC0_get_vcc();
    val_max=(uint16_t)((float)TEC_CONTROL_LIMITER_MAX/dac_verf_mv * DAC_MAX_RESULT) & DAC_MAX_RESULT;
    val_min=(uint16_t)((float)TEC_CONTROL_LIMITER_MIN/dac_verf_mv * DAC_MAX_RESULT) & DAC_MAX_RESULT;
	do{
		gPidData.tempOutPoint = ADC0_get_tec_tempout();
        if(abs(gPidData.targetPoint - gPidData.tempOutPoint) < TEC_TARGET_DIV)
			return 0;
	    else
	    {
	    	if(abs(gPidData.targetPoint - gPidData.tempOutPoint) > 6000)
			{
				diffNum++;
				if(diffNum > 20)
				{
					PID_init();
					diffNum = 0;
				}
			}
			else
			{
				diffNum = 0;
			}
	    	gPidData.tempSetPoint = pidCalculateFunc();
	    }

#ifdef HW_VER_AM1_301
		DAC1_setOutput(gPidData.tempSetPoint);
#endif
#if defined(HW_VER_AM2_301) || defined(HW_VER_AM3_301) || defined(HW_VER_AM4_301)
		DAC3_setOutput(gPidData.tempSetPoint);
#endif
	} while(0);

    return 1;
}

uint16_t PID_get_tempset(void)
{
	return gPidData.tempSetPoint;
}

uint16_t PID_get_tempout(void)
{
	return gPidData.tempOutPoint;
}

#endif

