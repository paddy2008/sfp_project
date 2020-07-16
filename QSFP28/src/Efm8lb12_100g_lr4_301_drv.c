#include <math.h>
#include "Efm8lb12_pid.h"
#include "Efm8lb12_adc.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_smbus.h"
#include "Efm8lb12_common.h"
#include "Efm8lb12_msa_pin.h"
#include "Efm8lb12_100g_com_drv.h"

#ifdef EFM8LB12E64_100G_SFP_LR4_301

static uint8_t _i2c_dev_ready = 0;

/********************************* DRV API Interface ****************************************/
static bool _lr4_301_txcdr_check_exist()
{
	uint8_t i = 0, Data = 0;

#if defined(HW_VER_AM1_301) || defined(HW_VER_AM2_301)
	if(TX3V3_EN == 1)
#else
	if(TX3V3_EN == 0)
#endif
	{
		MSA_p2_log("\r\ntxcdr is lower power mode");
		return false;
	}

	for(i = 0; i < 3; i++)
	{
		if(GOS_OK == SMB_read_byte(LR4_TXCDR_BASE_ADDR, 0x0, &Data))
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

static bool _lr4_301_rxcdr_check_exist()
{
	uint8_t i = 0, Data = 0;

#if defined(HW_VER_AM1_301) || defined(HW_VER_AM2_301)
	if(TX3V3_EN == 1)
#else
	if(TX3V3_EN == 0)
#endif
	{
		MSA_p2_log("\r\nrxcdr is lower power mode");
		return false;
	}

	for(i = 0; i < 3; i++)
	{
		if(GOS_OK == SMB_read_byte(LR4_RXCDR_BASE_ADDR, 0x0, &Data))
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

static bool _lr4_301_tosa_check_exist()
{
	uint8_t i = 0, Data = 0;

#if defined(HW_VER_AM1_301) || defined(HW_VER_AM2_301)
	if(TX3V3_EN == 1)
#else
	if(TX3V3_EN == 0)
#endif
	{
		MSA_p2_log("\r\ntosa is lower power mode");
		return false;
	}

	for(i = 0; i < 3; i++)
	{
		if(GOS_OK == SMB_read_byte(LR4_TOSA_BASE_ADDR, 0x0, &Data))
		{
			MSA_p2_log("\r\ntosa smb read OK");
			return true;
		}
		else
		{
			MSA_p2_log("\r\ntosa smb read failed");
		}
		TIMER5_hw_delay_us(5000);
	}
	return false;
}

static uint8_t _lr4_301_txcdr_m37049_i2c_read_byte(uint8_t OffsetAddr, uint8_t *Data)
{
#if defined(HW_VER_AM1_301) || defined(HW_VER_AM2_301)
	if((TX3V3_EN == 1) ||((_i2c_dev_ready & TX_READY) == 0x0))
#else
	if((TX3V3_EN == 0) ||((_i2c_dev_ready & TX_READY) == 0x0))
#endif
	{
		return GOS_FAIL;
	}
	return SMB_read_byte(LR4_TXCDR_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t _lr4_301_txcdr_m37049_i2c_write_byte(uint8_t OffsetAddr, uint8_t Data)
{
#if defined(HW_VER_AM1_301) || defined(HW_VER_AM2_301)
	if((TX3V3_EN == 1) || ((_i2c_dev_ready & TX_READY) == 0x0))
#else
	if((TX3V3_EN == 0) || ((_i2c_dev_ready & TX_READY) == 0x0))
#endif
	{
		return GOS_FAIL;
	}
	return SMB_write_byte(LR4_TXCDR_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t _lr4_301_rxcdr_m37046_i2c_read_byte(uint8_t OffsetAddr, uint8_t *Data)
{
#if defined(HW_VER_AM1_301) || defined(HW_VER_AM2_301)
	if((TX3V3_EN == 1) || ((_i2c_dev_ready & RX_READY) == 0x0))
#else
	if((TX3V3_EN == 0) || ((_i2c_dev_ready & RX_READY) == 0x0))
#endif
	{
		return GOS_FAIL;
	}
	return SMB_read_byte(LR4_RXCDR_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t _lr4_301_rxcdr_m37046_i2c_write_byte(uint8_t OffsetAddr,uint8_t Data)
{
#if defined(HW_VER_AM1_301) || defined(HW_VER_AM2_301)
	if((TX3V3_EN == 1) || ((_i2c_dev_ready & RX_READY) == 0x0))
#else
	if((TX3V3_EN == 0) || ((_i2c_dev_ready & RX_READY) == 0x0))
#endif
	{
		return GOS_FAIL;
	}
	return SMB_write_byte(LR4_RXCDR_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t _lr4_301_tosa_m4820_i2c_read_byte(uint8_t OffsetAddr, uint8_t *Data)
{
#if defined(HW_VER_AM1_301) || defined(HW_VER_AM2_301)
	if((TX3V3_EN == 1) || ((_i2c_dev_ready & TOSA_READY) == 0x0))
#else
	if((TX3V3_EN == 0) || ((_i2c_dev_ready & TOSA_READY) == 0x0))
#endif
	{
		return GOS_FAIL;
	}
	return SMB_read_byte(LR4_TOSA_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t _lr4_301_tosa_m4820_i2c_write_byte(uint8_t OffsetAddr,uint8_t Data)
{
#if defined(HW_VER_AM1_301) || defined(HW_VER_AM2_301)
	if((TX3V3_EN == 1) || ((_i2c_dev_ready & TOSA_READY) == 0x0))
#else
	if((TX3V3_EN == 0) || ((_i2c_dev_ready & TOSA_READY) == 0x0))
#endif
	{
		return GOS_FAIL;
	}
	return SMB_write_byte(LR4_TOSA_BASE_ADDR, OffsetAddr, Data);
}

static void _lr4_301_port0_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;
	//1.set port default value
#ifdef HW_VER_AM1_301
	P0 = P0_B0__LOW | P0_B1__HIGH | P0_B2__HIGH | P0_B3__LOW | P0_B4__HIGH | P0_B5__HIGH | P0_B6__HIGH | P0_B7__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P0MDIN = P0MDIN_B0__DIGITAL   					   //set p0.0 digital mode  is used for TX circuit voltage enable
			| P0MDIN_B1__ANALOG | P0MDIN_B2__ANALOG    //set p0.1 and p0.2 analog mode is used for monitor TEC current and voltage
			| P0MDIN_B3__DIGITAL 					   //set p0.3 digital mode is used for TEC enable
			| P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL  //set p0.4 and p0.5 digital mode is used for uart0
			| P0MDIN_B6__ANALOG 					   //set p0.6 analog mode is used for mointor thermal resistor  temperature
			| P0MDIN_B7__DIGITAL;					   //set p0.7 digital mode is used for QSFP  interrupt output


	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P0MDOUT = P0MDOUT_B0__PUSH_PULL    							//set p0.0 push pull is used for TX circuit voltage enable
			| P0MDOUT_B1__OPEN_DRAIN | P0MDOUT_B2__OPEN_DRAIN	//because the p0.1 and p0.2 are set analog mode��so these two bits can be not set
			| P0MDOUT_B3__PUSH_PULL	    						//set p0.3 push pull is used for TEC enable
			| P0MDOUT_B4__OPEN_DRAIN | P0MDOUT_B5__OPEN_DRAIN    //set p0.4 push pull and p0.5 open drain is uesd for uart0
			| P0MDOUT_B6__OPEN_DRAIN							//because the p0.6 are set analog mode��so this bit can be not set
			| P0MDOUT_B7__OPEN_DRAIN;						    //set p0.7 open drain is used for QSFP interrupt output


	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P0SKIP = P0SKIP_B0__SKIPPED					//because P1.2 and p1.3 are used to Smbus,so P0.0 ~ P0.7 must be skipped
			| P0SKIP_B1__SKIPPED| P0SKIP_B2__SKIPPED   //because p0.1 and p0.2 are set analog mode ,so the two bits must be skipped are used for monitor TEC current and voltage
			| P0SKIP_B3__SKIPPED
			| P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
			| P0SKIP_B6__SKIPPED 				//because p0.6 is set analog mode ,so this bit must be skipped are used for mointor thermal resistor temperature
			| P0SKIP_B7__SKIPPED;

	//5. set p0MASK
	P0MASK = P0MASK_B0__IGNORED | P0MASK_B1__IGNORED | P0MASK_B2__IGNORED
			| P0MASK_B3__IGNORED | P0MASK_B4__IGNORED | P0MASK_B5__IGNORED
			| P0MASK_B6__IGNORED | P0MASK_B7__IGNORED;

	//6. set P0MAT
	P0MAT = P0MAT_B0__HIGH | P0MAT_B1__HIGH | P0MAT_B2__HIGH | P0MAT_B3__HIGH
			| P0MAT_B4__HIGH | P0MAT_B5__HIGH | P0MAT_B6__HIGH | P0MAT_B7__HIGH;
#endif

#if defined(HW_VER_AM2_301) || defined(HW_VER_AM3_301)
	P0 = P0_B0__HIGH | P0_B1__HIGH | P0_B2__HIGH | P0_B3__LOW | P0_B4__HIGH | P0_B5__HIGH | P0_B6__HIGH | P0_B7__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P0MDIN = P0MDIN_B0__DIGITAL   					   //set p0.0 digital mode  is used for TX circuit voltage enable
			| P0MDIN_B1__DIGITAL | P0MDIN_B2__DIGITAL    //set p0.1 and p0.2 analog mode is used for monitor TEC current and voltage
			| P0MDIN_B3__DIGITAL 					   //set p0.3 digital mode is used for TEC enable
			| P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL  //set p0.4 and p0.5 digital mode is used for uart0
			| P0MDIN_B6__ANALOG 					   //set p0.6 analog mode is used for mointor thermal resistor  temperature
			| P0MDIN_B7__ANALOG;					   //set p0.7 digital mode is used for QSFP  interrupt output


	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P0MDOUT = P0MDOUT_B0__OPEN_DRAIN    							//set p0.0 push pull is used for TX circuit voltage enable
			| P0MDOUT_B1__OPEN_DRAIN | P0MDOUT_B2__OPEN_DRAIN	//because the p0.1 and p0.2 are set analog mode��so these two bits can be not set
			| P0MDOUT_B3__PUSH_PULL	    						//set p0.3 push pull is used for TEC enable
			| P0MDOUT_B4__OPEN_DRAIN | P0MDOUT_B5__OPEN_DRAIN    //set p0.4 push pull and p0.5 open drain is uesd for uart0
			| P0MDOUT_B6__OPEN_DRAIN							//because the p0.6 are set analog mode��so this bit can be not set
			| P0MDOUT_B7__OPEN_DRAIN;						    //set p0.7 open drain is used for QSFP interrupt output


	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P0SKIP = P0SKIP_B0__SKIPPED					//because P1.2 and p1.3 are used to Smbus,so P0.0 ~ P0.7 must be skipped
			| P0SKIP_B1__SKIPPED| P0SKIP_B2__SKIPPED   //because p0.1 and p0.2 are set analog mode ,so the two bits must be skipped are used for monitor TEC current and voltage
			| P0SKIP_B3__SKIPPED
			| P0SKIP_B4__SKIPPED | P0SKIP_B5__SKIPPED
			| P0SKIP_B6__SKIPPED 				//because p0.6 is set analog mode ,so this bit must be skipped are used for mointor thermal resistor temperature
			| P0SKIP_B7__SKIPPED;

	//5. set p0MASK
	P0MASK = P1MASK_B0__COMPARED | P0MASK_B1__IGNORED | P0MASK_B2__IGNORED
			| P0MASK_B3__IGNORED | P0MASK_B4__IGNORED | P0MASK_B5__IGNORED
			| P0MASK_B6__IGNORED | P0MASK_B7__IGNORED;

	//6. set P0MAT
	P0MAT = P0MAT_B0__HIGH | P0MAT_B1__HIGH | P0MAT_B2__HIGH | P0MAT_B3__HIGH
			| P0MAT_B4__HIGH | P0MAT_B5__HIGH | P0MAT_B6__HIGH | P0MAT_B7__HIGH;
#endif


#if defined(HW_VER_AM4_301)
	P0 = P0_B0__HIGH | P0_B1__HIGH | P0_B2__HIGH | P0_B3__LOW | P0_B4__HIGH | P0_B5__HIGH | P0_B6__HIGH | P0_B7__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P0MDIN = P0MDIN_B0__DIGITAL   					   //set p0.0 digital mode  is used for TX circuit voltage enable
			| P0MDIN_B1__DIGITAL | P0MDIN_B2__DIGITAL    //set p0.1 and p0.2 analog mode is used for monitor TEC current and voltage
			| P0MDIN_B3__DIGITAL 					   //set p0.3 digital mode is used for TEC enable
			| P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL  //set p0.4 and p0.5 digital mode is used for uart0
			| P0MDIN_B6__ANALOG 					   //set p0.6 analog mode is used for mointor thermal resistor  temperature
			| P0MDIN_B7__DIGITAL;					   //set p0.7 digital mode is used for QSFP  interrupt output


	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P0MDOUT = P0MDOUT_B0__OPEN_DRAIN    							//set p0.0 push pull is used for TX circuit voltage enable
			| P0MDOUT_B1__OPEN_DRAIN | P0MDOUT_B2__OPEN_DRAIN	//because the p0.1 and p0.2 are set analog mode��so these two bits can be not set
			| P0MDOUT_B3__PUSH_PULL	    						//set p0.3 push pull is used for TEC enable
			| P0MDOUT_B4__OPEN_DRAIN | P0MDOUT_B5__OPEN_DRAIN    //set p0.4 push pull and p0.5 open drain is uesd for uart0
			| P0MDOUT_B6__OPEN_DRAIN							//because the p0.6 are set analog mode��so this bit can be not set
			| P0MDOUT_B7__OPEN_DRAIN;						    //set p0.7 open drain is used for QSFP interrupt output


	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P0SKIP = P0SKIP_B0__SKIPPED					//because P1.2 and p1.3 are used to Smbus,so P0.0 ~ P0.7 must be skipped
			| P0SKIP_B1__SKIPPED| P0SKIP_B2__SKIPPED   //because p0.1 and p0.2 are set analog mode ,so the two bits must be skipped are used for monitor TEC current and voltage
			| P0SKIP_B3__SKIPPED
			| P0SKIP_B4__SKIPPED | P0SKIP_B5__SKIPPED
			| P0SKIP_B6__SKIPPED 				//because p0.6 is set analog mode ,so this bit must be skipped are used for mointor thermal resistor temperature
			| P0SKIP_B7__SKIPPED;

	//5. set p0MASK
	P0MASK = P1MASK_B0__COMPARED | P0MASK_B1__IGNORED | P0MASK_B2__IGNORED
			| P0MASK_B3__IGNORED | P0MASK_B4__IGNORED | P0MASK_B5__IGNORED
			| P0MASK_B6__IGNORED | P0MASK_B7__COMPARED;

	//6. set P0MAT
	P0MAT = P0MAT_B0__HIGH | P0MAT_B1__HIGH | P0MAT_B2__HIGH | P0MAT_B3__HIGH
			| P0MAT_B4__HIGH | P0MAT_B5__HIGH | P0MAT_B6__HIGH | P0MAT_B7__HIGH;
#endif
	SFRPAGE = SFRPAGE_SAVE;
}

static void _lr4_301_port1_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;
	//1.set port default value
#ifdef HW_VER_AM1_301
	P1 = P1_B0__LOW | P1_B1__LOW | P1_B2__HIGH | P1_B3__HIGH | P1_B4__HIGH | P1_B5__HIGH | P1_B6__HIGH | P1_B7__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P1MDIN = P1MDIN_B0__ANALOG  					   //set p1.0 analog mode is used for mointor the 3.3V module input voltage
			| P1MDIN_B1__DIGITAL 					   //set p1.1 digital mode is used for TX close signal
			| P1MDIN_B2__DIGITAL | P1MDIN_B3__DIGITAL  //set p1.2 and p1.3 digital mode is used for i2c master mode

			//set p1.4 ~ p1.7 analog mode are used for MPD0 ~ MPD3
			| P1MDIN_B4__ANALOG  | P1MDIN_B5__ANALOG | P1MDIN_B6__ANALOG | P1MDIN_B7__ANALOG;

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN    			  			//because the p1.1 are set analog mode��so this bit can be not set
			| P1MDOUT_B1__PUSH_PULL					  			//set p1.1 push pull mode is used for TX close signal
			| P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN	//because p1.2 and p1.3 are used for i2c master mode,so the two bits must be open drain

			//because the p1.4 ~ p1.7 are set analog mode��so these bits can be not set
			| P1MDOUT_B4__OPEN_DRAIN | P1MDOUT_B5__OPEN_DRAIN  | P1MDOUT_B6__OPEN_DRAIN	| P1MDOUT_B7__OPEN_DRAIN;

	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P1SKIP = P1SKIP_B0__SKIPPED 			//because p1.0 is set analog mode and p1.2 and p1.3 are used to Smbus ,so this bit must be skipped are used for mointor the 3.3V module input voltage
			| P1SKIP_B1__SKIPPED			//set p1.1 skip in order to p1.2 and p1.3 are used for i2c master mode
			| P1SKIP_B2__NOT_SKIPPED | P1SKIP_B3__NOT_SKIPPED

			//because p1.4 ~ p1.7 are set analog mode ,so these bits must be skipped are used for for MPD0 ~ MPD3
			| P1SKIP_B4__SKIPPED | P1SKIP_B5__SKIPPED | P1SKIP_B6__SKIPPED | P1SKIP_B7__SKIPPED;

	//5. set p1MASK
	P1MASK = P1MASK_B0__IGNORED | P1MASK_B1__IGNORED | P1MASK_B2__IGNORED
			| P1MASK_B3__IGNORED | P1MASK_B4__IGNORED | P1MASK_B5__IGNORED
			| P1MASK_B6__IGNORED | P1MASK_B7__IGNORED;

	//6. set P1MAT
	P1MAT = P1MAT_B0__HIGH | P1MAT_B1__HIGH | P1MAT_B2__HIGH | P1MAT_B3__HIGH
			| P1MAT_B4__HIGH | P1MAT_B5__HIGH | P1MAT_B6__HIGH | P1MAT_B7__HIGH;
#endif

#if defined(HW_VER_AM2_301) || defined(HW_VER_AM3_301) || defined(HW_VER_AM4_301)
	P1 = P1_B0__HIGH | P1_B1__LOW | P1_B2__LOW | P1_B3__LOW | P1_B4__HIGH | P1_B5__HIGH | P1_B6__HIGH | P1_B7__HIGH;

		//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
		P1MDIN = P1MDIN_B0__ANALOG  					   //set p1.0 analog mode is used for mointor the 3.3V module input voltage
				| P1MDIN_B1__DIGITAL 					   //set p1.1 digital mode is used for TX close signal
				| P1MDIN_B2__DIGITAL | P1MDIN_B3__DIGITAL  //set p1.2 and p1.3 digital mode is used for i2c master mode
				//set p1.4 ~ p1.7 analog mode are used for MPD0 ~ MPD3
				| P1MDIN_B4__ANALOG  | P1MDIN_B5__ANALOG | P1MDIN_B6__ANALOG | P1MDIN_B7__ANALOG;

		//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
		P1MDOUT = P1MDOUT_B0__OPEN_DRAIN    			  			//because the p1.1 are set analog mode��so this bit can be not set
				| P1MDOUT_B1__PUSH_PULL					  			//set p1.1 push pull mode is used for TX close signal
				| P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN	//because p1.2 and p1.3 are used for i2c master mode,so the two bits must be open drain
				//because the p1.4 ~ p1.7 are set analog mode��so these bits can be not set
				| P1MDOUT_B4__OPEN_DRAIN | P1MDOUT_B5__OPEN_DRAIN  | P1MDOUT_B6__OPEN_DRAIN	| P1MDOUT_B7__OPEN_DRAIN;

		//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
		P1SKIP = P1SKIP_B0__SKIPPED 			//because p1.0 is set analog mode and p1.2 and p1.3 are used to Smbus ,so this bit must be skipped are used for mointor the 3.3V module input voltage
				| P1SKIP_B1__SKIPPED			//set p1.1 skip in order to p1.2 and p1.3 are used for i2c master mode
				| P1SKIP_B2__NOT_SKIPPED | P1SKIP_B3__NOT_SKIPPED

				//because p1.4 ~ p1.7 are set analog mode ,so these bits must be skipped are used for for MPD0 ~ MPD3
				| P1SKIP_B4__SKIPPED | P1SKIP_B5__SKIPPED | P1SKIP_B6__SKIPPED | P1SKIP_B7__SKIPPED;

		//5. set p1MASK
		P1MASK = P1MASK_B0__IGNORED | P1MASK_B1__IGNORED | P1MASK_B2__IGNORED
				| P1MASK_B3__IGNORED | P1MASK_B4__IGNORED | P1MASK_B5__IGNORED
				| P1MASK_B6__IGNORED | P1MASK_B7__IGNORED;

		//6. set P1MAT
		P1MAT = P1MAT_B0__HIGH | P1MAT_B1__HIGH | P1MAT_B2__HIGH | P1MAT_B3__HIGH
				| P1MAT_B4__HIGH | P1MAT_B5__HIGH | P1MAT_B6__HIGH | P1MAT_B7__HIGH;
#endif

	SFRPAGE = SFRPAGE_SAVE;
}

static void _lr4_301_port2_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x20;
	//1.set port default value
#ifdef HW_VER_AM1_301
	P2 = P2_B0__HIGH | P2_B1__HIGH | P2_B2__HIGH | P2_B3__HIGH | P2_B4__HIGH | P2_B5__HIGH | P2_B6__HIGH ;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P2MDIN = P2MDIN_B0__DIGITAL | P2MDIN_B1__DIGITAL	 //set p2.0 and p2.1 digital mode are used for i2c slave mode

			| P2MDIN_B2__DIGITAL   //set  p2.2 digital mode is used for tosa reset signal

			//p2.3 ~ p2.6  analog mode are used for RSSI0 ~ RSSI3
			| P2MDIN_B3__ANALOG | P2MDIN_B4__ANALOG | P2MDIN_B5__ANALOG | P2MDIN_B6__ANALOG ;

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).

	P2MDOUT = P2MDOUT_B0__OPEN_DRAIN | P2MDOUT_B1__OPEN_DRAIN //set p2.0 and p2.1 open drain mode are used for i2c slave mode

			| P2MDOUT_B2__PUSH_PULL  //set  p2.2 push pull is used for tosa reset signal

			//because the p2.3 ~ p2.6 are set analog mode��so these bits can be not set
			| P2MDOUT_B3__OPEN_DRAIN | P2MDOUT_B4__OPEN_DRAIN | P2MDOUT_B5__OPEN_DRAIN  | P2MDOUT_B6__OPEN_DRAIN;


	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P2SKIP = P2SKIP_B0__SKIPPED | P2SKIP_B1__SKIPPED   		//set p2.0 and p2.1 skipped for i2c slave mode
			| P2SKIP_B2__SKIPPED

			//because p2.3 ~ p2.6 are set analog mode ,so these bits must be skipped are used for for RSSI0 ~ RSSI3
			| P2SKIP_B3__SKIPPED | 0x10 | 0x20 | 0x40;


	//5. set p2MASK
	P2MASK = P2MASK_B0__IGNORED | P2MASK_B1__IGNORED | P2MASK_B2__IGNORED
			| P2MASK_B3__IGNORED | P2MASK_B4__IGNORED | P2MASK_B5__IGNORED
			| P2MASK_B6__IGNORED ;

	//6. set P2MAT
	P2MAT = P2MAT_B0__HIGH | P2MAT_B1__HIGH | P2MAT_B2__HIGH | P2MAT_B3__HIGH
			| P2MAT_B4__HIGH | P2MAT_B5__HIGH | P2MAT_B6__HIGH ;
#endif

#if defined(HW_VER_AM2_301) || defined(HW_VER_AM3_301) || defined(HW_VER_AM4_301)
	P2 = P2_B0__HIGH | P2_B1__HIGH | P2_B2__HIGH | P2_B3__HIGH | P2_B4__HIGH | P2_B5__HIGH | P2_B6__HIGH ;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P2MDIN = P2MDIN_B0__DIGITAL | P2MDIN_B1__DIGITAL	 //set p2.0 and p2.1 digital mode are used for i2c slave mode

			| P2MDIN_B2__ANALOG   //set  p2.2 digital mode is used for tosa reset signal

			//p2.3 ~ p2.6  analog mode are used for RSSI0 ~ RSSI3
			| P2MDIN_B3__ANALOG | P2MDIN_B4__ANALOG | P2MDIN_B5__ANALOG | P2MDIN_B6__ANALOG ;

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).

	P2MDOUT = P2MDOUT_B0__OPEN_DRAIN | P2MDOUT_B1__OPEN_DRAIN //set p2.0 and p2.1 open drain mode are used for i2c slave mode

			| P2MDOUT_B2__OPEN_DRAIN   //set  p2.2 push pull is used for tosa reset signal

			//because the p2.3 ~ p2.6 are set analog mode��so these bits can be not set
			| P2MDOUT_B3__OPEN_DRAIN | P2MDOUT_B4__OPEN_DRAIN | P2MDOUT_B5__OPEN_DRAIN  | P2MDOUT_B6__OPEN_DRAIN;


	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P2SKIP = P2SKIP_B0__SKIPPED | P2SKIP_B1__SKIPPED   		//set p2.0 and p2.1 skipped for i2c slave mode
			| P2SKIP_B2__SKIPPED

			//because p2.3 ~ p2.6 are set analog mode ,so these bits must be skipped are used for for RSSI0 ~ RSSI3
			| P2SKIP_B3__SKIPPED | 0x10 | 0x20 | 0x40;


	//5. set p2MASK
	P2MASK = P2MASK_B0__IGNORED | P2MASK_B1__IGNORED | P2MASK_B2__IGNORED
			| P2MASK_B3__IGNORED | P2MASK_B4__IGNORED | P2MASK_B5__IGNORED
			| P2MASK_B6__IGNORED ;

	//6. set P2MAT
	P2MAT = P2MAT_B0__HIGH | P2MAT_B1__HIGH | P2MAT_B2__HIGH | P2MAT_B3__HIGH
			| P2MAT_B4__HIGH | P2MAT_B5__HIGH | P2MAT_B6__HIGH ;
#endif

	SFRPAGE = SFRPAGE_SAVE;
}

static void _lr4_301_port3_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x20;
	//1.set port default value
#ifdef HW_VER_AM1_301
	P3 = P3_B0__HIGH | P3_B1__HIGH | P3_B2__HIGH | P3_B3__HIGH | P3_B4__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P3MDIN = P3MDIN_B0__DIGITAL    //set p3.0 digital mode is used for qsfp low power mode

			| P3MDIN_B1__DIGITAL   //set p3.1 digital mode is used for DAC

			| P3MDIN_B2__DIGITAL   //set p3.2 digital mode is used for i2c slave mode chip selection signal

			| P3MDIN_B3__DIGITAL   //set p3.3 digital mode is used for qsfp module reset

			| P3MDIN_B4__DIGITAL;   //set p3.4 digital mode is used for tx circuit 1.8 voltage enable

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P3MDOUT = P3MDOUT_B0__PUSH_PULL  //set p3.0 push pull is used for qsfp low power mode

			| P3MDOUT_B1__OPEN_DRAIN //set p3.1 open drain mode is used for DAC

			| P3MDOUT_B2__OPEN_DRAIN  //set p3.2 push pull is used for i2c slave mode chip selection signal

			| P3MDOUT_B3__PUSH_PULL  //set p3.3 push pull  is used for qsfp module reset

			| P3MDOUT_B4__PUSH_PULL;  //set p3.4 push pull is used for tx circuit voltage enable
#endif

#if defined(HW_VER_AM2_301) || defined(HW_VER_AM3_301)

#if defined(HW_VER_AM2_301)
	P3 = P3_B0__HIGH | P3_B1__HIGH | P3_B2__HIGH | P3_B3__HIGH | P3_B4__HIGH;
#else
	P3 = P3_B0__HIGH | P3_B1__LOW | P3_B2__HIGH | P3_B3__HIGH | P3_B4__LOW;
#endif
	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P3MDIN = P3MDIN_B0__DIGITAL    //set p3.0 digital mode is used for qsfp low power mode

			| P3MDIN_B1__DIGITAL   //set p3.1 digital mode is used for DAC

			| P3MDIN_B2__DIGITAL   //set p3.2 digital mode is used for i2c slave mode chip selection signal

			| P3MDIN_B3__DIGITAL   //set p3.3 digital mode is used for qsfp module reset

			| P3MDIN_B4__DIGITAL;   //set p3.4 digital mode is used for tx circuit 1.8 voltage enable

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P3MDOUT = P3MDOUT_B0__OPEN_DRAIN  //set p3.0 push pull is used for qsfp low power mode

			| P3MDOUT_B1__PUSH_PULL //set p3.1 open drain mode is used for DAC

			| P3MDOUT_B2__PUSH_PULL  //set p3.2 push pull is used for i2c slave mode chip selection signal

			| P3MDOUT_B3__OPEN_DRAIN  //set p3.3 push pull  is used for qsfp module reset

			| P3MDOUT_B4__PUSH_PULL;  //set p3.4 push pull is used for tx circuit voltage enable
#endif


#if defined(HW_VER_AM4_301)
	P3 = P3_B0__HIGH | P3_B1__LOW | P3_B2__HIGH | P3_B3__HIGH | P3_B4__LOW;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P3MDIN = P3MDIN_B0__DIGITAL    //set p3.0 digital mode is used for qsfp low power mode

			| P3MDIN_B1__DIGITAL   //set p3.1 digital mode is used for DAC

			| P3MDIN_B2__DIGITAL   //set p3.2 digital mode is used for i2c slave mode chip selection signal

			| P3MDIN_B3__DIGITAL   //set p3.3 digital mode is used for qsfp module reset

			| P3MDIN_B4__DIGITAL;   //set p3.4 digital mode is used for tx circuit 1.8 voltage enable

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P3MDOUT = P3MDOUT_B0__OPEN_DRAIN  //set p3.0 push pull is used for qsfp low power mode

			| P3MDOUT_B1__PUSH_PULL //set p3.1 open drain mode is used for DAC

			| P3MDOUT_B2__OPEN_DRAIN  //set p3.2 push pull is used for i2c slave mode chip selection signal

			| P3MDOUT_B3__OPEN_DRAIN  //set p3.3 push pull  is used for qsfp module reset

			| P3MDOUT_B4__PUSH_PULL;  //set p3.4 push pull is used for tx circuit voltage enable
#endif
	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).

	//p3 do't have p3SKIP registers

	//5. set p2MASK
	//p3 do't have P3MASK registers

	//6. set P2MAT
	//p3 do't have P3MAT registers
	SFRPAGE = SFRPAGE_SAVE;
}

static void _lr4_301_txcdr_m37049_reset()
{
	if(_lr4_301_txcdr_check_exist())
	{
		_i2c_dev_ready |= TX_READY;
	}

	_lr4_301_txcdr_m37049_i2c_write_byte(M3704X_RESET,0xaa);
}

static void _lr4_301_rxcdr_m37046_reset()
{
	if(_lr4_301_rxcdr_check_exist())
	{
		_i2c_dev_ready |= RX_READY;
	}

	_lr4_301_rxcdr_m37046_i2c_write_byte(M3704X_RESET, 0xaa);  	//rxcdr reset
	TIMER_soft_delay_ms(100);
}

static void _lr4_301_tosa_m4820_reset()
{
	uint8_t value = 0x1;
	uint16_t counter = 0;

	if(_lr4_301_tosa_check_exist())
	{
		_i2c_dev_ready |= TOSA_READY;
	}

	TOSA_RST = 0;
	TIMER_soft_delay_ms(10);
	TOSA_RST = 1;

	while(value)
	{
		_lr4_301_tosa_m4820_i2c_read_byte(TOSA_PAGE0_RESET, &value);
		counter ++;
		TIMER_soft_delay_ms(counter * 2);
		if(counter >= 100)
		{
			MSA_p2_log("\r\ntosa reset failed");
			return;
		}
	}
}

static void _lr4_301_txcdr_m37049_init()
{
	_lr4_301_txcdr_m37049_reset();
	_lr4_301_txcdr_m37049_i2c_write_byte(M3704X_MODE_CH(2),0x04);
	_lr4_301_txcdr_m37049_i2c_write_byte(M3704X_MODE_CH(3),0x04);
//	_lr4_301_txcdr_m37049_i2c_write_byte(M3704X_OUTPUT_DEEMPH,0x58);
}

static void _lr4_301_rxcdr_m37046_init()
{
	_lr4_301_rxcdr_m37046_reset();
	_lr4_301_rxcdr_m37046_i2c_write_byte(M3704X_MODE,0x00);
	_lr4_301_rxcdr_m37046_i2c_write_byte(M3704X_LOS_THRS,0x15);
//	_lr4_301_rxcdr_m37046_i2c_write_byte(M3704X_OUTPUT_DEEMPH,0x58);
}


static uint8_t _lr4_301_tasa_m4820_value(uint8_t ch,SFP_DRV_LASER_CFG_T *cfg)
{
	uint8_t page;
	static code uint8_t mod_reg[][2]   = {{0x05,TOSA_PAGEN_VO1},{0x05,TOSA_PAGEN_VO2},{0x07,TOSA_PAGEN_VO1},{0x07,TOSA_PAGEN_VO2}};
	static code uint8_t cross_reg[][2] = {{0x04,TOSA_PAGEN_VO1},{0x04,TOSA_PAGEN_VO2},{0x06,TOSA_PAGEN_VO1},{0x06,TOSA_PAGEN_VO2}};

	ch = 3 - ch;

	_lr4_301_tosa_m4820_i2c_read_byte(TOSA_PAGE0_INDIRECT_PAGE,(uint8_t*)&page);

	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,ch+4);
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGEN_GAIN_ADJUST,(uint8_t)cfg->bias);   //VB

	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,mod_reg[ch][0]);
	_lr4_301_tosa_m4820_i2c_write_byte(mod_reg[ch][1],(uint8_t)cfg->mod);			//VC

	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,cross_reg[ch][0]);
	_lr4_301_tosa_m4820_i2c_write_byte(cross_reg[ch][1],(uint8_t)cfg->cross);		//VX

	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,page);

	return GOS_OK;
}
static void _lr4_301_tosa_m4820_init()
{
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,0x00);
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,0x04);
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGEN_AGC_ENABLE,0x00);
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,0x05);
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGEN_AGC_ENABLE,0x00);
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,0x06);
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGEN_AGC_ENABLE,0x00);
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,0x07);
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGEN_AGC_ENABLE,0x00);
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,0x00);
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_CHX_SHUTDOW,0xff);
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_ADC_ENABLE_II,0xe0);
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_ADC_ENABLE_II+1,0x01);
}

/************************************ Public Interface Drv API *************************************/
void LR4_301_chip_reset()
{
	_lr4_301_txcdr_m37049_reset();
	_lr4_301_rxcdr_m37046_reset();
	_lr4_301_tosa_m4820_reset();
}

void LR4_301_chip_init()
{

	LR4_301_chip_reset();
	_lr4_301_txcdr_m37049_init();
	_lr4_301_rxcdr_m37046_init();
	_lr4_301_tosa_m4820_init();
}

void LR4_301_port_init()
{
	_lr4_301_port0_init();
	_lr4_301_port1_init();
	_lr4_301_port2_init();
	_lr4_301_port3_init();
}


int16_t  LR4_301_ddmi_get_temp(MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		DDMI_CALI_PARA_T *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		return ((int16_t)((float)(ddmi_cali->temp) * TIMER_temp_slow_boot_cal()) + ADC0_get_mcu_temp());  		//MSA format uint: 1/256 C
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return  ADC0_get_mcu_temp();  //uint: 1/256 C
	}

	return GOS_FAIL;

}

uint16_t LR4_301_ddmi_get_vcc(MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		DDMI_CALI_PARA_T *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		return (ddmi_cali->vcc + ADC0_get_vcc()); //uint:0.1mv
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return ADC0_get_vcc(); //uint:0.1mv
	}
	return 0;
}

static uint16_t _lr4_301_get_bias_average_value()
{
	uint16_t buff16[AVERAGE_MAX_COUNTER] = {0};
	uint8_t i = 0, value_h = 0, value_l = 0;

	for(i = 0; i < AVERAGE_MAX_COUNTER; i++)
	{
		_lr4_301_tosa_m4820_i2c_read_byte(TOSA_PAGEN_EXTERNAL_SOURCE_II,(uint8_t*)&value_l);
		_lr4_301_tosa_m4820_i2c_read_byte(TOSA_PAGEN_EXTERNAL_SOURCE_II+1,(uint8_t*)&value_h);
		buff16[i] = value_l | (value_h << 8);
	}

	return com_average_value16(buff16, AVERAGE_MAX_COUNTER);
}

static uint16_t _lr4_301_get_ddm_bias(uint8_t channel)
{
	uint16_t  value16 = 0;
	uint8_t page;

	channel = 3 - channel;
	_lr4_301_tosa_m4820_i2c_read_byte(TOSA_PAGE0_INDIRECT_PAGE,(uint8_t*)&page);
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE, channel+4);
	value16 = _lr4_301_get_bias_average_value();
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE, page);
	return (uint16_t)((float)value16 * 0.0568);
}

uint16_t LR4_301_ddmi_get_bias(uint8_t channel, MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		uint16_t bias = 0;
		float bias_temp_cali = 0.0;
		const DDMI_CALI_PARA_T *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		const MSA_LUT_T	 *laser_stat = MSA_p4_get_laser_state();
		const SFP_DRV_LASER_CFG_T *lo_cfg = (SFP_DRV_LASER_CFG_T *)MSA_p67_get_drv_lut();
		const SFP_DRV_LASER_CFG_T *m_cfg  = (lo_cfg + MSA_CHANNEL_NUM);

		if(MSA_p67_check_apc_lut_tab(lo_cfg))  //apc paramters is invalid, so there is no need to apc calibration
		{
			const int16_t cur_temp = ADC0_get_mcu_temp();

			if(cur_temp <= (m_cfg + channel)->temp)  //low temp
			{
				bias_temp_cali = (float)(cur_temp - (m_cfg + channel)->temp) / (float)TEMP_SCALE * 0.1;
			}
			else  //high temp
			{
				bias_temp_cali = (float)(cur_temp - (m_cfg + channel)->temp) / (float)TEMP_SCALE * 0.56;
			}
		}

		bias  = (uint16_t)((((float)laser_stat->ch[channel].stat.txbias + ddmi_cali->current[channel].bias.bias_offset2  - ddmi_cali->current[channel].bias.bias_offset1) * 0.1 + bias_temp_cali)  * 1000.0 * 0.5); //uint:2uA
		return  bias;//uint:2uA
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return (uint16_t)((_lr4_301_get_ddm_bias(channel)) *10);  //0.1mA
	}
	return GOS_FAIL;
}

static uint16_t _lr4_301_get_ddm_rssi(uint8_t channel)
{
	return  (uint16_t)(((float)ADC0_get_rssi(channel)*0.1)/(float)RSSI_SERIES_RES* 1000.00);
}

uint16_t LR4_301_ddmi_get_rx_power(uint8_t channel, MSA_DRV_VAL_TYPE type)
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
		return  _lr4_301_get_ddm_rssi(channel);  //uint:uA (uint16_t)((float)rssi_mv / (float)RSSI_SERIES_RES * 1000.00);  //uA
	}
	return GOS_FAIL;
}

static uint16_t _lr4_301_get_ddm_mpd(uint8_t channel)
{
	channel = 3 - channel;
	return ADC0_get_mpd(channel);
}

uint16_t LR4_301_ddmi_get_tx_power(uint8_t channel, MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		float  txmon_N = 0.0;
		float txpower_result = 0.0, txpower1 = 0.0, txpower2 = 0.0, bias1 = 0.0, bias2 = 0.0;
		const DDMI_CALI_PARA_T    *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		const MSA_LUT_T 		  *laser_stat = MSA_p4_get_laser_state();

		txmon_N =(float)laser_stat->ch[channel].stat.txpwr*0.1 ;

		//ddmi_cali->tx_rx_pwr[channel].txpower.txpower uint:0.1uw
		txpower1 = (float)ddmi_cali->current[channel].txpower.txpower1 * 0.1 / 1000.0; //uint:mW
		txpower2 = (float)ddmi_cali->current[channel].txpower.txpower2 * 0.1 / 1000.0; //uint:mW

		//ddmi_cali->tx_rx_pwr[channel].txpower.bias uint:0.1mA
		bias1 = (float)ddmi_cali->current[channel].txpower.bias1 * 0.1; //uint:mA
		bias2 = (float)ddmi_cali->current[channel].txpower.bias2 * 0.1; //uint:mA

		if(bias1 != bias2)
		{
			txpower_result = txpower1 + (txpower1 - txpower2)/(bias1 - bias2) * (txmon_N - bias1);  //uint:mW
		}

		CHECK_POWER_MIN(txpower_result);
		CHECK_POWER_MAX(txpower_result);
		return (uint16_t)(txpower_result * 1000.0 * 10.0); //MSA format uint:0.1uW
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return _lr4_301_get_ddm_mpd(channel); // uint:0閵嗭拷mA
	}
	return GOS_FAIL;
}

void LR4_301_drv_adjust_laser_ch(uint8_t ch, SFP_DRV_LASER_CFG_T *cfg)
{
	_lr4_301_tasa_m4820_value(ch,cfg);
}

void LR4_301_drv_get_laser_full_state(uint8_t ch, MSA_LUT_T *lut)
{
	uint8_t page,bias, mod, cross;
	static code uint8_t mod_reg[][2]   = {{0x05,TOSA_PAGEN_VO1},{0x05,TOSA_PAGEN_VO2},{0x07,TOSA_PAGEN_VO1},{0x07,TOSA_PAGEN_VO2}};
	static code uint8_t cross_reg[][2] = {{0x04,TOSA_PAGEN_VO1},{0x04,TOSA_PAGEN_VO2},{0x06,TOSA_PAGEN_VO1},{0x06,TOSA_PAGEN_VO2}};

	lut->temp = DDM_GET_TEMP(MSA_DRV_VAL_HW);
	_lr4_301_tosa_m4820_i2c_read_byte(TOSA_PAGE0_INDIRECT_PAGE,(uint8_t*)&page);

	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,(3 - ch)+4);
	_lr4_301_tosa_m4820_i2c_read_byte(TOSA_PAGEN_GAIN_ADJUST,&bias);   //VB

	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,mod_reg[(3 - ch)][0]);
	_lr4_301_tosa_m4820_i2c_read_byte(mod_reg[(3 - ch)][1],&mod);			//VC

	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,cross_reg[(3 - ch)][0]);
	_lr4_301_tosa_m4820_i2c_read_byte(cross_reg[(3 - ch)][1],&cross);		//VX

	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,page);

	lut->ch[ch].config.bias = (uint16_t)bias;
	lut->ch[ch].config.mod  = (uint16_t)mod;
	lut->ch[ch].config.cross = (uint16_t)cross;
}


void LR4_301_drv_poll()
{
	ADC0_poll();
}

static void  _lr4_301_drv_check_laser_ch(SFP_DRV_LASER_CFG_T *cfg)
{
	if(cfg->bias >= 255)
	{
		cfg->bias = 255;
	}
	if(cfg->mod >= 255)
	{
		cfg->mod = 255;
	}
	if(cfg->cross >= 255)
	{
		cfg->cross = 255;
	}
}


void LR4_301_drv_ch_calc_cfg(int16_t cur_temp, uint8_t ch, const SFP_DRV_LASER_CFG_T *lut, SFP_DRV_LASER_CFG_T *ret)
{
	SFP_DRV_LASER_CFG_T *lo_cfg = (lut + ch);
	SFP_DRV_LASER_CFG_T *m_cfg  = (lo_cfg + MSA_CHANNEL_NUM);
	SFP_DRV_LASER_CFG_T *hi_cfg = (m_cfg + MSA_CHANNEL_NUM);

	if(!MSA_p67_check_apc_lut_tab(lut))  //apc paramters is invalid, so there is no need to apc calibration
	{
		ret->bias  = 0;
		ret->mod   = 0;
		ret->cross = 0;
		return;
	}

	ret->temp = cur_temp;
	if ((cur_temp <= m_cfg->temp) && (m_cfg->temp != lo_cfg->temp))
	{
		ret->bias = (uint16_t )(floor((double)lo_cfg->bias +
						((double)(cur_temp - lo_cfg->temp)) * ((double)(m_cfg->bias - lo_cfg->bias)/ (m_cfg->temp - lo_cfg->temp))));
		ret->mod = (uint16_t)(floor((double)lo_cfg->mod +
						((double)(cur_temp - lo_cfg->temp)) * ((double)(m_cfg->mod - lo_cfg->mod)/ (m_cfg->temp - lo_cfg->temp))));
		ret->cross = (int16_t)(floor((double)lo_cfg->cross +
						((double)(cur_temp - lo_cfg->temp)) * ((double)(m_cfg->cross - lo_cfg->cross)/ (m_cfg->temp - lo_cfg->temp))));
	}
	else if ((cur_temp > m_cfg->temp) && (m_cfg->temp != hi_cfg->temp))
	{
		ret->bias = (uint16_t )(floor((double)m_cfg->bias +
						((double)(cur_temp - m_cfg->temp)) * ((double)(hi_cfg->bias - m_cfg->bias)/ (hi_cfg->temp - m_cfg->temp))));
		ret->mod = (uint16_t)(floor((double)m_cfg->mod +
						((double)(cur_temp - m_cfg->temp)) * ((double)(hi_cfg->mod - m_cfg->mod)/ (hi_cfg->temp - m_cfg->temp))));
		ret->cross = (int16_t)(floor((double)m_cfg->cross +
						((double)(cur_temp - m_cfg->temp)) * ((double)(hi_cfg->cross - m_cfg->cross)/(hi_cfg->temp - m_cfg->temp))));
	}
	else
	{
		ret->bias  = 0;
		ret->mod   = 0;
		ret->cross = 0;
	}

	_lr4_301_drv_check_laser_ch(ret);
}

void LR4_301_update_dev_ready_status()
{
	_lr4_301_rxcdr_check_exist();  //update  _i2c_dev_ready
	_lr4_301_txcdr_check_exist();  //update  _i2c_dev_ready
}

MSA_INTR_STATUS_T *LR4_301_get_drv_intr_status(uint8_t intr_mask)
{
	static MSA_INTR_STATUS_T status;
	uint16_t ts = 0;
	uint8_t value = 0;

	ts = MSA_get_poll_timestamp();

	if(ts % 10)
	{
		return NULL;
	}

	if(intr_mask & PORT_INTR_TX_FAULT)  //read register until there is a tx fault intr
	{
		_lr4_301_txcdr_m37049_i2c_read_byte(M3704X_LOL_OR_LOS_LOS_STATUS, &value);  //get tx fault
		status.tx_fault = REVERSE_LSB_4BITS(value & 0x0F);
	}

	if(intr_mask & PORT_INTR_TX_LOS)  //read register until there is a tx los intr
	{
		_lr4_301_txcdr_m37049_i2c_read_byte(M3704X_LOS_LOL_STATUS, &value);  //get tx los
		status.tx_los = REVERSE_LSB_4BITS(value & 0x0F);;
	}

	if(intr_mask & PORT_INTR_TX_LOL)  //read register until there is a tx lol intr
	{
		_lr4_301_txcdr_m37049_i2c_read_byte(M3704X_LOS_LOL_STATUS, &value);  //get tx lol
		status.tx_lol = REVERSE_LSB_4BITS(value & 0xF0) >> 4;
	}

	if(intr_mask & PORT_INTR_RX_LOS)  //read register until there is a rx los intr
	{
		_lr4_301_rxcdr_m37046_i2c_read_byte(M3704X_LOS_LOL_STATUS, &value);  //get rx lol
		status.rx_los = (value & 0x0F);
	}

	if(intr_mask & PORT_INTR_RX_LOL)  //read register until there is a rx lol intr
	{
		_lr4_301_rxcdr_m37046_i2c_read_byte(M3704X_LOS_LOL_STATUS, &value);  //get rx lol
		status.rx_lol = (value & 0xF0) >> 4;
	}
	return &status;
}

void LR4_301_set_tec_enable(uint8_t enale)
{
	TX_DIS   =   enale & 0x01;
	TOSA_RST = !(enale & 0x01);
	TEC_EN   = !(enale & 0x01);
}

uint16_t LR4_301_get_tec_tempout()
{
	return PID_get_tempout();
}

uint16_t LR4_301_get_tec_tempset()
{
	return PID_get_tempset();
}

uint16_t LR4_301_get_itec()
{
	return ADC0_get_itec();
}

uint16_t LR4_301_get_vtec()
{
	return ADC0_get_vtec();
}

//MSA Lower Page byte86 for tx disable
void LR4_301_set_tx_enable(uint8_t enale)
{
	uint8_t value = 0,page;

	//tx channel reverse
	enale = REVERSE_LSB_4BITS(enale);
	_lr4_301_tosa_m4820_i2c_read_byte(TOSA_PAGE0_INDIRECT_PAGE,&page);

	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,0);
	value = (enale & 0x01)|((enale & 0x02)<<1)|((enale & 0x04)<<2)|((enale & 0x08) <<3);
	value = ~((value<<1) | value);
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_CHX_SHUTDOW, value);
	_lr4_301_tosa_m4820_i2c_read_byte(TOSA_PAGE0_CHX_SHUTDOW, &value);
	value = ~(value & 0x01)|((value & 0x02)>>1)|((value & 0x04)>>2)|((value & 0x08)>>3);
	_lr4_301_tosa_m4820_i2c_write_byte(TOSA_PAGE0_INDIRECT_PAGE,page);
}

//MSA Lower Page byte93 for power override
void LR4_301_set_low_power(uint8_t enable)
{
	if(enable == 0)
	{
		MSA_pin_set_low_power_mode(1);
		IT01CF |= IT01CF_IN0PL__ACTIVE_HIGH;
		TOSA_RST = 1;
		TEC_EN   = 1;
		TX_DIS = 0;
		SDA_M = 1;
		SCL_M = 1;
#if defined(HW_VER_AM1_301) || defined(HW_VER_AM2_301)
		TX3V3_EN = 0; //open tx3v3 in order to realize high power mode
		TX1V8_EN = 1;
#endif

#if defined(HW_VER_AM3_301)
		TX1V8_EN = 1;
		TX3V3_EN = 1; //open tx3v3 in order to realize high power mode
#endif

#if defined(HW_VER_AM4_301)
		TX3V3_EN = 1; //open tx3v3 in order to realize high power mode
#endif
	}
	else
	{
		MSA_pin_set_low_power_mode(0);
		IT01CF &= ~IT01CF_IN0PL__ACTIVE_HIGH;
		TOSA_RST = 0;
		TEC_EN   = 0;
		TX_DIS = 1;
		SDA_M = 0;
		SCL_M = 0;
#if defined(HW_VER_AM1_301) || defined(HW_VER_AM2_301)
		TX3V3_EN = 1;  //close tx3v3 in order to realize low power mode
		TX1V8_EN = 0;
#endif

#if defined(HW_VER_AM3_301)
//		TX1V8_EN = 0;
//		TX3V3_EN = 0;  //close tx3v3 in order to realize low power mode
#endif

#if defined(HW_VER_AM4_301)
//		TX3V3_EN = 0;  //close tx3v3 in order to realize low power mode
#endif
	}
}

//MSA Lower Page byte98 for tx and rx cdr control
void LR4_301_set_cdr_control(uint8_t value)
{
	uint8_t tx_cdr_control = 0, rx_cdr_control = 0, tmp = 0,channel;

	//tx channel reverse
	tx_cdr_control = ~REVERSE_LSB_4BITS(((value) & 0xF0) >> 4);
	rx_cdr_control = ~(value & 0x0F);

	for(channel = 0; channel < 4; channel ++)
	{
		_lr4_301_txcdr_m37049_i2c_read_byte(M3704X_MODE_CH(channel),&tmp);
		tmp = CLEAR_BIT(tmp, 5) | (GET_BIT(tx_cdr_control, channel) << 5);
		_lr4_301_txcdr_m37049_i2c_write_byte(M3704X_MODE_CH(channel),tmp);

		_lr4_301_rxcdr_m37046_i2c_read_byte(M3704X_MODE_CH(channel),&tmp);
		tmp = CLEAR_BIT(tmp, 5) | (GET_BIT(rx_cdr_control, channel) << 5);
		_lr4_301_rxcdr_m37046_i2c_write_byte(M3704X_MODE_CH(channel),tmp);
	}
}

//MSA Page3 byte234 and byte235 for tx eq
void LR4_301_set_tx_eq(uint8_t channel, uint8_t value)
{
	uint8_t eq_table[11] = {0,0,1,2,2,3,4,4,4,6,7};

	if(value != 0xff)
	{
		value = (eq_table[value & 0x0f]) << 4;
	}
	else
	{
		value = 0x50;
	}

	//tx channel reverse
	channel = 3 - channel;

	_lr4_301_txcdr_m37049_i2c_write_byte(M3704X_SLA_CH(channel),value);
}

//MSA Page3 byte236 and byte237 for rx emphasis
void LR4_301_set_rx_emphasis(uint8_t channel, uint8_t value)
{
	uint8_t de_emph[8] = {0x00,0x03,0x05,0x07,0x09,0x0b,0x0d,0x0f};

	value = (de_emph[value & 0x07]) << 4;
	_lr4_301_rxcdr_m37046_i2c_write_byte(M3704X_OUTPUT_DEEMPH_CH(channel),value);
}

//MSA Page3 byte238 and byte239 rx amplitude
void LR4_301_set_rx_amplitude(uint8_t channel, uint8_t value)
{
	uint8_t tmp = 0;
	uint8_t amplitude[4] = {0x00,0x0f,0x1e,0x3c};

	value = amplitude[value & 0x03];

	_lr4_301_rxcdr_m37046_i2c_read_byte(M3704X_OUTPUT_SWING_CH(channel),&tmp);
	tmp = (value & 0x3f)|(tmp & 0xc0);
	_lr4_301_rxcdr_m37046_i2c_write_byte(M3704X_OUTPUT_SWING_CH(channel),tmp);
}

//MSA Page3 byte240 sq disable
void LR4_301_set_cdr_sq_disable(uint8_t value)
{
	uint8_t tmp = 0,rx_sq = 0, tx_sq = 0, channel;

	//tx channel reverse
	tx_sq  = REVERSE_LSB_4BITS(value & 0x0F);
	rx_sq  = (value & 0xF0) >> 4;

	for(channel = 0;channel < 4 ;channel++)
	{
		_lr4_301_rxcdr_m37046_i2c_read_byte(M3704X_OUTPUT_SWING_CH(channel),&tmp);
		tmp = CLEAR_BIT(tmp, 7) | (GET_BIT(rx_sq, channel) << 7);
		_lr4_301_rxcdr_m37046_i2c_write_byte(M3704X_OUTPUT_SWING_CH(channel),tmp);

		_lr4_301_txcdr_m37049_i2c_read_byte(M3704X_OUTPUT_SWING_CH(channel),&tmp);
		tmp = CLEAR_BIT(tmp, 7) | (GET_BIT(tx_sq, channel) << 7);
		_lr4_301_txcdr_m37049_i2c_write_byte(M3704X_OUTPUT_SWING_CH(channel),tmp);
	}
}

//MSA Page3 byte241 for rx output disable
void  LR4_301_set_rx_output_disable(uint8_t value)
{
	uint8_t tmp = 0, rx_output_disable = 0, channel = 0;

	rx_output_disable = (value & 0xF0) >> 4;

	for(channel = 0; channel < 4; channel++)
	{
		_lr4_301_rxcdr_m37046_i2c_read_byte(M3704X_MODE_CH(channel),&tmp);
		tmp = CLEAR_BIT(tmp, 7) | (GET_BIT(rx_output_disable, channel) << 7);
		_lr4_301_rxcdr_m37046_i2c_write_byte(M3704X_MODE_CH(channel),tmp);
	}

}

//MSA Page3 byte241 for tx adapt eq
void  LR4_301_set_tx_adapt_eq(uint8_t value1, uint8_t value2,uint8_t value3)
{
	uint8_t tmp = 0, tx_adapt_eq = 0,channel;

	tx_adapt_eq = (value1 & 0x0F);

	for(channel = 0; channel < MSA_CHANNEL_NUM; channel ++)
	{
		if(GET_BIT(tx_adapt_eq, channel) == 0x1)
		{
			LR4_301_set_tx_eq(channel,0xff);
		}
		else
		{
			if(channel >= 0x0 && channel < 0x2)
			{
				tmp = ((value2) >> (4 * ((channel + 1) % 2)))& 0x0f;
			}
			else if(channel >= 0x2 && channel < MSA_CHANNEL_NUM)
			{
				tmp = ((value3) >> (4 * ((channel + 1) % 2)))& 0x0f;
			}
			LR4_301_set_tx_eq(channel,tmp& 0x0f);
		}
	}
}
#endif
