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

SI_SEGMENT_VARIABLE( ddm_Lower,       extern struct LOWER_MEMORY_MAP_struct,  SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( I2c_Buffer,      extern struct I2C_BUFFER_Struct,	      SI_SEG_XDATA );
SI_SEGMENT_VARIABLE( Module_Status_s, extern struct Module_Status_struct,     SI_SEG_DATA  );

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


#define  TASK_NUM      11

typedef  void ( *FUNC )( void);

FUNC Task[ TASK_NUM ] =
{
	UpdateModuleStatus,                           // ����ģ��״̬��Ϣ

	TOSA_LDChip_ON_OFF,                           // TOSA/LDоƬ�򿪹ر�

	GetDOM_Temperature,                           // ����ģ���¶�

    GetDOM_Voltage,                               // ����������ѹ

	GetDOM_LDBias,                                // ����ƫ�õ���

	GetDOM_TxPower,                               // ��������⹦��

	GetDOM_RxPower,                               // �������չ⹦��

	TxPowerAdjust,                                // ����⹦�ʵ���

	ModCurrentAdjust,                             // ���Ƶ�������

	SetDeviceCtrlData,                            // ����оƬ��������

	UpdateAlarmAndWarningBits                     // ���¸澯�ֽ�����
};
//-----------------------------------------------------------------------------
// Main Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, then waits for I2C
// communication.
//
//-----------------------------------------------------------------------------
int main (void)
{
	int i;

	// Call hardware initialization routine
	Port_Init();

	mcu_sysclk_init();

	// WDT0_init(200);                     // ��ʼ������200ms���Ź���ʱ

	I2C_Slave_Init();

	TransferDataToRam();

	I2C_Master_Init();

	InitPrivateData();

	ADC0_Init();

	InitDeviceData();

	TIMER2_Init();

	TIMER4_Init();

    IE_EA = 1;

	while (1)
	{
		WDT0_feed();                    //

	   	ProcessI2cBuffersData();        //

	   	UpdateDOMData();	            //

		if ( Module_Status_s.Flush_DOMData == 1 )
		{
			Module_Status_s.Flush_DOMData = 0;

			// for ( i = 0; (i < TASK_NUM) && (Module_Status_s.Flush_DOMData == 0) && (I2c_Buffer.Status == I2C_BUFFER_IDLE); i++ )
			{
				UpdateModuleStatus();

				TOSA_LDChip_ON_OFF();

				GetDOM_Temperature();

			    GetDOM_Voltage();

				GetDOM_LDBias();

				GetDOM_TxPower();

				GetDOM_RxPower();

				TxPowerAdjust();

				ModCurrentAdjust();

				// SetDeviceCtrlData();

				UpdateAlarmAndWarningBits();

				//Task[i]();  // ִ��DOMУ���������������������߼��źż�صȲ���
			}

			if( I2c_Buffer.Status == I2C_BUFFER_WAITING_TO_PROCESS )
			{
				Module_Status_s.Flush_DOMData = 1;
			}

			if ( ( ddm_Lower.G_CtrlStatus_Bits.CtrlStatus_110.Bits.Data_Ready_Bar == 1 ) && ( (i >= TASK_NUM - 1) ) )
			{
				ddm_Lower.G_CtrlStatus_Bits.CtrlStatus_110.Bits.Data_Ready_Bar = 0;
			}
			else
			{
				ddm_Lower.G_CtrlStatus_Bits.CtrlStatus_110.Bits.Data_Ready_Bar = 1;
			}
		}

	} // Spin forever
}
