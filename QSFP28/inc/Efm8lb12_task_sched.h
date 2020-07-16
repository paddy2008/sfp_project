#ifndef __100g_sfp_EFM8LB1_task_sched_h__
#define __100g_sfp_EFM8LB1_task_sched_h__

#include <stdint.h>

#define TASK_NUM  (11)

typedef struct
{
	uint8_t num;
	uint8_t id[TASK_NUM];  //task id, after init completed, task and it's id is one to one
}TASK_FAST_SCHED_TAB;

typedef enum
{
	TASK_ID_LOWER_POWER_ON_POLL		= 0x0,
	TASK_ID_SET_INTL_FLAG_ON_POLL	= 0x1,
	TASK_ID_DRV_ON_POLL		   		= 0x2,
	TASK_ID_LOWER_PAGE_ON_POLL		= 0x3,
	TASK_ID_PAGE67_ON_POLL    		= 0x4,
	TASK_ID_CONTROL_BYTE_ON_POLL	= 0x5,
	TASK_ID_WRITE_EEP_ON_POLL		= 0x6,
	TASK_ID_SOFT_TIMER_ON_POLL      = 0x7,
	TASK_ID_PAGE4_ON_POLL	   		= 0x8,
	TASK_ID_DEBUG_CMD_POLL			= 0x9,
	TASK_ID_PAGE8_ON_POLL     		= 0xA,
	TASK_ID_INVALID					= 0xB,
}TASK_ID;

void TASK_sched(void);
void TASK_table_init(void);
void TASK_fast_sched(uint8_t id);


#endif
