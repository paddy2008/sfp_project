
#include "system.h"
#include "sfr_page.h"


void SystemReset(void)
{
	// RSTSRC SFR Page = 0x0;
	RSTSRC = RSTSRC_SWRSF__SET;
}


void mcu_sysclk_init (void)
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


















