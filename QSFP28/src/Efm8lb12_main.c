#include "wdt_0.h"
#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_smbus.h"
#include "Efm8lb12_adc.h"
#include "Efm8lb12_port.h"
#include "Efm8lb12_pid.h"
#include "Efm8lb12_main.h"
#include "Efm8lb12_msa_pin.h"
#include "Efm8lb12_task_sched.h"
#include "Efm8lb12_i2c_slave.h"
#include "Efm8lb12_100g_com_drv.h"


static xdata uint16_t	_ts = 1;

uint16_t MSA_get_poll_timestamp()
{
	return _ts;
}

static void mcu_sysclk_init (void)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	// $[HFOSC1 Setup]
	// Ensure SYSCLK is > 24 MHz before switching to HFOSC1

	//if SYSCLK > 50MHZ,must set PFE0CN register
	SFRPAGE = 0x10;
	PFE0CN = PFE0CN_FLRT__SYSCLK_BELOW_75_MHZ ;

	SFRPAGE = 0x00;
	CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
	CLKSEL = CLKSEL_CLKSL__HFOSC0 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
	while (CLKSEL & CLKSEL_DIVRDY__BMASK == CLKSEL_DIVRDY__NOT_READY);

	CLKSEL = CLKSEL_CLKSL__HFOSC1 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
	CLKSEL = CLKSEL_CLKSL__HFOSC1 | CLKSEL_CLKDIV__SYSCLK_DIV_1;
	while (CLKSEL & CLKSEL_DIVRDY__BMASK == CLKSEL_DIVRDY__NOT_READY);
	SFRPAGE  = SFRPAGE_SAVE;
}

static void mcu_peripheral_init()
{
	IE_EA = 0;
	WDT_init(7);
	mcu_sysclk_init();

	PORT_crossbar_xbr2();
	PORT_INIT();
	PORT_crossbar_xbr0();
	PORT_crossbar_xbr1();

	TIMER_init();
	SMB_init();
	I2C_slave_init();
	ADC0_Init();

#if defined(SUPPORT_DAC1_FUNC)
	DAC1_init(DAC1_UPDATE_TRIGGER_SYSCLK);
#endif

#if defined(SUPPORT_DAC3_FUNC)
	DAC3_init(DAC3_UPDATE_TRIGGER_SYSCLK);
#endif

#if defined(SUPPORT_EXT_INT0_FUNC)
	PORT_external_int0_init();
#endif

#if defined(SUPPORT_EXT_INT1_FUNC)
	PORT_external_int1_init();
#endif

	PORT_match_init();

	PORT_disable_all_pin_intrrupt();  //Disable all pin interrupts

	MSA_pin_check_reset_pin();   	 //reset detection

	IE_EA = 1;      			  //Enable global interrupts
}


void main (void)
{
	uint8_t i = 0;

 	mcu_peripheral_init();

 	init_mempool(0x0000,4096);

	MSA_init();  //after msa_init completed, set mod status not ready, and INTL = 1, lower page byte2[0] = 1,indicate data not ready; byte2[1] = 1, update INTL pin status

	TASK_table_init();

	MSA_p67_resume_normal_mode(); //drv chip init and MSA control byte config

	MSA_pin_boot_detection();     //this function must be MSA control byte after, because extern pin pri more than MSA control byte pri

	com_drv_on_poll();

#if defined(SUPPORT_PID_FUNC)
	PID_init();  //because this API call ADC value, there must be com_drv_on_poll init after
#endif

	MSA_page_lower_on_poll();     //update ddmi

	MSA_page_lower_set_boot_status(true);  //set module status ready,indacate data is ready

	PORT_enable_all_pin_intrrupt();  //enable reset, lpmode, modsel interrupt

	WDT0_start();

	while(1)
	{
		WDT0_feed();

		TASK_sched();

		_ts += 1;
	}
}
