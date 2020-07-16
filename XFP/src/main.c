//-----------------------------------------------------------------------------
//      Copyright (c) 2017-2018,
//-----------------------------------------------------------------------------
//
// file name  :       pin.c
//
// description:       main file
//
// Target     :       EFM8LB12F32E-A-QFN24
//
// version    :       V100.001
//					  2019/06/16
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Include.h"

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
	// Disable the watchdog here
}

//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, then waits for I2C
// communication.
//
//-----------------------------------------------------------------------------

extern volatile uint8_t poll_flag;

extern volatile uint8_t data_sync_ready;

int main (void)
{
	bool rtl9601c_ready = false;

	Port_Init();

	mcu_sysclk_init();

	InitPrivateData();

	TransferA0_DataToRam();

	I2C_Slave_Init();

	I2C_Master_Init();

	TIMER2_Init();

	TIMER4_Init();

    IE_EA = 1;

	while (1)
	{
		WDT0_feed();                    //

	    ProcessI2cBuffersData();        //

	   	if(data_sync_ready)
	   	{
	   		data_sync_ready = 0;
	   		rtl9601c_ready = true;
	   		DataSynchronization();
	   	}

	   	if(poll_flag && rtl9601c_ready)
	   	{
	   		poll_flag = 0;
	   		RealTimeDataPoll();
	   	}

	} // Spin forever
}
