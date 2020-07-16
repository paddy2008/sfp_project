//-----------------------------------------------------------------------------
//      Copyright (c) 2017-2018,
//-----------------------------------------------------------------------------
//
// file name  :       sfp_data_struct_public_def.h
//
// description:       SFP Data struct
//
// Target     :       SFP
//
// version    :       V100.001
//					  2019/06/16
//
//-----------------------------------------------------------------------------

#ifndef PRIVATE_DATA_STRUCT_DEF_H_
#define PRIVATE_DATA_STRUCT_DEF_H_

struct Module_ADC_DOM_struct
{
    struct
    {

        uint16_t Temperature;
        uint16_t Bias;   
        uint16_t Voltage;
        uint16_t TxPower;   
        uint16_t RxPower;   
        
        uint16_t TEC_Temperature;   
        uint16_t TEC_Current;

        uint16_t tempsensor_0c;
        
    } ADCValues;   
    
    
    struct
    {
        int16_t Temperature;
        uint16_t Bias;
        uint16_t Voltage;
        uint16_t TxPower;   
        uint16_t RxPower;

        int16_t  ChipTemperature;
        
        int16_t  TEC_Temperature;
        int16_t  TEC_Current;

    } DOMValues;
};


struct Module_Status_struct
{
    uint8_t Flush_DOMData;
    uint8_t LD_Driver_ON_Flag;
    uint8_t TOSA_ON_Flag;
    uint8_t TOSA_Temperature_Overrun_Flag;
    uint8_t TemperatureLevel_Flag;
    uint8_t RxPower_Dark_Flag;
    uint8_t I2c_Busy_Flag;
};


struct Module_CtrlData_struct
{
	struct
	{
		uint16_t            BiasSet;
	    uint16_t            ModSet;
	    uint16_t            ModBias;
	    uint16_t            CPSet;
	    uint16_t            EQSet;

	} TxPath;

	struct
	{
		uint16_t            RXLOSSet_1;
		uint16_t            RXLOSSet_2;

	} RxPath;
};


#endif
