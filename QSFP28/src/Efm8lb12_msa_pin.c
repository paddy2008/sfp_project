/*
 * Efm7lb12_100g_aoc_macom_pin.c
 *
 *  Created on: 2017-12-13
 *      Author: lipan
 */
#include "wdt_0.h"
#include "Efm8lb12_msa_pin.h"
#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_i2c_slave.h"
#include "Efm8lb12_task_sched.h"
#include "Efm8lb12_100g_com_drv.h"

static volatile POWER_MODE power_mode = {0, 0};

#ifdef HW_VER_AM3_101
static void _msa_pin_modsel_handle(uint8_t valid)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE  = 0x20;

	if(valid)  //module is not selected
	{
		//I2C0CN0 &= ~I2C0CN0_PINMD__I2C_MODE;  //set i2c slave pin to gpio mode
		I2C0CN0 &= ~I2C0CN0_I2C0EN__ENABLED;	//disable i2c slave
	}
	else	   //module is selected
	{
		I2C0CN0 |= I2C0CN0_PINMD__I2C_MODE;  //set i2c slave pin to i2c mode
		I2C0CN0 |= I2C0CN0_I2C0EN__ENABLED;	 //enable i2c slave
	}
	SFRPAGE   = SFRPAGE_SAVE;
}

static void _msa_pin_lpmode_handle(uint8_t valid)
{
	if(valid)
	{
		TX3V3_EN = 1;  //close tx3v3 in order to realize low power mode
	}
	else
	{
		TX3V3_EN = 0; //open tx3v3 in order to realize high power mode
	}
}

#endif

static void _msa_pin_tx_fault_handle(uint8_t tx_fault)
{
	static uint8_t prev_fault = 0;

	if(U8_LSB_4BITS(tx_fault))  //if exist fault
	{
		prev_fault = U8_LSB_4BITS(tx_fault);  //save prev result
		if(TX_DIS == 0x0)  //if current state is in high power, enter lower power mode, if current statue is in lower power, do nothing
		{
			MSA_pin_set_low_power_mode(0); //notice: don't call SFP_DRV_SET_LOW_POWER API enter lower power, because this api reverse lpmode pin match level status
			TX_DIS = 1;
			SDA_M = 0;
			SCL_M = 0;
		}
	}
	else if(prev_fault && (U8_LSB_4BITS(tx_fault) == 0x0))//if prev fault != 0x0 and current fault == 0x0, indicat fault are loss
	{
		prev_fault = 0;  //clear prev result
		if(TX_DIS)  //if current state is in lower power, enter high lower mode, if current statue is in high power, do nothing
		{
			MSA_pin_set_low_power_mode(1); //notice: don't call SFP_DRV_SET_LOW_POWER API enter lower power, because this api reverse lpmode pin match level status
			TX_DIS = 0;
			SDA_M = 1;
			SCL_M = 1;
		}
	}
}

static void _msa_pin_set_intl_status()
{
	 QSFP_MSA_PAGE_T 			*lower_page = MSA_get_page_ptr(MSA_PAGE_LOWER);
	 MSA_PAGE3_MASK_T 			*page3_mask = MSA_p3_get_mask();
	 MSA_PAGE_LOWER_MASK_T 		*lower_page_mask = (MSA_PAGE_LOWER_MASK_T *)&lower_page->ptr[OFFSET_INDEX(MSA_OFFSET_LOWER_LOS_MASK)];
	 MSA_PAGE_LOWER_ISR_FLAG_T  *isr_flag = (MSA_PAGE_LOWER_ISR_FLAG_T *)&(lower_page->ptr[MSA_OFFSET_LOWER_LOS]);

	if(ASSERT(lower_page) || ASSERT(page3_mask) || ASSERT(lower_page_mask) || ASSERT(isr_flag) || (!MSA_lower_page_read_ready_intl()))
	{
		return;
	}

	if((isr_flag->txrx_los & (~lower_page_mask->los))  		 	||
	   (isr_flag->tx_fault & (~(lower_page_mask->fault & 0x0f)))||
	   (isr_flag->txrx_lol & (~lower_page_mask->lol)) 			||
	   (isr_flag->temp 	   & (~(lower_page_mask->temp & 0xf0))) ||
	   (isr_flag->vcc      & (~(lower_page_mask->vcc & 0xf0)))  ||
	   (isr_flag->rx_power & (~page3_mask->rxpower))            ||
	   (isr_flag->tx_bias  & (~page3_mask->txbias))             ||
	   (isr_flag->tx_power & (~page3_mask->txpower)))
	{
		INTL = 0;
		lower_page->ptr[MSA_OFFSET_LOWER_STATUS] &= MSA_PAGE_LOWER_INTL_CLEAR;
	}
	else
	{
		INTL = 1;
		lower_page->ptr[MSA_OFFSET_LOWER_STATUS] |= MSA_PAGE_LOWER_INTL_SET;
	}

	_msa_pin_tx_fault_handle(isr_flag->tx_fault);
}

void MSA_pin_set_low_power_mode(uint8_t enable)
{
	if(enable)  //if enable == 1, indicate high power
	{
		power_mode.high = 1;
	}
	else  //if enable == 0, indicate low power
	{
		power_mode.low = 1;
	}

}

void MSA_pin_lpmode_on_poll(void)
{
	if(power_mode.high)  //if power_mode.high = 1, indicate high power mode
	{
		uint8_t byte241 = FLASH_ByteRead(MSA_FLASH_ADDR_PAGE_03H + OFFSET_INDEX(MSA_OFFSET_PAGE_03H_RX_OUTPUT_DISABLE));
		SFP_DRV_SET_RX_OUTPUT_DISABLE(byte241);
		power_mode.high = 0;
	}

	if(power_mode.low)  //if power_mode.low = 1, indicate low power mode
	{
		power_mode.low = 0;
		SFP_DRV_SET_RX_OUTPUT_DISABLE(0xFF);
	}
}

void MSA_pin_on_poll(void)
{
	#ifdef HW_VER_AM3_101
		_msa_pin_modsel_handle(MODSEL? 1 : 0);
		_msa_pin_lpmode_handle(LPMODE ? 1 : 0);
	#endif

	_msa_pin_set_intl_status();
}

void MSA_pin_check_reset_pin(void)
{
	while(!RESET)
	{
		WDT0_feed();
	}
}

void MSA_pin_boot_detection(void)
{
	I2C_slave_enable(MODSEL ? 1 : 0);		 //modsel detection

	SFP_DRV_SET_LOW_POWER(LPMODE ? 1 : 0);   //lpmode detection
}


#ifdef SUPPORT_EXT_INT0_FUNC
// LPMODE
SI_INTERRUPT (INT0_ISR, INT0_IRQn)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE  = 0x0;
	TCON    &= ~TCON_IE0__SET;  //clear extern interrupt0 flag
	SFRPAGE  = SFRPAGE_SAVE;

	SFP_DRV_SET_LOW_POWER(LPMODE ? 1 : 0);
	TASK_fast_sched(TASK_ID_LOWER_POWER_ON_POLL);
}
#endif

#ifdef SUPPORT_EXT_INT1_FUNC
// RESET
SI_INTERRUPT (INT1_ISR, INT1_IRQn)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	TX_DIS = 1;  //tx disable
	SDA_M  = 1;
	SCL_M  = 1;

	INTL = 1;    //clear warning

	SFRPAGE  = 0x0;
	MSA_pin_check_reset_pin();
	RSTSRC  |= 0x10;			//Writing a 1 to fourth bit forces a system reset
}
#endif
