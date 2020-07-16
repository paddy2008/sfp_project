#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_i2c_slave.h"
#include "Efm8lb12_port.h"
#include "Efm8lb12_task_sched.h"
#include "Efm8lb12_100g_com_drv.h"

static PORT_INTR_MAP_T _isr_map[] = {

#ifdef EFM8LB12E64_100G_SFP_AOC_101
#ifdef HW_VER_AM3_101
	{ PORT_INTR_RX_LOS | PORT_INTR_RX_LOL, 2, 3, true },
	{ PORT_INTR_TX_FAULT | PORT_INTR_TX_LOS | PORT_INTR_TX_LOL, 1, 5, true},
#endif
  #ifdef HW_VER_AM4_101
	{ PORT_INTR_MODSEL, 1, 0, false },
	{ PORT_INTR_RX_LOS | PORT_INTR_RX_LOL, 0, 6, true },
	{ PORT_INTR_TX_FAULT | PORT_INTR_TX_LOS | PORT_INTR_TX_LOL, 0, 7, true},
  #endif
#endif
#ifdef EFM8LB12E64_100G_SFP_LR4_301
	{ PORT_INTR_MODSEL, 0, 0, false },
#ifdef HW_VER_AM4_301
	{ PORT_INTR_TX_LOS | PORT_INTR_TX_LOL, 0, 7, true },
#endif
#endif
#ifdef EFM8LB12E64_100G_SFP_CWDM4_202
	{ PORT_INTR_TX_FAULT, 0, 3, true },
	{ PORT_INTR_RX_LOS | PORT_INTR_RX_LOL, 0, 6, true },
	{ PORT_INTR_TX_LOS | PORT_INTR_TX_LOL, 0, 7, true },
	{ PORT_INTR_MODSEL, 1, 0, true },
#endif
#ifdef EFM8LB12E64_100G_SFP_CWDM4_206
	{ PORT_INTR_TX_FAULT, 0, 3, true },
	{ PORT_INTR_RX_LOS | PORT_INTR_RX_LOL, 0, 6, true },
	{ PORT_INTR_TX_LOS | PORT_INTR_TX_LOL, 0, 7, true },
	{ PORT_INTR_MODSEL, 1, 0, true },
#endif
#ifdef EFM8LB12E64_100G_SFP_CWDM4_207
	{ PORT_INTR_MODSEL, 0, 4, true },
	{ PORT_INTR_TX_LOL, 0, 6, true },
#endif
#ifdef EFM8LB12E64_100G_SFP_CWDM4_211
#ifdef HW_VER_SEMTEC_AM1_211
	{ PORT_INTR_TX_FAULT, 0, 3, true },
	{ PORT_INTR_RX_LOS | PORT_INTR_RX_LOL, 0, 6, true },
	{ PORT_INTR_TX_LOS | PORT_INTR_TX_LOL, 0, 7, true },
	{ PORT_INTR_MODSEL, 1, 0, true },
#endif
#ifdef HW_VER_MAXIM_AM1_211
	{ PORT_INTR_RX_LOS | PORT_INTR_RX_LOL, 0, 6, true },
	{ PORT_INTR_TX_LOS | PORT_INTR_TX_LOL | PORT_INTR_TX_FAULT, 0, 7, true },
	{ PORT_INTR_MODSEL, 1, 0, true },
#endif

#ifdef HW_VER_MAXIM_AM2_211
	{ PORT_INTR_RX_LOS | PORT_INTR_RX_LOL, 0, 6, true },
	{ PORT_INTR_TX_LOS | PORT_INTR_TX_LOL, 0, 7, true },
	{ PORT_INTR_MODSEL, 0, 3, true },
#endif

#ifdef HW_VER_MAXIM_AM3_211
	{ PORT_INTR_RX_LOS | PORT_INTR_RX_LOL, 0, 6, true },
	{ PORT_INTR_TX_LOS | PORT_INTR_TX_LOL, 0, 7, true },
	{ PORT_INTR_MODSEL, 0, 3, true },
#endif

#endif
};

/* 
 * bit mask for each channel 
 * 1 is masked/disabled
 */

static uint8_t port_status_get(uint8_t port)
{
	if(port == 0)
		return P0;
	else if(port == 1)
		return P1;
	else if(port == 2)
		return P2;
	else if(port == 3)
		return P3;
	
	return 0;
}

static uint8_t port_match_get(uint8_t port)
{
	uint8_t r = 0, SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE = PG3_PAGE;

	if(port == 0)
		r = P0MAT;
	else if(port == 1)
		r = P1MAT;
	else if(port == 2)
		r = P2MAT;
	
	SFRPAGE = SFRPAGE_SAVE;
	return r;
}

void PORT_crossbar_xbr0(void)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;

	XBR0 = XBR0_URT0E__DISABLED 	//disable UART0 I/O

	| XBR0_SPI0E__DISABLED  	//disable SPI0 I/O
	| XBR0_SMB0E__ENABLED | XBR0_CP0E__DISABLED   			//enable SMB0 I/O
	| XBR0_CP0AE__DISABLED | XBR0_CP1E__DISABLED
	| XBR0_CP1AE__DISABLED | XBR0_SYSCKE__DISABLED;

	SFRPAGE = SFRPAGE_SAVE;
}

void PORT_crossbar_xbr2(void)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;

	XBR2 = XBR2_URT1E__DISABLED    			//disable UART1 I/O
		| XBR2_URT1RTSE__DISABLED  			//disable UART1 RTS Output
		| XBR2_URT1CTSE__DISABLED  			//disable UART1 CTS Input
		| XBR2_XBARE__ENABLED	   			//Crossbar Enable
		| XBR2_WEAKPUD__PULL_UPS_DISABLED;  //Weak Pullups disable

	SFRPAGE = SFRPAGE_SAVE;
}

void PORT_crossbar_xbr1(void)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;

	XBR1 = XBR1_PCA0ME__DISABLED  //disable PCA Module I/O
		| XBR1_ECIE__DISABLED     //disable PCA0 External Counter Input
		| XBR1_T0E__DISABLED	  //disable T0
		| XBR1_T1E__DISABLED      //disable T1
		| XBR1_T2E__DISABLED;     //disable T2

	SFRPAGE = SFRPAGE_SAVE;
}

#ifdef SUPPORT_EXT_INT0_FUNC
/*lpmode = 0£¬ TX3V3_EN = 0 ---high power mode, lpmode = 1£¬ TX3V3_EN = 1 ---low power mode */
void PORT_external_int0_init(void)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE   = 0x0;
	TCON 	 &= ~TCON_IT0__EDGE;	 //level  trigger
#if defined(EFM8LB12E64_100G_SFP_CWDM4_207)
	IT01CF 	 |= IT01CF_IN0SL__P0_5;  		//p0.5 is defined as lpmode pin for
#else
	IT01CF 	 |= IT01CF_IN0SL__P0_1;  		//p0.1 is defined as lpmode pin
#endif
	IT01CF 	 |= IT01CF_IN0PL__ACTIVE_LOW;  	//p0.1 is defined as lpmode pin
	IP       |= 0x01;   					//set int0 interrupt the highest pri
	SFRPAGE   = 0x10;
	IPH      |= 0x01;
	SFRPAGE   = SFRPAGE_SAVE;
	IE       &= ~IE_EX0__ENABLED ;
}
#endif


#ifdef SUPPORT_EXT_INT1_FUNC
/*reset hold low for 2us, down edge to reset handle*/
void PORT_external_int1_init(void)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;
	TCON 	|= TCON_IT1__EDGE;
#if defined(HW_VER_MAXIM_AM1_211)
	IT01CF	|= IT01CF_IN1PL__ACTIVE_HIGH | IT01CF_IN1SL__P0_2;		//p0.2 is defined as reset pin
#endif
#if defined(HW_VER_MAXIM_AM2_211) || defined(HW_VER_MAXIM_AM3_211)
	IT01CF	|= IT01CF_IN1PL__ACTIVE_LOW | IT01CF_IN1SL__P0_4;		//p0.4 is defined as reset pin
#endif
	IP      |= 0x04;   					//set int1 interrupt the highest pri
	SFRPAGE  = 0x10;
	IPH     |= 0x04;
	SFRPAGE  = SFRPAGE_SAVE;
	IE      &= ~IE_EX1__ENABLED ;
}
#endif


void PORT_match_init(void)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE   = 0x10;
	EIP1     &= ~(EIP1_PMAT__HIGH);  	//set port match interrupt the lowest pri
	EIP1H    &= ~(EIP1H_PHMAT__HIGH);
	EIE1     &= ~EIE1_EMAT__ENABLED;    //disable interrupt requests generated by a Port Match
	SFRPAGE   = SFRPAGE_SAVE;
}

static void port_external_interrupt_enable(PORT_EXT_INTR_TYPE intr, uint8_t enable)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE  = LEGACY_PAGE;

	if (intr == PORT_EXT_INTR0)
	{
#ifdef SUPPORT_EXT_INT0_FUNC
		if (enable)
		{
			TCON &= ~TCON_IE0__BMASK;
			IE   |= IE_EX0__ENABLED;
		}
		else
		{
			IE  &= ~IE_EX0__BMASK;
		}
#endif
	}
	else if (intr == PORT_EXT_INTR1)
	{
#ifdef SUPPORT_EXT_INT1_FUNC
		if (enable)
		{
			TCON &= ~TCON_IE1__BMASK;
			IE   |= IE_EX1__ENABLED;
		}
		else
		{
			IE  &= ~IE_EX1__BMASK;
		}
#endif
	}
	SFRPAGE = SFRPAGE_SAVE;
	return;
}

static void port_match_all_interrupt_enable(uint8_t enable)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE  = PG2_PAGE;

	if (enable)
	{
		EIE1  |= EIE1_EMAT__ENABLED;    //Enable interrupt requests generated by a Port Match
	}
	else
	{
		EIE1  &= ~EIE1_EMAT__BMASK;     //Disable interrupt requests generated by a Port Match
	}

	SFRPAGE = SFRPAGE_SAVE;
	return;
}

static void port_match_interrupt_enable(uint8_t port, uint8_t pin, uint8_t enable)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	
	SFRPAGE  = PG3_PAGE;

	if (enable)
	{
		if(port == 0)
			P0MASK |= (1 << pin); 
		else if(port == 1)
			P1MASK |= (1 << pin); 
		else if(port == 2)
			P2MASK |= (1 << pin); 
	}
	else
	{
		if(port == 0)
			P0MASK &= ~(1 << pin); 
		else if(port == 1)
			P1MASK &= ~(1 << pin); 
		else if(port == 2)
			P2MASK &= ~(1 << pin); 
	}
	SFRPAGE = SFRPAGE_SAVE;
	return;
}

static void port_match_reverse(uint8_t port, uint8_t pin)
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;
	
	SFRPAGE  = PG3_PAGE;

	if(port == 0)
	{
		if (P0MAT & (1 << pin))
			P0MAT &= ~(1 << pin);
		else
			P0MAT |= (1 << pin);
	}
	else if(port == 1)
	{
		if (P1MAT & (1 << pin))
			P1MAT &= ~(1 << pin);
		else
			P1MAT |= (1 << pin);
	}
	else if(port == 2)
	{
		if (P1MAT & (1 << pin))
			P1MAT &= ~(1 << pin);
		else
			P1MAT |= (1 << pin);
	}
	SFRPAGE = SFRPAGE_SAVE;
	return;
}

static uint16_t port_get_intr_pin(PORT_INTR_TYPE intr)
{
	uint8_t i, n;
	uint16_t ret = 0;
	n = sizeof(_isr_map)/sizeof(PORT_INTR_MAP_T);
	
	for(i = 0; i < n; i++)
	{
		if (intr & _isr_map[i].intr)
		{
			ret = _isr_map[i].port << 8 | _isr_map[i].pin;
			break;
		}
	}
	return ret;
}

/*
 * if the intr is asserted, return 1; othwise return 0
 * status takes the current value of the intr pin on return
 */
uint8_t PORT_isr_get_intr_status(PORT_INTR_TYPE intr, uint8_t *status)
{
	uint8_t i, n, p, pmat, ret = 0;
	n = sizeof(_isr_map)/sizeof(PORT_INTR_MAP_T);
	
	for(i = 0; i < n; i++)
	{
		if (intr & _isr_map[i].intr)
		{
			p = port_status_get(_isr_map[i].port) & (1 << _isr_map[i].pin);
			pmat = port_match_get(_isr_map[i].port) & (1 << _isr_map[i].pin);
			ret = p ^ pmat;
			*status = p;
			break;
		}
	}
	return ret;
}

static uint8_t _port_isr_get_intr_status(PORT_INTR_TYPE intr, uint8_t *status)
{
	uint8_t i, n, p, pmat, ret = 0;
	n = sizeof(_isr_map)/sizeof(PORT_INTR_MAP_T);

	for(i = 0; i < n; i++)
	{
		if (intr & _isr_map[i].intr)
		{
			p = port_status_get(_isr_map[i].port) & (1 << _isr_map[i].pin);
			pmat = port_match_get(_isr_map[i].port) & (1 << _isr_map[i].pin);
			ret = p ^ pmat;
			*status = p;
			break;
		}
	}
	return ret;
}

void PORT_set_intr_en(PORT_INTR_TYPE intr, uint8_t enable)
{
	if (intr == PORT_INTR_LPMODE)
		port_external_interrupt_enable(PORT_EXT_INTR0, enable);
	else if(intr == PORT_INTR_RESET)
		port_external_interrupt_enable(PORT_EXT_INTR1, enable);
	else if(intr == PORT_INTR_ALL_MATCH)
		port_match_all_interrupt_enable(enable);
	else
	{
		uint16_t port_pin = port_get_intr_pin(intr);
		port_match_interrupt_enable((port_pin >> 8) & 0xFF, port_pin & 0xFF, enable);
	}
}

void PORT_isr_set_intr_en(PORT_INTR_TYPE intr, uint8_t enable)
{
	if (intr == PORT_INTR_LPMODE)
		port_external_interrupt_enable(PORT_EXT_INTR0, enable);
	else if(intr == PORT_INTR_RESET)
		port_external_interrupt_enable(PORT_EXT_INTR1, enable);
	else if(intr == PORT_INTR_ALL_MATCH)
		port_match_all_interrupt_enable(enable);
	else
	{
		uint16_t port_pin = port_get_intr_pin(intr);
		port_match_interrupt_enable((port_pin >> 8) & 0xFF, port_pin & 0xFF, enable);
	}
}

static void _port_isr_set_intr_en(PORT_INTR_TYPE intr, uint8_t enable)
{
	if (intr == PORT_INTR_LPMODE)
		port_external_interrupt_enable(PORT_EXT_INTR0, enable);
	else if(intr == PORT_INTR_RESET)
		port_external_interrupt_enable(PORT_EXT_INTR1, enable);
	else if(intr == PORT_INTR_ALL_MATCH)
		port_match_all_interrupt_enable(enable);
	else
	{
		uint16_t port_pin = port_get_intr_pin(intr);
		port_match_interrupt_enable((port_pin >> 8) & 0xFF, port_pin & 0xFF, enable);
	}
}

void PORT_disable_all_pin_intrrupt(void)
{
	PORT_set_intr_en(PORT_INTR_LPMODE   , PORT_INTR_DISABLED);
	PORT_set_intr_en(PORT_INTR_ALL_MATCH, PORT_INTR_DISABLED);

	#if defined(EFM8LB12E64_100G_SFP_AOC_101)   || defined(EFM8LB12E64_100G_SFP_CWDM4_202) || \
	defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_LR4_301)   || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	PORT_set_intr_en(PORT_INTR_RESET    , PORT_INTR_DISABLED);
	#endif
}

void PORT_enable_all_pin_intrrupt(void)
{
	PORT_set_intr_en(PORT_INTR_LPMODE   , PORT_INTR_EANBLED);
	PORT_set_intr_en(PORT_INTR_ALL_MATCH, PORT_INTR_EANBLED);

	#if defined(EFM8LB12E64_100G_SFP_AOC_101)   || defined(EFM8LB12E64_100G_SFP_CWDM4_202) || \
	defined(EFM8LB12E64_100G_SFP_CWDM4_206) || defined(EFM8LB12E64_100G_SFP_LR4_301)   || defined(EFM8LB12E64_100G_SFP_CWDM4_211)
	PORT_set_intr_en(PORT_INTR_RESET    , PORT_INTR_EANBLED);
	#endif
}

SI_INTERRUPT (PMATCH_ISR, PMATCH_IRQn)
{
	uint8_t modsel_isr, pin_status;
	uint16_t port_pin;

	modsel_isr = _port_isr_get_intr_status(PORT_INTR_MODSEL, &pin_status);

	if(modsel_isr) // normally modsel interrupt should be disabled if console enabled
	{
		I2C_slave_enable(pin_status);
		port_pin = port_get_intr_pin(PORT_INTR_MODSEL);
		port_match_reverse((port_pin >> 8) & 0xFF, port_pin & 0xFF);
	}

	if(_port_isr_get_intr_status(PORT_INTR_TX_LOS, &pin_status))
	{
		_port_isr_set_intr_en(PORT_INTR_TX_LOS, PORT_INTR_DISABLED);
	}

	if(_port_isr_get_intr_status(PORT_INTR_TX_LOL, &pin_status))
	{
		_port_isr_set_intr_en(PORT_INTR_TX_LOL, PORT_INTR_DISABLED);
	}

	if(_port_isr_get_intr_status(PORT_INTR_TX_FAULT, &pin_status))
	{
		_port_isr_set_intr_en(PORT_INTR_TX_FAULT, PORT_INTR_DISABLED);
	}

	if(_port_isr_get_intr_status(PORT_INTR_RX_LOS, &pin_status))
	{
		_port_isr_set_intr_en(PORT_INTR_RX_LOS, PORT_INTR_DISABLED);
	}

	if(_port_isr_get_intr_status(PORT_INTR_RX_LOL, &pin_status))
	{
		_port_isr_set_intr_en(PORT_INTR_RX_LOL, PORT_INTR_DISABLED);
	}

	TASK_fast_sched(TASK_ID_SET_INTL_FLAG_ON_POLL);
}

