#ifndef __100g_sfp_EFM8LB1_msa_pin_h__
#define __100g_sfp_EFM8LB1_msa_pin_h__

#include <SI_EFM8LB1_Register_Enums.h>

typedef enum
{
	HIGH_POWER_MODE  = 0x0,
	LOWER_POWER_MODE = 0x1,
}LPMODE_T;


typedef struct
{
	uint8_t high;
	uint8_t low;
}POWER_MODE;

void MSA_pin_lpmode_on_poll(void);
void MSA_pin_on_poll(void);
void MSA_pin_boot_detection(void);
void MSA_pin_check_reset_pin(void);
void MSA_pin_set_low_power_mode(uint8_t enable);

#endif
