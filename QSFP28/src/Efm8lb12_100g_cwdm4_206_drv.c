#include <math.h>
#include "Efm8lb12_adc.h"
#include "Efm8lb12_common.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_timer.h"
#include "Efm8lb12_smbus.h"
#include "Efm8lb12_msa_pin.h"
#include "Efm8lb12_100g_com_drv.h"

#ifdef EFM8LB12E64_100G_SFP_CWDM4_206

static uint8_t _i2c_dev_ready = 0;

static void _cwdm4_206_port0_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;
	//1.set port default value
	P0 = P0_B0__HIGH | P0_B1__HIGH | P0_B2__HIGH | P0_B3__HIGH | P0_B4__HIGH | P0_B5__HIGH | P0_B6__HIGH | P0_B7__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P0MDIN =  P0MDIN_B0__DIGITAL   					    //not used
			| P0MDIN_B1__DIGITAL 						// set p0.1 digital mode is used for qsfp low power mode
			| P0MDIN_B2__DIGITAL                        //not used
			| P0MDIN_B3__DIGITAL						//not used
			| P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL   //set p0.4 and p0.5 digital mode is used for uart0
			| P0MDIN_B6__DIGITAL 					    //p0.6 is input pin,used for rx-INTL
			| P0MDIN_B7__DIGITAL;					    //p0.7 is input pin,used for rx-los

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P0MDOUT = P0MDOUT_B0__OPEN_DRAIN    					    //not used
			| P0MDOUT_B1__OPEN_DRAIN 							//because p0.1 is input pin,so set p0.1 default value(open drain mode) is used for lpmode pin
			| P0MDOUT_B2__OPEN_DRAIN							//p0.2 is input pin  p0.2 used for reset mcu
			| P0MDOUT_B3__OPEN_DRAIN	    					//p0.3 is input pin,used for tx-fault
			| P0MDOUT_B4__OPEN_DRAIN | P0MDOUT_B5__OPEN_DRAIN   //set p0.4 push pull and p0.5 open drain is uesd for uart0
			| P0MDOUT_B6__OPEN_DRAIN							//p0.6 is input pin,used for rx-lol
			| P0MDOUT_B7__OPEN_DRAIN;						    //p0.7 is input pin,used for rx-los

	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P0SKIP = P0SKIP_B0__SKIPPED					//because P1.2 and p1.3 are used to Smbus,so P0.0 ~ P0.7 must be skipped
			| P0SKIP_B1__SKIPPED                // P0.1 used for lpmode
			| P0SKIP_B2__SKIPPED                // p0.2 used for reset mcu
			| P0SKIP_B3__SKIPPED			    // p0.3 used for tx-fault
			| P0SKIP_B4__SKIPPED | P0SKIP_B5__SKIPPED
			| P0SKIP_B6__SKIPPED 				// p0.6 used for rx-lol
			| P0SKIP_B7__SKIPPED;               // p0.7 used for rx-los

	//5. set p0MASK
	P0MASK = P0MASK_B0__IGNORED | P0MASK_B1__IGNORED | P0MASK_B2__IGNORED
			| P0MASK_B3__COMPARED | P0MASK_B4__IGNORED | P0MASK_B5__IGNORED
			| P0MASK_B6__COMPARED | P0MASK_B7__COMPARED;

	//6. set P0MAT
	P0MAT = P0MAT_B0__HIGH | P0MAT_B1__HIGH | P0MAT_B2__LOW | P0MAT_B3__HIGH
			| P0MAT_B4__HIGH | P0MAT_B5__HIGH | P0MAT_B6__HIGH | P0MAT_B7__LOW;

	SFRPAGE = SFRPAGE_SAVE;

}

static void _cwdm4_206_port1_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;
	//1.set port default value
	P1 = P1_B0__HIGH | P1_B1__LOW | P1_B2__HIGH | P1_B3__HIGH | P1_B4__LOW | P1_B5__HIGH | P1_B6__HIGH | P1_B7__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P1MDIN =  P1MDIN_B0__DIGITAL 					   //set p1.0 digital mode is used for modesel mcu
			| P1MDIN_B1__DIGITAL 					   //set p1.1 digital mode is used for TX close signal
			| P1MDIN_B2__DIGITAL | P1MDIN_B3__DIGITAL  //set p1.2 and p1.3 digital mode is used for i2c master mode

			| P1MDIN_B4__DIGITAL     			  //p1.4 txcdr reset pin
			| P1MDIN_B5__DIGITAL				  //not used
			| P1MDIN_B6__DIGITAL    			  //not used
			| P1MDIN_B7__ANALOG;                  //set p1.7 analog mode  used for 3V3MON  

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN    			  			//because the p1.0 are set analog mode£¬so this bit can be not set
			| P1MDOUT_B1__PUSH_PULL					  			//set p1.1 push pull mode is used for TX close signal
			| P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN	//because p1.2 and p1.3 are used for i2c master mode,so the two bits must be open drain

			| P1MDOUT_B4__PUSH_PULL              //p1.4 txcdr reset pin
			| P1MDOUT_B5__OPEN_DRAIN			 //not used 
			| P1MDOUT_B6__OPEN_DRAIN			 //not used
			| P1MDOUT_B7__OPEN_DRAIN;            // p1.7 	used for 3V3MON  

	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P1SKIP = P1SKIP_B0__SKIPPED 			//because p1.0 is set analog mode and p1.2 and p1.3 are used to Smbus ,so this bit must be skipped are used for mointor the 3.3V module input voltage
			| P1SKIP_B1__SKIPPED			//set p1.1 skip in order to p1.2 and p1.3 are used for i2c master mode
			| P1SKIP_B2__NOT_SKIPPED | P1SKIP_B3__NOT_SKIPPED

			//because P2.0 and p2.1 are used for i2c slave mode,so P1.0 ~ P1.7 must be skipped
			| P1SKIP_B4__SKIPPED | P1SKIP_B5__SKIPPED| P1SKIP_B6__SKIPPED | P1SKIP_B7__SKIPPED;

	//5. set p1MASK
	P1MASK = P1MASK_B0__COMPARED | P1MASK_B1__IGNORED | P1MASK_B2__IGNORED
			| P1MASK_B3__IGNORED | P1MASK_B4__IGNORED | P1MASK_B5__IGNORED
			| P1MASK_B6__IGNORED | P1MASK_B7__IGNORED;

	//6. set P1MAT
	P1MAT = P1MAT_B0__HIGH | P1MAT_B1__HIGH | P1MAT_B2__HIGH | P1MAT_B3__HIGH
			| P1MAT_B4__HIGH | P1MAT_B5__HIGH | P1MAT_B6__HIGH | P1MAT_B7__HIGH;

	SFRPAGE = SFRPAGE_SAVE;
}


static void _cwdm4_206_port2_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x20;
	//1.set port default value
	P2 = P2_B0__HIGH | P2_B1__HIGH | P2_B2__HIGH | P2_B3__HIGH | P2_B4__HIGH | P2_B5__HIGH | P2_B6__HIGH ;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P2MDIN = P2MDIN_B0__DIGITAL | P2MDIN_B1__DIGITAL	 //set p2.0 and p2.1 digital mode are used for i2c slave mode
			| P2MDIN_B2__DIGITAL   	  //not used
			| P2MDIN_B3__DIGITAL   	  //not used
			| P2MDIN_B4__DIGITAL      //not used
			| P2MDIN_B5__ANALOG       //used for RSSI0
			| P2MDIN_B6__DIGITAL ;    //not used

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).

	P2MDOUT = P2MDOUT_B0__OPEN_DRAIN | P2MDOUT_B1__OPEN_DRAIN //set p2.0 and p2.1 open drain mode are used for i2c slave mode
			| P2MDOUT_B2__OPEN_DRAIN	 //not used
			| P2MDOUT_B3__OPEN_DRAIN     //not used
			| P2MDOUT_B4__OPEN_DRAIN     //not used
			| P2MDOUT_B5__OPEN_DRAIN     //used for RSSI0
			| P2MDOUT_B6__OPEN_DRAIN;    //not used

	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P2SKIP = P2SKIP_B0__SKIPPED | P2SKIP_B1__SKIPPED   		//set p2.0 and p2.1 skipped for i2c slave mode
			| P2SKIP_B2__SKIPPED

			//because p2.5 is set analog mode ,so this bit must be skipped is used for for RSSI
			| 0x20;

	//5. set p2MASK
	P2MASK = P2MASK_B0__IGNORED | P2MASK_B1__IGNORED | P2MASK_B2__IGNORED
			| P2MASK_B3__IGNORED | P2MASK_B4__IGNORED | P2MASK_B5__IGNORED
			| P2MASK_B6__IGNORED ;

	//6. set P2MAT
	P2MAT = P2MAT_B0__HIGH | P2MAT_B1__HIGH | P2MAT_B2__HIGH | P2MAT_B3__HIGH
			| P2MAT_B4__HIGH | P2MAT_B5__HIGH | P2MAT_B6__HIGH ;

	SFRPAGE = SFRPAGE_SAVE;
}

static void _cwdm4_206_port3_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x20;
	//1.set port default value
	P3 = P3_B0__HIGH | P3_B1__LOW | P3_B2__HIGH | P3_B3__HIGH | P3_B4__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P3MDIN = P3MDIN_B0__DIGITAL    //set p3.0 digital mode is INITL
			| P3MDIN_B1__DIGITAL   //set p3.1 digital mode is V3P3_EN
			| P3MDIN_B2__DIGITAL   //not used
			| P3MDIN_B3__DIGITAL   //notused
			| P3MDIN_B4__DIGITAL;   //not used

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P3MDOUT = P3MDOUT_B0__OPEN_DRAIN   //because p3.0 is used INITL
			| P3MDOUT_B1__PUSH_PULL   //because p3.1 is used V3P3_EN
			| P3MDOUT_B2__OPEN_DRAIN   //not used
			| P3MDOUT_B3__OPEN_DRAIN   //not used 
			| P3MDOUT_B4__OPEN_DRAIN;   //not used

	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).

	//p3 do't have p3SKIP registers

	//5. set p2MASK
	//p3 do't have P3MASK registers

	//6. set P2MAT
	//p3 do't have P3MAT registers
	SFRPAGE = SFRPAGE_SAVE;
}

static bool _cwdm4_206_txcdr_check_exist()
{
	uint8_t i = 0, Data = 0;

	if(TX3V3_EN == 0x0)
	{
		MSA_p2_log("\r\ntxcdr is lower power mode");
		return false;
	}

	for(i = 0; i < 3; i++)
	{
		if(GOS_OK == SMB_read_byte(CWDM4_206_TXCDR_BASE_ADDR, 0x0, &Data))
		{
			MSA_p2_log("\r\ntxcdr smb read OK");
			return true;
		}
		else
		{
			MSA_p2_log("\r\ntxcdr smb read failed");
		}
		TIMER5_hw_delay_us(5000);
	}

	return false;
}

static bool _cwdm4_206_rxcdr_check_exist()
{
	uint8_t i = 0, Data = 0;

	if(TX3V3_EN == 0x0)
	{
		MSA_p2_log("\r\nrxcdr is lower power mode");
		return false;
	}

	for(i = 0; i < 3; i++)
	{
		if(GOS_OK == SMB_read_byte(CWDM4_206_RXCDR_BASE_ADDR, 0x0, &Data))
		{
			MSA_p2_log("\r\nrxcdr smb read OK");
			return true;
		}
		else
		{
			MSA_p2_log("\r\nrxcdr smb read failed");
		}
		TIMER5_hw_delay_us(5000);
	}

	return false;
}

static uint8_t _cwdm4_206_rxcdr_m37244_i2c_read_byte(uint8_t OffsetAddr, uint8_t *Data)
{
	if(TX3V3_EN == 0x0 || ((_i2c_dev_ready & RX_READY) == 0x0))
	{
		return GOS_FAIL;
	}
	return SMB_read_byte(CWDM4_206_RXCDR_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t _cwdm4_206_rxcdr_m37244_i2c_write_byte(uint8_t OffsetAddr,uint8_t Data)
{
	if(TX3V3_EN == 0x0 || ((_i2c_dev_ready & RX_READY) == 0x0))
	{
		return GOS_FAIL;
	}
	return SMB_write_byte(CWDM4_206_RXCDR_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t _cwdm4_206_txcdr_gn2105_i2c_read_byte(uint16_t OffsetAddr, uint8_t *Data)
{
	if(TX3V3_EN == 0x0 || ((_i2c_dev_ready & TX_READY) == 0x0))
	{
		return GOS_FAIL;
	}
	return SMB_read_byte(CWDM4_206_TXCDR_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t _cwdm4_206_txcdr_gn2105_i2c_write_byte(uint16_t OffsetAddr, uint8_t Data)
{
	if(TX3V3_EN == 0x0 || ((_i2c_dev_ready & TX_READY) == 0x0))
	{
		return GOS_FAIL;
	}
	return SMB_write_byte(CWDM4_206_TXCDR_BASE_ADDR, OffsetAddr, Data);
}

static void _cwdm4_206_txcdr_gn2105_reset()
{
	TX_RST = 0;  //reset cdr
	TIMER_soft_delay_ms(10);
	TX_RST = 1;  //restore cdr

	if(_cwdm4_206_txcdr_check_exist())
	{
		_i2c_dev_ready |= TX_READY;
	}
}

static void _cwdm4_206_txcdr_gn2105_init()
{
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x472,0x00);  //PD_ALL    power on
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x078,0x00);  //L0_PD_LANE
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x178,0x00);  //L1_PD_LANE
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x278,0x00);  //L2_PD_LANE
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x378,0x00);  //L3_PD_LANE

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x448,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x449,0x00);    //ADC_PARAM_MON_CTRL_RESET
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x473,0x00);    //PD_SENSE_REG_0
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x433,0x00);    //TXDSBL_SOFT

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x078,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x079,0x01);

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x084,0x01);  // bias enable
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x184,0x01);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x284,0x01);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x384,0x01);

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x01c,0x02);  //L*_DRV_IMON_CTRL
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x11c,0x02);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x21c,0x02);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x31c,0x02);

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x01b,0x00);  //L*L*_DRV_PD_MON_REG_0
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x11b,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x21b,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x31b,0x00);

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x082,0x00);  // bias override
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x083,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x182,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x183,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x282,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x283,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x382,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x383,0x00);

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x087,0x01);  // ch0 mod enable
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x187,0x01);  // ch1 mod enable
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x287,0x01);  // ch2 mod enable
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x387,0x01);  // ch3 mod enable

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x085,0x00);  // ch0 Imod_dac_low
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x086,0x00);  // ch0 Imod_dac_high

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x185,0x00);  // ch1 Imod_dac_low
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x186,0x00);  // ch1 Imod_dac_high

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x285,0x00);  // ch2 Imod_dac_low
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x286,0x00);  // ch2 Imod_dac_high

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x385,0x00);  // ch3 Imod_dac_low
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x386,0x00);  // ch3 Imod_dac_high

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x023,0x1f);  // ch0 Icross_dac_low
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x024,0x10);  // ch0 Icross_dac_high

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x123,0x1f);  // ch1 Icross_dac_low
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x124,0x10);  // ch1 Icross_dac_high

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x223,0x1f);  // ch2 Icross_dac_low
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x224,0x10);  // ch2 Icross_dac_high

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x323,0x1f);  // ch3 Icross_dac_low
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x324,0x10);  // ch3 Icross_dac_high

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x08a,0x01);  // ch0 hfde enable
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x18a,0x01);  // ch1 hfde enable
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x28a,0x01);  // ch2 hfde enable
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x38a,0x01);  // ch3 hfde enable

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x088,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x089,0x00);

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x188,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x189,0x00);

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x288,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x289,0x00);

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x388,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x389,0x00);

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x08d,0x01);  // ch0 lfde enable
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x18d,0x01);  // ch1 lfde enable
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x28d,0x01);  // ch2 lfde enable
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x38d,0x01);  // ch3 lfde enable

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x08b,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x08c,0x00);

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x18b,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x18c,0x00);

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x28b,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x28c,0x00);

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x38b,0x00);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x38c,0x00);
}

static void _cwdm4_206_rxcdr_m37244_init()
{
	_cwdm4_206_rxcdr_m37244_i2c_write_byte(0x03,0xf0);
	_cwdm4_206_rxcdr_m37244_i2c_write_byte(0x19,0xff);
	_cwdm4_206_rxcdr_m37244_i2c_write_byte(0x1b,0x03);
	_cwdm4_206_rxcdr_m37244_i2c_write_byte(0x23,0xf0);
	_cwdm4_206_rxcdr_m37244_i2c_write_byte(0x40,0xf0);
	_cwdm4_206_rxcdr_m37244_i2c_write_byte(0x42,0x1e);
	_cwdm4_206_rxcdr_m37244_i2c_write_byte(0x43,0x1e);
	_cwdm4_206_rxcdr_m37244_i2c_write_byte(0x44,0x1e);
	_cwdm4_206_rxcdr_m37244_i2c_write_byte(0x45,0x1e);
}

static void _cwdm4_206_rxcdr_m37244_reset()
{
	if(_cwdm4_206_rxcdr_check_exist())
	{
		_i2c_dev_ready |= RX_READY;
	}

	_cwdm4_206_rxcdr_m37244_i2c_write_byte(0x02, 0xaa);
	TIMER_soft_delay_ms(100);
	_cwdm4_206_rxcdr_m37244_i2c_write_byte(0x02, 0x00);
	_cwdm4_206_rxcdr_m37244_i2c_write_byte(0x03, 0xf0);
}

static void _cwdm4_206_get_bias_average_value(uint8_t channel, uint16_t *sink_dac, uint16_t *mod_dac)
{
	const uint16_t sink_reg[] = {0x070, 0x170, 0x270, 0x370};
	const uint16_t mon_reg[]  = {0x01b, 0x11b, 0x21b, 0x31b};
	const uint16_t mod_reg[]  = {0x06e, 0x16e, 0x26e, 0x36e};
	uint16_t buff16[AVERAGE_MAX_COUNTER] = {0};
	uint8_t i = 0, value_h = 0, value_l = 0;

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x449, 0);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(mon_reg[channel], 0);

	for(i = 0; i < AVERAGE_MAX_COUNTER; i++)
	{
		_cwdm4_206_txcdr_gn2105_i2c_read_byte(sink_reg[channel], &value_l);
		_cwdm4_206_txcdr_gn2105_i2c_read_byte(sink_reg[channel] + 1, &value_h); //bit9 8
		buff16[i] = value_l | ((value_h & 0x0F) << 8);
	}

	*sink_dac = com_average_value16(buff16, AVERAGE_MAX_COUNTER);

	for(i = 0; i < AVERAGE_MAX_COUNTER; i++)
	{
		_cwdm4_206_txcdr_gn2105_i2c_read_byte(mod_reg[channel], &value_l);
		_cwdm4_206_txcdr_gn2105_i2c_read_byte(mod_reg[channel] + 1, &value_h); //bit9 8
		buff16[i] = value_l | ((value_h & 0x0F) << 8);
	}

	*mod_dac = com_average_value16(buff16, AVERAGE_MAX_COUNTER);
}

/* Return in 0.1mA */
static float cwdm4_206_get_gn2105_bias(uint8_t channel)
{
	float result = 0.0;
	uint16_t  sink_dac = 0, mod_dac = 0;
	uint8_t   RPP = 0;

	_cwdm4_206_txcdr_gn2105_i2c_read_byte(0x466, &RPP);
	_cwdm4_206_get_bias_average_value(channel, &sink_dac, &mod_dac);
	result = ((float)sink_dac / (4095.0 * 8.774 * (1.145845 - 0.00463 * (float)RPP)) + (float)mod_dac / (2.0 * 4095.0 * 6.701 * (1.145845 - 0.00463 * (float)RPP))) * 1000.0 * 10.0;   //0.1mA
	return result;
}

static uint16_t _cwdm4_206_get_rssi_average_value()
{
	uint8_t i = 0;
	uint16_t buff16[AVERAGE_MAX_COUNTER] = {0};

	for(i = 0; i < AVERAGE_MAX_COUNTER; i++)
	{
		buff16[i] = ADC0_manual_poll(ADC0_RSSI0) * 0.1;   // uint:mv
		TIMER5_hw_delay_us(2000);
	}

	return com_average_value16(buff16, AVERAGE_MAX_COUNTER);
}

static uint16_t cwdm4_206_ddmi_get_rssi(uint8_t channel)
{
	uint8_t value = 0, i = 0;
	uint16_t rssi_mv = 0;

	channel = 3 - channel;  //rxcdr pin is inverted sequence with qsfp28 pin


	_cwdm4_206_rxcdr_m37244_i2c_read_byte(0x3, &value);
	value = SET_BIT(value,6);  //rssi enable
	if(channel == MSA_PWR_CHANNEL_1)
	{
		value = CLEAR_BIT(value,4);
		value = CLEAR_BIT(value,5);
	}
	else if(channel == MSA_PWR_CHANNEL_2)
	{
		value = SET_BIT(value,4);
		value = CLEAR_BIT(value,5);
	}
	else if(channel == MSA_PWR_CHANNEL_3)
	{
		value = CLEAR_BIT(value,4);
		value = SET_BIT(value,5);
	}
	else if(channel == MSA_PWR_CHANNEL_4)
	{
		value = SET_BIT(value,4);
		value = SET_BIT(value,5);
	}

	_cwdm4_206_rxcdr_m37244_i2c_write_byte(0x3, value);

	rssi_mv = _cwdm4_206_get_rssi_average_value();

	return (uint16_t) ((float)rssi_mv / (float)CWDM4_206_RSSI_SERIES_RES * 1000.00);
}

#if defined(HW_VER_AM1_206)
static uint16_t _cwdm4_206_get_txmon_average_value(uint8_t channel)
{
	uint8_t i = 0, value_h = 0, value_l = 0;
	uint16_t buff16[AVERAGE_MAX_COUNTER] = {0};
	const uint16_t txmon_reg[] = {0x076, 0x176, 0x276, 0x376};
	const uint16_t mod_reg[]   = {0x01b, 0x11b, 0x21b, 0x31b};

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x449, 0);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(mod_reg[channel], 0);

	for(i = 0; i < AVERAGE_MAX_COUNTER; i++)
	{
		_cwdm4_206_txcdr_gn2105_i2c_read_byte(txmon_reg[channel], &value_l);
		_cwdm4_206_txcdr_gn2105_i2c_read_byte(txmon_reg[channel] + 1, &value_h);
		buff16[i]  = value_l | ((value_h & 0x03) << 8);
	}

	return com_average_value16(buff16, AVERAGE_MAX_COUNTER);
}
#endif


#if defined(HW_VER_AM1_206)
static uint16_t cwdm4_206_ddmi_get_txmon(uint8_t channel)
{
	/*
	uint8_t  RPP = 0,value8 = 0;
	uint16_t value16 = 0;
	uint16_t imod_reg[] = {0x01c,0x11c,0x21c,0x31c};
	float coefficient[8] = {2346.0, 786.7, 394.1, 198.0, 2327.0, 781.2, 391.0, 196.9}, K = 0.0;

	_cwdm4_206_txcdr_gn2105_i2c_read_byte(0x465, &RPP);
	value16 = _cwdm4_206_get_txmon_average_value(channel);
	_cwdm4_206_txcdr_gn2105_i2c_read_byte(imod_reg[channel], &value8);
	K = coefficient[value8 & 0x07];
	return (uint16_t)(((float)value16 / (1023.0 * K * (1.145845 - 0.00463 * (float)RPP))) * 1000.0 * 10.0);   //0.1mA
	*/
	return 0;
}
#endif


#if defined(HW_VER_AM2_206)
static uint16_t cwdm4_206_ddmi_get_txmon(uint8_t channel)
{
	const uint16_t up_vcc = 25000;
	uint16_t mpd = ADC0_get_mpd(channel);

	if(up_vcc >= mpd)
	{
		return up_vcc - mpd;
	}
	else
	{
		return 0;
	}
}
#endif


static void  cwdm4_206_drv_check_laser_ch(SFP_DRV_LASER_CFG_T *cfg)
{
	if(cfg->bias > 1000)
	{
		cfg->bias = 800;
	}
	if(cfg->mod > 1000)
	{
		cfg->mod = 800;
	}
}

//***********************************************public function***************************************/

void CWDM4_206_chip_reset(void)
{
	_cwdm4_206_txcdr_gn2105_reset();
	_cwdm4_206_rxcdr_m37244_reset();
}

void CWDM4_206_port_init(void)// mcu_port_init
{
	_cwdm4_206_port0_init();
	_cwdm4_206_port1_init();
	_cwdm4_206_port2_init();
	_cwdm4_206_port3_init();
}

void CWDM4_206_chip_init(void)		// init
{
	CWDM4_206_chip_reset();
	_cwdm4_206_txcdr_gn2105_init();
	_cwdm4_206_rxcdr_m37244_init();
}

int16_t CWDM4_206_ddmi_get_temp(MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		DDMI_CALI_PARA_T *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		return ((int16_t)((float)(ddmi_cali->temp) * TIMER_temp_slow_boot_cal()) + ADC0_get_mcu_temp()); 		//MSA format uint: 1/256 C
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return  ((int16_t)ADC0_get_mcu_temp());  //MSA format uint:1/256ÉãÊÏ¶È
	}

	return GOS_FAIL;
}

uint16_t CWDM4_206_ddmi_get_vcc(MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		const DDMI_CALI_PARA_T *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		return (ddmi_cali->vcc + ADC0_get_vcc()); //uint:0.1mv
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return ADC0_get_vcc();  //uint:0.1mV
	}

	return GOS_FAIL;
}

uint16_t CWDM4_206_ddmi_get_bias(uint8_t channel, MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		uint16_t bias = 0;
		const DDMI_CALI_PARA_T *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		const MSA_LUT_T	 *laser_stat = MSA_p4_get_laser_state();

		//ddmi_cali->bias uint:0.1mA
		bias  = (uint16_t)(((ddmi_cali->current[channel].bias.bias_offset2 - ddmi_cali->current[channel].bias.bias_offset1)  + laser_stat->ch[channel].stat.txbias) * 0.1 * 1000.0 * 0.5); //uint:2uA
		return  bias;  //uint:2uA
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return (uint16_t)(cwdm4_206_get_gn2105_bias(channel));
	}
	return GOS_FAIL;
}

uint16_t CWDM4_206_ddmi_get_rx_power(uint8_t channel, MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		float rxpower_result = 0.0 ,rxpower1 = 0.0, rxpower2 = 0.0, rssi1 = 0.0, rssi2 = 0.0;
		const DDMI_CALI_PARA_T *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		const MSA_LUT_T 		*laser_stat = MSA_p4_get_laser_state();

		//ddmi_cali->tx_rx_pwr[channel].rxpower.rxpower1 uint:0.1uw
		rxpower1 = (float)ddmi_cali->current[channel].rxpower.rxpower1 * 0.1 / 1000.0;  //uint:mW
		rxpower2 = (float)ddmi_cali->current[channel].rxpower.rxpower2 * 0.1 / 1000.0;  //uint:mW

		//ddmi_cali->tx_rx_pwr[channel].rxpower.rssi1 uint:uA
		rssi1 	 = (float)ddmi_cali->current[channel].rxpower.rssi1;
		rssi2 	 = (float)ddmi_cali->current[channel].rxpower.rssi2;

		if(rssi2 != rssi1)
		{
			  rxpower_result = rxpower1 + ((float)laser_stat->ch[channel].stat.rssi - rssi1) * ((rxpower2 - rxpower1) / (rssi2 - rssi1));  //uint: mW
		}

		CHECK_POWER_MIN(rxpower_result);
		CHECK_POWER_MAX(rxpower_result);
		return (uint16_t)(rxpower_result * 10.0 * 1000.0);  //MSA format uint:0.1uW
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return  (uint16_t)cwdm4_206_ddmi_get_rssi(channel);  //uint:ua
	}

	return GOS_FAIL;
}

uint16_t CWDM4_206_ddmi_get_tx_power(uint8_t channel, MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		float txpower_result = 0.0, txpower1 = 0.0, txpower2 = 0.0, txmon1 = 0.0, txmon2 = 0.0,txmon_N = 0.0;
		const DDMI_CALI_PARA_T    *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		const MSA_LUT_T 		  *laser_stat = MSA_p4_get_laser_state();
		const SFP_DRV_LASER_CFG_T *lo_cfg = (SFP_DRV_LASER_CFG_T *)MSA_p67_get_drv_lut();
		const SFP_DRV_LASER_CFG_T *m_cfg  = (lo_cfg + MSA_CHANNEL_NUM);

		//laser_stat->ch[channel].stat.txpwr uint:0.1mA
		txmon_N = (float)laser_stat->ch[channel].stat.txpwr * 0.1;

		//ddmi_cali->tx_rx_pwr[channel].txpower.txpower uint:0.1uw
		txpower1 = (float)ddmi_cali->current[channel].txpower.txpower1 * 0.1 / 1000.0; //uint:mW
		txpower2 = (float)ddmi_cali->current[channel].txpower.txpower2 * 0.1 / 1000.0; //uint:mW

		//ddmi_cali->tx_rx_pwr[channel].txpower.bias uint:0.1mA
		txmon1 = (float)ddmi_cali->current[channel].txpower.bias1 * 0.1; //uint:mA
		txmon2 = (float)ddmi_cali->current[channel].txpower.bias2 * 0.1; //uint:mA

		if(txmon1 != txmon2)
		{
			txpower_result = txpower1 + (txpower1 - txpower2)/(txmon1 - txmon2) * (txmon_N - txmon1);  //uint:mW
		}

		CHECK_POWER_MIN(txpower_result);
		CHECK_POWER_MAX(txpower_result);
		return (uint16_t)(txpower_result * 1000.0 * 10.0); //MSA format uint:0.1uW

	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return cwdm4_206_ddmi_get_txmon(channel); //0.1mA
	}
	return GOS_FAIL;
}

void CWDM4_206_drv_adjust_laser_ch(uint8_t ch, SFP_DRV_LASER_CFG_T *cfg)
{
	uint16_t mod_hfde = 0, mod_lfde = 0;
	float k_hlf = 0.05;
	static code uint16_t bias_reg[]	    = {0x082, 0x182, 0x282, 0x382};
	static code uint16_t mod_reg[]	    = {0x085, 0x185, 0x285, 0x385};
	static code uint16_t cross_reg[]    = {0x023, 0x123, 0x223, 0x323};
	static code uint16_t mod_hfde_reg[] = {0x088, 0x188, 0x288, 0x388};
	static code uint16_t mod_lfde_reg[] = {0x08b, 0x18b, 0x28b, 0x38b};

	mod_hfde = (uint16_t)floor((2.0 *k_hlf*(0.784226 * (float)cfg->mod - 16.0)/0.4 + 0.5));
	mod_lfde = (uint16_t)floor((2.0 *k_hlf*(0.784226 * (float)cfg->mod - 16.0)/0.4 + 0.5));

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(bias_reg[ch], (uint8_t)(U16_LSB(cfg->bias)) );
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(bias_reg[ch] + 1, (uint8_t)((U16_MSB(cfg->bias)) & 0x03));
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(mod_reg[ch], (uint8_t)(U16_LSB(cfg->mod)));
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(mod_reg[ch] + 1, (uint8_t)((U16_MSB(cfg->mod)) & 0x03));
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(cross_reg[ch], (uint8_t)(U16_LSB(cfg->cross) & 0x1F));
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(cross_reg[ch] + 1, (uint8_t)((U16_MSB(cfg->cross)) & 0x1F));

	_cwdm4_206_txcdr_gn2105_i2c_write_byte(mod_hfde_reg[ch], (uint8_t)(U16_LSB(mod_hfde)));
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(mod_hfde_reg[ch] + 1, (uint8_t)((U16_MSB(mod_hfde)) & 0x03));
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(mod_lfde_reg[ch], (uint8_t)(U16_LSB(mod_lfde)));
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(mod_lfde_reg[ch] + 1, (uint8_t)((U16_MSB(mod_lfde)) & 0x03));
}

void CWDM4_206_drv_get_laser_full_state(uint8_t channel, MSA_LUT_T *lut)
{
	static code uint16_t bias_reg[]	 = {0x082, 0x182, 0x282, 0x382};
	static code uint16_t mod_reg[]	 = {0x085, 0x185, 0x285, 0x385};
	static code uint16_t cross_reg[] = {0x023, 0x123, 0x223, 0x323};

	uint8_t bias_low, mod_low, cross_low;
	uint8_t bias_high, mod_high, cross_high;

	lut->temp = DDM_GET_TEMP(MSA_DRV_VAL_HW);

	_cwdm4_206_txcdr_gn2105_i2c_read_byte(bias_reg[channel],  &bias_low);
	_cwdm4_206_txcdr_gn2105_i2c_read_byte(bias_reg[channel] + 1,  &bias_high);
	_cwdm4_206_txcdr_gn2105_i2c_read_byte(mod_reg[channel],	 &mod_low);
	_cwdm4_206_txcdr_gn2105_i2c_read_byte(mod_reg[channel] + 1,	 &mod_high);
	_cwdm4_206_txcdr_gn2105_i2c_read_byte(cross_reg[channel], &cross_low);
	_cwdm4_206_txcdr_gn2105_i2c_read_byte(cross_reg[channel] + 1, &cross_high);

	lut->ch[channel].config.bias = (uint16_t)(bias_low | (bias_high & 0x03) << 8);
	lut->ch[channel].config.mod  = (uint16_t)(mod_low | (mod_high & 0x03) << 8);
	lut->ch[channel].config.cross = (uint16_t)((cross_low & 0x1F) | (cross_high & 0x1F) << 8);
	return ;
}

void CWDM4_206_drv_ch_calc_cfg (int16_t cur_temp, uint8_t ch, const SFP_DRV_LASER_CFG_T *lut, SFP_DRV_LASER_CFG_T *ret)
{
	SFP_DRV_LASER_CFG_T *lo_cfg = (lut + ch);
	SFP_DRV_LASER_CFG_T *m_cfg	= (lo_cfg + MSA_CHANNEL_NUM);
	SFP_DRV_LASER_CFG_T *hi_cfg = (m_cfg + MSA_CHANNEL_NUM);
	int16_t temp_l = 0, temp_r = 0, temp_h = 0;
	float a1 = 0.0, a2 = 0.0, a3 = 0.0;
	APC_CALI_PARAM para;

	ret->temp = cur_temp;

	ret->bias  = 86;
	ret->mod   = 306;
	ret->cross = 4127;


	if(!MSA_p67_check_apc_lut_tab(lut))  //apc paramters is invalid, so there is no need to apc calibration
	{
		return;
	}

	if (cur_temp <= m_cfg->temp)
	{
		if((m_cfg->temp == lo_cfg->temp))
		{
			return;
		}

		ret->bias = (uint16_t )(floor(0.5 + (double)lo_cfg->bias +
						((double)(cur_temp - lo_cfg->temp)) * ((double)(m_cfg->bias - lo_cfg->bias))
						/ ((double)(m_cfg->temp - lo_cfg->temp))));
		ret->mod = (uint16_t)(floor(0.5 + (double)lo_cfg->mod +
						((double)(cur_temp - lo_cfg->temp)) * ((double)(m_cfg->mod - lo_cfg->mod))
						/ ((double)(m_cfg->temp - lo_cfg->temp))));
		ret->cross = 4127;

	}
	else
	{
		if(lo_cfg->temp == m_cfg->temp || lo_cfg->temp == hi_cfg->temp || m_cfg->temp == hi_cfg->temp)
		{
			return;
		}

		//MSA_p2_log("\r\n bias_l = %hd,bias_r = %hd,bias_h = %hd",lo_cfg->bias,m_cfg->bias,hi_cfg->bias);

		//MSA_p2_log("\r\n tl = %hd, tr = %hd, th = %hd",lo_cfg->temp,m_cfg->temp,hi_cfg->temp);

		temp_l = lo_cfg->temp / TEMP_SCALE;
		temp_r = m_cfg->temp  / TEMP_SCALE;
		temp_h = hi_cfg->temp / TEMP_SCALE;

		a1 = (double)lo_cfg->bias /((double)(temp_l - temp_r) * (double)(temp_l - temp_h));
		a2 = (double)m_cfg->bias  /((double)(temp_r - temp_l) * (double)(temp_r - temp_h));
		a3 = (double)hi_cfg->bias /((double)(temp_h - temp_l) * (double)(temp_h - temp_r));

		//MSA_p2_log("\r\n a1= %f,a2 = %f, a3= %f",a1,a2,a3);

		para.a = a1 + a2 + a3;
		para.b = -a1 * (double)(temp_r + temp_h) - a2 * (double)(temp_l + temp_h) - a3 * (double)(temp_l + temp_r);
		para.c = a1 * (double)(temp_r) * (double)(temp_h) + a2 * (double)(temp_l) * (double)(temp_h) + a3 * (double)(temp_l) * (double)(temp_r);
		ret->bias = (uint16_t)(floor(0.5 + (para.a * pow((float)(cur_temp/TEMP_SCALE), 2.0) + para.b * (float)(cur_temp/TEMP_SCALE) + para.c)));

		//MSA_p2_log("\r\n a= %f,b = %f, c= %f, bias = %hd",para.a, para.b, para.c, ret->bias);

		a1 = (double)lo_cfg->mod /((double)(temp_l - temp_r) * (double)(temp_l - temp_h));
		a2 = (double)m_cfg->mod  /((double)(temp_r - temp_l) * (double)(temp_r - temp_h));
		a3 = (double)hi_cfg->mod /((double)(temp_h - temp_l) * (double)(temp_h - temp_r));

		//MSA_p2_log("\r\n a1= %f,a2 = %f, a3= %f",a1,a2,a3);

		para.a = a1 + a2 + a3;
		para.b = -a1 * (double)(temp_r+ temp_h) - a2 * (double)(temp_l + temp_h) - a3 * (double)(temp_l + temp_r);
		para.c =  a1 * (double)(temp_r) * (double)(temp_h) + a2 * (double)(temp_l) * (double)(temp_h) + a3 * (double)(temp_l) * (double)(temp_r);
		ret->mod = (uint16_t)(floor(0.5 + (para.a * pow((float)(cur_temp/TEMP_SCALE), 2.0) + para.b * (float)(cur_temp/TEMP_SCALE) + para.c)));
		ret->cross = 4127;

		//MSA_p2_log("\r\n bias= %hd,mod = %hd",ret->bias, ret->mod);

	}

	cwdm4_206_drv_check_laser_ch(ret);
	return ;
}

void CWDM4_206_drv_poll(void)
{
	ADC0_poll();
	return ;
}

MSA_INTR_STATUS_T *CWDM4_206_get_drv_intr_status(uint8_t intr_mask)
{
	static MSA_INTR_STATUS_T status;
	uint8_t value = 0;

	if(intr_mask & PORT_INTR_TX_FAULT)
	{
		_cwdm4_206_txcdr_gn2105_i2c_read_byte(0x437, &value);  //get tx fault
		status.tx_fault = (value & 0x0F);
	}
	if(intr_mask & PORT_INTR_TX_LOS)
	{
		_cwdm4_206_txcdr_gn2105_i2c_read_byte(0x413, &value);  //get tx los
		status.tx_los = (value & 0x0F);
	}
	if(intr_mask & PORT_INTR_TX_LOL)
	{
		_cwdm4_206_txcdr_gn2105_i2c_read_byte(0x413, &value);  //get tx lol
		status.tx_lol = (value & 0xF0) >> 4;
	}
	if(intr_mask & PORT_INTR_RX_LOL)
	{
		_cwdm4_206_rxcdr_m37244_i2c_read_byte(0x17, &value);  //get rx lol
		status.rx_lol = REVERSE_LSB_4BITS(((value & 0xF0) >> 4));
	}
	if(intr_mask & PORT_INTR_RX_LOS)
	{
		_cwdm4_206_rxcdr_m37244_i2c_read_byte(0x17, &value);  //get rx los
		status.rx_los = REVERSE_LSB_4BITS(value & 0x0F);
	}
	return &status;

}

//MSA Lower Page byte93 for power override
void CWDM4_206_set_low_power(uint8_t enable)
{
	if(enable)
	{
		MSA_pin_set_low_power_mode(0);
		IT01CF &= ~IT01CF_IN0PL__ACTIVE_HIGH;
		//TX3V3_EN = 0;  //close tx3v3 in order to realize low power mode
		TX_RST = 0;    //make TX_RST = 0 to prevent mcu provide voltage to cdr chip
		SDA_M = 0;
		SCL_M = 0;
	}
	else
	{
		MSA_pin_set_low_power_mode(1);
		IT01CF |= IT01CF_IN0PL__ACTIVE_HIGH;
		TX3V3_EN = 1; //open tx3v3 in order to realize high power mode
		TX_RST = 1;
		SDA_M = 1;
		SCL_M = 1;
	}
}


//MSA Lower Page byte86 for tx disable
void CWDM4_206_set_tx_enable(uint8_t enale)
{
	uint8_t value = 0;
	
	_cwdm4_206_txcdr_gn2105_i2c_read_byte(0x433,&value);
	value = CLEAR_LSB_4BITS(value) | CLEAR_MSB_4BITS(enale);
	_cwdm4_206_txcdr_gn2105_i2c_write_byte(0x433, value);
	_cwdm4_206_txcdr_gn2105_i2c_read_byte(0x433,&value);
}

//MSA Lower Page byte98 for tx and rx cdr control
void CWDM4_206_set_cdr_control(uint8_t value)
{
	uint8_t tx_cdr_control = 0, rx_cdr_control = 0, tmp = 0, i = 0;
	uint16_t reg_addr[] = {0x000, 0x100, 0x200, 0x300};

	tx_cdr_control = ((~value) & 0xF0) >> 4;
	rx_cdr_control = ((~value) & 0x0F);
	rx_cdr_control = REVERSE_LSB_4BITS(rx_cdr_control);

	for(i = 0; i < MSA_CHANNEL_NUM; i++)
	{
		_cwdm4_206_txcdr_gn2105_i2c_read_byte(reg_addr[i], &tmp);  //set tx cdr control
		tmp = CLEAR_BIT(tmp, 0);
		tmp |= GET_BIT(tx_cdr_control,i);
		_cwdm4_206_txcdr_gn2105_i2c_write_byte(reg_addr[i], tmp);  //set tx cdr control
	}

	_cwdm4_206_rxcdr_m37244_i2c_read_byte(0x10,&tmp);
	tmp = CLEAR_LSB_4BITS(tmp)| rx_cdr_control;
	_cwdm4_206_rxcdr_m37244_i2c_write_byte(0x10, tmp);  //set rx cdr control
}

//MSA Page3 byte234 and byte235 for tx eq
void CWDM4_206_set_tx_eq(uint8_t channel, uint8_t value)
{
	channel = value = 0;
}

//MSA Page3 byte236 and byte237 for rx emphasis
void CWDM4_206_set_rx_emphasis(uint8_t channel, uint8_t value)
{
	uint8_t reg_addr[] = {0x46, 0x46,0x47,0x47};
	uint8_t rx_emphasis[] = {0x0, 0x3, 0x5, 0x7,0x9, 0xb, 0xd, 0xf};
	uint8_t tmp = 0;

	if(value > RX_EMPHASIS_MAX_VALID)
	{
		return ;
	}

	//rx channel reverse,rxcdr pin is inverted sequence with qsfp28 pin
	channel = 3 - channel;

	_cwdm4_206_rxcdr_m37244_i2c_read_byte(reg_addr[channel], &tmp);

	if(channel == 0x0 || channel == 0x2)
	{
		value = CLEAR_LSB_4BITS(tmp) | rx_emphasis[value];
	}
	else if(channel == 0x1 || channel == 0x3)
	{
		value = CLEAR_MSB_4BITS(tmp) | (rx_emphasis[value] << 4);
	}

	_cwdm4_206_rxcdr_m37244_i2c_write_byte(reg_addr[channel], value);
}

//MSA Page3 byte238 and byte239 rx amplitude
//0x3 900  --0x3c
//0x2 600  --0x1e
//0x1 450  --0x0f
//0x0 250  --0x00
void CWDM4_206_set_rx_amplitude(uint8_t channel, uint8_t value)
{
	uint8_t reg_addr[] = {0x42,0x43,0x44,0x45};
	uint8_t rx_amplitude[] = {0x00, 0x0f, 0x1e, 0x3c};
	uint8_t tmp = 0;

	if(value > RX_AMPLITUDE_MAX_VALID)
	{
		return;
	}

	//rx channel reverse,rxcdr pin is inverted sequence with qsfp28 pin
	channel = 3 - channel;

	_cwdm4_206_rxcdr_m37244_i2c_read_byte(reg_addr[channel], &tmp);

	value = (tmp & 0xC0) | rx_amplitude[value];

	_cwdm4_206_rxcdr_m37244_i2c_write_byte(reg_addr[channel], value);
}

//MSA Page3 byte240 sq disable
void CWDM4_206_set_cdr_sq_disable(uint8_t value)
{
	uint8_t tx_sq = 0, rx_sq = 0, tmp = 0, i = 0;
	uint16_t reg_addr[] = {0x014, 0x114, 0x214, 0x314};

	rx_sq = (value & 0xF0) >> 4;
	tx_sq  = ~(value & 0x0F);
	rx_sq = REVERSE_LSB_4BITS(rx_sq);

	for(i = 0; i < MSA_CHANNEL_NUM; i++)
	{
		tmp = 0;
		_cwdm4_206_txcdr_gn2105_i2c_read_byte(reg_addr[i], &tmp);  //set tx cdr control
		tmp = CLEAR_BIT(tmp, 0);
		tmp |= GET_BIT(tx_sq,i);
		_cwdm4_206_txcdr_gn2105_i2c_write_byte(reg_addr[i], tmp);  //set tx cdr control
	}

	_cwdm4_206_rxcdr_m37244_i2c_read_byte(0x40,&tmp);
	tmp = CLEAR_MSB_4BITS(tmp)| (rx_sq << 4);
	_cwdm4_206_rxcdr_m37244_i2c_write_byte(0x40, tmp);  //set rx cdr control
}

//MSA Page3 byte241 for rx output disable
void CWDM4_206_set_rx_output_disable(uint8_t value)
{
	uint8_t rx_output_disable = 0,  tmp = 0;

	rx_output_disable = (value & 0xF0) >> 4;
	rx_output_disable = REVERSE_LSB_4BITS(rx_output_disable);

	_cwdm4_206_rxcdr_m37244_i2c_read_byte(0x10,&tmp);
	tmp = CLEAR_MSB_4BITS(tmp)| (rx_output_disable << 4);
	_cwdm4_206_rxcdr_m37244_i2c_write_byte(0x10, tmp);
}

//MSA Page3 byte241 for tx adapt eq
void CWDM4_206_set_tx_adapt_eq(uint8_t value1, uint8_t value2, uint8_t value3)
{
	value1 = value2 = value3 = 0;
}

#endif




