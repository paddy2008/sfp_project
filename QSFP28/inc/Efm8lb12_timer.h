#ifndef __100g_sfp_EFM8LB1_timer_h__
#define __100g_sfp_EFM8LB1_timer_h__

#include <SI_EFM8LB1_Register_Enums.h>

extern volatile uint32_t jiffies;

#define SMB_FREQUENCY  			(200000)  	  //Smbus clock 200khz
#define MAX_SOFT_TIMER_NUM  	(3)		  	  //the max num of soft timer
#define JIFFIES_MAX         	(0xFFFFFFFF)  //jiffies max value

typedef  void (*FUNC)(void);


typedef struct
{
   uint32_t timeout;
   uint32_t expires;
   uint8_t  enable;
   uint8_t  timeout_flag;
   FUNC     function;
}TIMER_LIST ;

typedef struct
{
	TIMER_LIST *pid_timer;
	TIMER_LIST *slow_boot_timer;
	TIMER_LIST *update_aging_duration_timer;
}SOFT_TIMER_T;


void TIMER_init(void);

void TIMER1_init(int counter);

void WDT_init(uint8_t interval);

void WDT_disable(void);

void  TIMER5_hw_delay_us(uint16_t delay_us);

void  TIMER_soft_delay_ms(uint16_t ms);

void  TIMER_soft_start(TIMER_LIST ** timer);

void  TIMER_soft_stop(TIMER_LIST ** timer);

void  TIMER_soft_on_poll(void);

float TIMER_temp_slow_boot_cal();

#endif

