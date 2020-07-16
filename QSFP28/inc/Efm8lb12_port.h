#ifndef __100g_sfp_EFM8LB1_port_h_
#define __100g_sfp_EFM8LB1_port_h_

#include <SI_EFM8LB1_Register_Enums.h>

typedef enum {
	PORT_INTR_NONE,
	PORT_INTR_LPMODE		= 0x01,
	PORT_INTR_RESET			= 0x02,
	PORT_INTR_MODSEL		= 0x04,
	PORT_INTR_TX_FAULT		= 0x08,	// tx fault or Equalization Fault
//	PORT_INTR_TX_EQF		= 0x10,
	PORT_INTR_TX_LOS		= 0x10,
	PORT_INTR_TX_LOL		= 0x20,
	PORT_INTR_RX_LOS		= 0x40,
	PORT_INTR_RX_LOL		= 0x80,
	PORT_INTR_ALL_MATCH		= 0xFF,
}PORT_INTR_TYPE;


typedef enum {
	TX_FAULT_INDEX		= 0x00,	// tx fault or Equalization Fault
	TX_LOS_INDEX		= 0x01,
	TX_LOL_INDEX		= 0x02,
	RX_LOS_INDEX		= 0x03,
	RX_LOL_INDEX		= 0x04,
	INVALID_INDEX		= 0x05,
}INTR_TYPE_INDEX;

typedef enum
{
	PORT_INTR_DISABLED	 = 0x0,
	PORT_INTR_EANBLED  	 = 0x1,
}PORT_INTR_SWITCH;

typedef enum {
	PORT_EXT_INTR0,
	PORT_EXT_INTR1,
}PORT_EXT_INTR_TYPE;

typedef struct {
	PORT_INTR_TYPE	intr;
	uint8_t			port;
	uint8_t			pin;
	uint8_t			poll;	// aggregate interrupt need polling to determine INTR source
}PORT_INTR_MAP_T;

void 	PORT_match_init(void);
void 	PORT_crossbar_xbr0(void);
void 	PORT_crossbar_xbr1(void);
void 	PORT_crossbar_xbr2(void);
void 	PORT_external_int0_init(void);
void 	PORT_external_int1_init(void);
void    PORT_disable_all_pin_intrrupt(void);
void 	PORT_enable_all_pin_intrrupt(void);
void	PORT_set_intr_en(PORT_INTR_TYPE intr, uint8_t enable);
void 	PORT_isr_set_intr_en(PORT_INTR_TYPE intr, uint8_t enable);

uint8_t	PORT_get_intr_status(PORT_INTR_TYPE intr, uint8_t *status);
uint8_t PORT_isr_get_intr_status(PORT_INTR_TYPE intr, uint8_t *status);

#endif
