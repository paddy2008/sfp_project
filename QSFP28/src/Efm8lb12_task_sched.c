#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_timer.h"
#include "Efm8lb12_msa_pin.h"
#include "Efm8lb12_i2c_slave.h"
#include "Efm8lb12_task_sched.h"


static volatile TASK_FAST_SCHED_TAB gTaskFastSchedTab;
static volatile FUNC gTaskInstant[TASK_NUM];


/*
 * because the keil C51 toolchains reason, there must be init task table as follow way
 */
static void _task_table_init_default()
{
	gTaskInstant[0]   = MSA_pin_lpmode_on_poll;  	    //LOWER power poll,support fast task

	gTaskInstant[1]   = com_drv_on_poll;  		 		//ADC poll

	gTaskInstant[2]   = MSA_page_lower_on_poll;  		//DDMI poll

	gTaskInstant[3]   = MSA_page_lower_set_intr_flag;   //INTR flag poll, support fast task

	gTaskInstant[4]   = MSA_p67_on_poll;			    //APC poll

	gTaskInstant[5]   = MSA_control_byte_poll;			//MSA control byte poll,support fast task

	gTaskInstant[6]   = MSA_write_eep_poll;				//WRITE eep poll,support fast task

	gTaskInstant[7]   = TIMER_soft_on_poll;				//TIMER task poll

	gTaskInstant[8]   = MSA_p4_on_poll;					//PAGE4 poll

	gTaskInstant[9]   = MSA_p4_debug_cmd_on_poll;		//DEBUG cmd poll, support fast task

	gTaskInstant[10]  = MSA_p9_on_poll;					//Aging poll
}

void TASK_fast_sched(uint8_t id)
{
	uint8_t i = 0;

	IE_EA = 0;      			  //Disable global interrupts

	for(i = 0; i < gTaskFastSchedTab.num; i++)
	{
		if(gTaskFastSchedTab.id[i] == id)  //if current id is already exist, do nothing
		{
			IE_EA = 1;      			  //Enable global interrupts
			return;
		}
	}

	//if current id is not exist
	if((gTaskFastSchedTab.num < TASK_NUM) && (gTaskFastSchedTab.id[gTaskFastSchedTab.num] == TASK_ID_INVALID))
	{
		//here there is not juduge id and gTaskInstant[id] validity,so they are judged in using
		gTaskFastSchedTab.id[gTaskFastSchedTab.num] = id;
		gTaskFastSchedTab.num ++;
	}

	IE_EA = 1;      			  //Enable global interrupts
}

static void _task_init_fast_sched_tab()
{
	uint8_t i = 0;

	for(i = 0; i < TASK_NUM; i++)
	{
		gTaskFastSchedTab.id[i] = TASK_ID_INVALID;
	}

	gTaskFastSchedTab.num = 0;
}

void TASK_table_init()
{
	_task_table_init_default();
	_task_init_fast_sched_tab();
}


void TASK_sched()
{
	uint8_t i = 0, j = 0, curTaskId = 0;

	for(i = 0; i < TASK_NUM; i++)
	{
		for(j = 0; ((j < gTaskFastSchedTab.num) && (I2C_slave_get_status() == false)); j++)
		{
			curTaskId = gTaskFastSchedTab.id[j];

			if((curTaskId != TASK_ID_INVALID) && (gTaskInstant[curTaskId] != NULL))  //i is actually pri, get task id by pri,and get task pointer by id
			{
				gTaskInstant[curTaskId]();
			}

			IE_EA = 0;  //because gTaskFastSchedTab.num and gTaskFastSchedTab.id[j] are used in multi-isrs, it must disable global interrupt
			gTaskFastSchedTab.num --;
			gTaskFastSchedTab.id[j] = TASK_ID_INVALID;
			IE_EA = 1;
		}

		if(gTaskInstant[i] != NULL)  //i is actually pri, get task id by pri,and get task pointer by id
		{
			gTaskInstant[i]();
		}
	}
}


