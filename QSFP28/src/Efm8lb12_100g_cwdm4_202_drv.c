#include <math.h>
#include "Efm8lb12_adc.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_smbus.h"
#include "Efm8lb12_common.h"
#include "Efm8lb12_msa_pin.h"
#include "Efm8lb12_100g_com_drv.h"

#ifdef EFM8LB12E64_100G_SFP_CWDM4_202

#ifdef HW_VER_AM1_202

static void cwdm4_201_port0_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;
	//1.set port default value
	P0 = P0_B0__HIGH | P0_B1__LOW | P0_B2__HIGH | P0_B3__HIGH | P0_B4__HIGH | P0_B5__HIGH | P0_B6__HIGH | P0_B7__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P0MDIN =  P0MDIN_B0__DIGITAL   					    //not used
			| P0MDIN_B1__DIGITAL 						//set p0.1 digital mode  is used for TX circuit voltage enable
			| P0MDIN_B2__DIGITAL                        //not used
			| P0MDIN_B3__DIGITAL						//not used
			| P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL   //set p0.4 and p0.5 digital mode is used for uart0
			| P0MDIN_B6__DIGITAL 					    //not used
			| P0MDIN_B7__DIGITAL;					    //set p0.7 digital mode is used for QSFP  interrupt output


	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P0MDOUT = P0MDOUT_B0__OPEN_DRAIN    					    //not used
			| P0MDOUT_B1__PUSH_PULL 							//set p0.1 digital mode  is used for TX circuit voltage enable
			| P0MDOUT_B2__OPEN_DRAIN							//not used
			| P0MDOUT_B3__OPEN_DRAIN	    					//not used
			| P0MDOUT_B4__OPEN_DRAIN | P0MDOUT_B5__OPEN_DRAIN   //set p0.4 push pull and p0.5 open drain is uesd for uart0
			| P0MDOUT_B6__OPEN_DRAIN							//not used
			| P0MDOUT_B7__OPEN_DRAIN;						    //set p0.7 open drain is used for QSFP interrupt output


	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P0SKIP = P0SKIP_B0__SKIPPED					//because P1.2 and p1.3 are used to Smbus,so P0.0 ~ P0.7 must be skipped
			| P0SKIP_B1__SKIPPED| P0SKIP_B2__SKIPPED   //not used
			| P0SKIP_B3__SKIPPED					   //not used
			| P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
			| P0SKIP_B6__SKIPPED 				//not used
			| P0SKIP_B7__SKIPPED;

	//5. set p0MASK
	P0MASK = P0MASK_B0__IGNORED | P0MASK_B1__IGNORED | P0MASK_B2__IGNORED
			| P0MASK_B3__IGNORED | P0MASK_B4__IGNORED | P0MASK_B5__IGNORED
			| P0MASK_B6__IGNORED | P0MASK_B7__IGNORED;

	//6. set P0MAT
	P0MAT = P0MAT_B0__HIGH | P0MAT_B1__HIGH | P0MAT_B2__HIGH | P0MAT_B3__HIGH
			| P0MAT_B4__HIGH | P0MAT_B5__HIGH | P0MAT_B6__HIGH | P0MAT_B7__HIGH;

	SFRPAGE = SFRPAGE_SAVE;

}

static void cwdm4_201_port1_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;
	//1.set port default value
	P1 = P1_B0__HIGH | P1_B1__LOW | P1_B2__HIGH | P1_B3__HIGH | P1_B4__LOW | P1_B5__HIGH | P1_B6__HIGH | P1_B7__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P1MDIN =  P1MDIN_B0__ANALOG  					   //set p1.0 analog mode is used for mointor the 3.3V module input voltage
			| P1MDIN_B1__DIGITAL 					   //set p1.1 digital mode is used for TX close signal
			| P1MDIN_B2__DIGITAL | P1MDIN_B3__DIGITAL  //set p1.2 and p1.3 digital mode is used for i2c master mode

			| P1MDIN_B4__DIGITAL     			  //txcdr reset pin
			| P1MDIN_B5__DIGITAL				  //txcdr los pin
			| P1MDIN_B6__DIGITAL    			  //txcdr fault pin
			| P1MDIN_B7__DIGITAL;                 //not used

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN    			  			//because the p1.0 are set analog mode£¬so this bit can be not set
			| P1MDOUT_B1__PUSH_PULL					  			//set p1.1 push pull mode is used for TX close signal
			| P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN	//because p1.2 and p1.3 are used for i2c master mode,so the two bits must be open drain

			| P1MDOUT_B4__PUSH_PULL             //txcdr reset pin
			| P1MDOUT_B5__OPEN_DRAIN			//because p1.5 is input pin,so set p1.5 default value(open drain mode) is used for tx unlock alarm
			| P1MDOUT_B6__OPEN_DRAIN			//because p1.6 is input pin,so set p1.6 default value(open drain mode)is used for tx lose efficacy alarm
			| P1MDOUT_B7__OPEN_DRAIN;           //not used

	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P1SKIP = P1SKIP_B0__SKIPPED 			//because p1.0 is set analog mode and p1.2 and p1.3 are used to Smbus ,so this bit must be skipped are used for mointor the 3.3V module input voltage
			| P1SKIP_B1__SKIPPED			//set p1.1 skip in order to p1.2 and p1.3 are used for i2c master mode
			| P1SKIP_B2__NOT_SKIPPED | P1SKIP_B3__NOT_SKIPPED

			//because P2.0 and p2.1 are used for i2c slave mode,so P1.0 ~ P1.7 must be skipped
			| P1SKIP_B4__SKIPPED | P1SKIP_B5__SKIPPED| P1SKIP_B6__SKIPPED | P1SKIP_B7__SKIPPED;

	//5. set p1MASK
	P1MASK = P1MASK_B0__IGNORED | P1MASK_B1__IGNORED | P1MASK_B2__IGNORED
			| P1MASK_B3__IGNORED | P1MASK_B4__IGNORED | P1MASK_B5__IGNORED
			| P1MASK_B6__IGNORED | P1MASK_B7__IGNORED;

	//6. set P1MAT
	P1MAT = P1MAT_B0__HIGH | P1MAT_B1__HIGH | P1MAT_B2__HIGH | P1MAT_B3__HIGH
			| P1MAT_B4__HIGH | P1MAT_B5__HIGH | P1MAT_B6__HIGH | P1MAT_B7__HIGH;

	SFRPAGE = SFRPAGE_SAVE;
}

static void cwdm4_201_port2_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x20;
	//1.set port default value
	P2 = P2_B0__HIGH | P2_B1__HIGH | P2_B2__LOW | P2_B3__HIGH | P2_B4__HIGH | P2_B5__HIGH | P2_B6__HIGH ;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P2MDIN = P2MDIN_B0__DIGITAL | P2MDIN_B1__DIGITAL	 //set p2.0 and p2.1 digital mode are used for i2c slave mode

			| P2MDIN_B2__DIGITAL   //not used

			//p2.3 ~ p2.6  analog mode are used for RSSI0 ~ RSSI3
			| P2MDIN_B3__ANALOG | P2MDIN_B4__ANALOG | P2MDIN_B5__ANALOG | P2MDIN_B6__ANALOG ;

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).

	P2MDOUT = P2MDOUT_B0__OPEN_DRAIN | P2MDOUT_B1__OPEN_DRAIN //set p2.0 and p2.1 open drain mode are used for i2c slave mode

			| P2MDOUT_B2__OPEN_DRAIN	 //not used

			//because the p2.3 ~ p2.6 are set analog mode£¬so these bits can be not set
			| P2MDOUT_B3__OPEN_DRAIN | P2MDOUT_B4__OPEN_DRAIN | P2MDOUT_B5__OPEN_DRAIN  | P2MDOUT_B6__OPEN_DRAIN;

	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P2SKIP = P2SKIP_B0__SKIPPED | P2SKIP_B1__SKIPPED   		//set p2.0 and p2.1 skipped for i2c slave mode
			| P2SKIP_B2__NOT_SKIPPED

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

static void cwdm4_201_port3_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x20;
	//1.set port default value
	P3 = P3_B0__HIGH | P3_B1__HIGH | P3_B2__HIGH | P3_B3__HIGH | P3_B4__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P3MDIN = P3MDIN_B0__DIGITAL    //set p3.0 digital mode is used for qsfp low power mode

			| P3MDIN_B1__DIGITAL   //set p3.1 digital mode is used for DAC

			| P3MDIN_B2__DIGITAL   //set p3.2 digital mode is used for i2c slave mode chip selection signal

			| P3MDIN_B3__DIGITAL   //set p3.3 digital mode is used for qsfp module reset

			| P3MDIN_B4__DIGITAL;   //not used

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P3MDOUT = P3MDOUT_B0__OPEN_DRAIN   //because p3.0 is input pin,so set p3.0 default value(open drain mode) is used for lpmode pin

			| P3MDOUT_B1__OPEN_DRAIN   //because p3.1 is not used,so set p3.1 default value(open drain mode)

			| P3MDOUT_B2__OPEN_DRAIN   //because p3.2 is input pin and it and uart pin multiplexing,so this pin must be open drain when use uart

			| P3MDOUT_B3__OPEN_DRAIN   //because p3.3 is input pin,so set p3.3 default value(open drain mode)  is used for qsfp module reset

			| P3MDOUT_B4__OPEN_DRAIN;   //because p3.4 is not used,so set p3.4 default value(open drain mode)

	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).

	//p3 do't have p3SKIP registers

	//5. set p2MASK
	//p3 do't have P3MASK registers

	//6. set P2MAT
	//p3 do't have P3MAT registers
	SFRPAGE = SFRPAGE_SAVE;
}
#endif

#if defined(HW_VER_AM2_202)

static void cwdm4_202_port0_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x00;
	//1.set port default value  // 
	P0 = P0_B0__HIGH | P0_B1__HIGH | P0_B2__HIGH | P0_B3__HIGH | P0_B4__HIGH | P0_B5__HIGH | P0_B6__HIGH | P0_B7__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P0MDIN =  P0MDIN_B0__DIGITAL   					    //not used
			| P0MDIN_B1__DIGITAL 						// set p0.1 digital mode is used for qsfp low power mode
			| P0MDIN_B2__DIGITAL                        //not used
			| P0MDIN_B3__DIGITAL						//not used
			| P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL   //set p0.4 and p0.5 digital mode is used for uart0
			| P0MDIN_B6__DIGITAL 					    //not used
			| P0MDIN_B7__DIGITAL;					    //set p0.7 digital mode is used for QSFP  interrupt output


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
			| P0MASK_B3__IGNORED | P0MASK_B4__IGNORED | P0MASK_B5__IGNORED
			| P0MASK_B6__COMPARED | P0MASK_B7__COMPARED;

	//6. set P0MAT
	P0MAT = P0MAT_B0__HIGH | P0MAT_B1__HIGH | P0MAT_B2__HIGH | P0MAT_B3__HIGH
			| P0MAT_B4__HIGH | P0MAT_B5__HIGH | P0MAT_B6__HIGH | P0MAT_B7__HIGH;

	SFRPAGE = SFRPAGE_SAVE;

}

static void cwdm4_202_port1_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;
	//1.set port default value
	P1 = P1_B0__HIGH | P1_B1__LOW | P1_B2__HIGH | P1_B3__HIGH | P1_B4__HIGH | P1_B5__HIGH | P1_B6__HIGH | P1_B7__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P1MDIN =  P1MDIN_B0__DIGITAL  					   //set p1.0 digital mode is used for modesel mcu
			| P1MDIN_B1__DIGITAL 					   //set p1.1 digital mode is used for TX close signal
			| P1MDIN_B2__DIGITAL | P1MDIN_B3__DIGITAL  //set p1.2 and p1.3 digital mode is used for i2c master mode

			| P1MDIN_B4__DIGITAL     			  //p1.4 txcdr reset pin
			| P1MDIN_B5__DIGITAL				  //not used
			| P1MDIN_B6__DIGITAL    			  //not used
			| P1MDIN_B7__ANALOG;                 //set p1.7 analog mode  used for 3V3MON  

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN    			  			//because the p1.0 are set analog mode£¬so this bit can be not set
			| P1MDOUT_B1__PUSH_PULL					  			//set p1.1 push pull mode is used for TX close signal
			| P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN	//because p1.2 and p1.3 are used for i2c master mode,so the two bits must be open drain

			| P1MDOUT_B4__PUSH_PULL             //p1.4 txcdr reset pin
			| P1MDOUT_B5__OPEN_DRAIN			//not used 
			| P1MDOUT_B6__OPEN_DRAIN			//not used
			| P1MDOUT_B7__OPEN_DRAIN;           // p1.7 	used for 3V3MON  

	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P1SKIP = P1SKIP_B0__SKIPPED 			//because p1.0 is set analog mode and p1.2 and p1.3 are used to Smbus ,so this bit must be skipped are used for mointor the 3.3V module input voltage
			| P1SKIP_B1__SKIPPED			//set p1.1 skip in order to p1.2 and p1.3 are used for i2c master mode
			| P1SKIP_B2__NOT_SKIPPED | P1SKIP_B3__NOT_SKIPPED

			//because P2.0 and p2.1 are used for i2c slave mode,so P1.0 ~ P1.7 must be skipped
			| P1SKIP_B4__SKIPPED | P1SKIP_B5__SKIPPED| P1SKIP_B6__SKIPPED | P1SKIP_B7__SKIPPED;

	//5. set p1MASK
	P1MASK = P1MASK_B0__IGNORED | P1MASK_B1__IGNORED | P1MASK_B2__IGNORED
			| P1MASK_B3__IGNORED | P1MASK_B4__IGNORED | P1MASK_B5__IGNORED
			| P1MASK_B6__IGNORED | P1MASK_B7__IGNORED;

	//6. set P1MAT
	P1MAT = P1MAT_B0__HIGH | P1MAT_B1__HIGH | P1MAT_B2__HIGH | P1MAT_B3__HIGH
			| P1MAT_B4__HIGH | P1MAT_B5__HIGH | P1MAT_B6__HIGH | P1MAT_B7__HIGH;

	SFRPAGE = SFRPAGE_SAVE;
}


static void cwdm4_202_port2_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x20;
	//1.set port default value
	P2 = P2_B0__HIGH | P2_B1__HIGH | P2_B2__HIGH | P2_B3__HIGH | P2_B4__HIGH | P2_B5__HIGH | P2_B6__HIGH ;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P2MDIN = P2MDIN_B0__DIGITAL | P2MDIN_B1__DIGITAL	 //set p2.0 and p2.1 digital mode are used for i2c slave mode
			| P2MDIN_B2__DIGITAL   //not used
			//p2.3 ~ p2.6  analog mode are used for RSSI0 ~ RSSI3
			| P2MDIN_B3__ANALOG | P2MDIN_B4__ANALOG | P2MDIN_B5__ANALOG | P2MDIN_B6__ANALOG ;

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).

	P2MDOUT = P2MDOUT_B0__OPEN_DRAIN | P2MDOUT_B1__OPEN_DRAIN //set p2.0 and p2.1 open drain mode are used for i2c slave mode
			| P2MDOUT_B2__OPEN_DRAIN	 //not used
			//because the p2.3 ~ p2.6 are set analog mode£¬so these bits can be not set
			| P2MDOUT_B3__OPEN_DRAIN | P2MDOUT_B4__OPEN_DRAIN | P2MDOUT_B5__OPEN_DRAIN  | P2MDOUT_B6__OPEN_DRAIN;

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

static void cwdm4_202_port3_init()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x20;
	//1.set port default value
	P3 = P3_B0__HIGH | P3_B1__HIGH | P3_B2__HIGH | P3_B3__HIGH | P3_B4__HIGH;

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

#endif

static uint8_t cwdm4_202_rxcdr_m37046_i2c_read_byte(uint8_t OffsetAddr, uint8_t *Data)
{
	return smb_read_byte(CWDM4_202_RXCDR_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t cwdm4_202_rxcdr_m37046_i2c_write_byte(uint8_t OffsetAddr,uint8_t Data)
{
	return smb_write_byte(CWDM4_202_RXCDR_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t cwdm4_202_txcdr_gn2105_i2c_read_byte(uint16_t OffsetAddr, uint8_t *Data)
{
	return smb_read_byte(CWDM4_202_TXCDR_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t cwdm4_202_txcdr_gn2105_i2c_write_byte(uint16_t OffsetAddr, uint8_t Data)
{
	return smb_write_byte(CWDM4_202_TXCDR_BASE_ADDR, OffsetAddr, Data);
}

#if defined(HW_VER_AM1_202)

static void cwdm4_202_txcdr_gn2105_reset()
{
	Soft_timer_delay_ms(100);
	TX_RESET = 1;
	Soft_timer_delay_ms(10);	/*
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x471,0x00);  //PD_ALL    pown on
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x067,0x00);  //L0_PD_LANE
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x167,0x00);  //L1_PD_LANE
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x267,0x00);  //L2_PD_LANE
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x367,0x00);  //L3_PD_LANE

	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x000,0x00);  //L0_CDR_BYPASS_CTRL_REG_0
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x100,0x00);  //L1_CDR_BYPASS_CTRL_REG_0
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x200,0x00);  //L2_CDR_BYPASS_CTRL_REG_0
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x300,0x00);  //L3_CDR_BYPASS_CTRL_REG_0
	*/
}
#endif

#if defined(HW_VER_AM2_202)  // for GN2105B

static void cwdm4_202_txcdr_gn2105_reset()
{
	Soft_timer_delay_ms(100);
	TX_RESET = 1;
	Soft_timer_delay_ms(10);
/*
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x472,0x00);  //PD_ALL    power on
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x078,0x00);  //L0_PD_LANE
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x178,0x00);  //L1_PD_LANE
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x278,0x00);  //L2_PD_LANE
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x378,0x00);  //L3_PD_LANE

	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x000,0x00);  //L0_CDR_BYPASS_CTRL_REG_0
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x100,0x00);  //L1_CDR_BYPASS_CTRL_REG_0
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x200,0x00);  //L2_CDR_BYPASS_CTRL_REG_0
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x300,0x00);  //L3_CDR_BYPASS_CTRL_REG_0
	return ;*/
}
#endif

static void cwdm4_202_txcdr_gn2105_init()
{
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x472,0x00);	//PD_ALL	power on
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x078,0x00);	//L0_PD_LANE
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x178,0x00);	//L1_PD_LANE
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x278,0x00);	//L2_PD_LANE
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x378,0x00);	//L3_PD_LANE

	//cwdm4_206_txcdr_gn2105_i2c_write_byte(0x000,0x00);  //L0_CDR_BYPASS_CTRL_REG_0
	//cwdm4_206_txcdr_gn2105_i2c_write_byte(0x100,0x00);  //L1_CDR_BYPASS_CTRL_REG_0
	//cwdm4_206_txcdr_gn2105_i2c_write_byte(0x200,0x00);  //L2_CDR_BYPASS_CTRL_REG_0
	//cwdm4_206_txcdr_gn2105_i2c_write_byte(0x300,0x00);  //L3_CDR_BYPASS_CTRL_REG_0

	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x448,0x00);	
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x449,0x00);	  //ADC_PARAM_MON_CTRL_RESET
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x473,0x00);	  //PD_SENSE_REG_0
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x433,0x00);	  //TXDSBL_SOFT

	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x078,0x00);	
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x079,0x01); 

	//cwdm4_206_txcdr_gn2105_i2c_write_byte(0x013,0x00); 
	//cwdm4_206_txcdr_gn2105_i2c_write_byte(0x014,0x00); 
	//cwdm4_206_txcdr_gn2105_i2c_write_byte(0x015,0x00);	 //L*_DRV_PD_CTRL_REG_0
	//cwdm4_206_txcdr_gn2105_i2c_write_byte(0x115,0x00); 
	//cwdm4_206_txcdr_gn2105_i2c_write_byte(0x215,0x00); 
	//cwdm4_206_txcdr_gn2105_i2c_write_byte(0x315,0x00); 

	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x84,0x01);  // bias enable
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x184,0x01);	
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x284,0x01);	
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x384,0x01);	
	
	//cwdm4_206_txcdr_gn2105_i2c_write_byte(0x40a,0x39);  
	//cwdm4_206_txcdr_gn2105_i2c_write_byte(0x545,0x80);  
	//cwdm4_206_txcdr_gn2105_i2c_write_byte(0x546,0xff);  
	
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x01c,0x02);	 //L*_DRV_IMON_CTRL
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x11c,0x02);
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x21c,0x02);
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x31c,0x02);

	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x01b,0x00);	 //L*L*_DRV_PD_MON_REG_0
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x11b,0x00);
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x21b,0x00);
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x31b,0x00);


	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x082,0x00);	// bias override
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x083,0x00);	
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x182,0x00);	
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x183,0x00); 
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x282,0x00);	
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x283,0x00); 
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x382,0x00);	
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x383,0x00); 

}

static void cwdm4_202_rxcdr_m37046_reset()
{
	cwdm4_202_rxcdr_m37046_i2c_write_byte(0x02, 0xaa);
	Soft_timer_delay_ms(100);
	cwdm4_202_rxcdr_m37046_i2c_write_byte(0x02, 0x00);
}

#if defined(HW_VER_AM1_202)

static float cwdm4_202_get_gn2105_bias(uint8_t channel)
{
	uint8_t lane_low = 0, lane_high = 0;
	uint16_t  channel_reg[] = {0x071, 0x171, 0x271, 0x371};
	float Ibias = 0;
	cwdm4_202_txcdr_gn2105_i2c_read_byte(channel_reg[channel], &lane_low);
	cwdm4_202_txcdr_gn2105_i2c_read_byte(channel_reg[channel + 1], &lane_high); //bit9 8

	Ibias  = lane_low | ((lane_high & 0x03) << 8);
	return Ibias;
}

#endif

#if defined(HW_VER_AM2_202)

static float cwdm4_202_get_gn2105_bias(uint8_t channel)
{
	uint8_t lane_low, lane_high, RPP = 0;
	uint16_t  channel_reg[] = {0x070, 0x170, 0x270, 0x370};
	float Ibias = 0;
	cwdm4_202_txcdr_gn2105_i2c_read_byte(0x465, &RPP);
	cwdm4_202_txcdr_gn2105_i2c_read_byte(channel_reg[channel], &lane_low);
	cwdm4_202_txcdr_gn2105_i2c_read_byte(channel_reg[channel] + 1, &lane_high); //bit9 8
	Ibias  = lane_low | ((lane_high & 0x0F) << 8);
	Ibias = Ibias / (4095 * 8.774 * (1.145845 - 0.00463 * RPP))*1000*10;   //0.1mA
	return Ibias;
}

static uint16_t cwdm4_202_ddmi_get_txmon(uint8_t channel)
{
	uint8_t txmon_lo, txmon_hi,  RPP = 0;
	uint16_t  channel_reg[] = {0x076, 0x176, 0x276, 0x376};
	float txmon = 0;
	cwdm4_202_txcdr_gn2105_i2c_read_byte(0x465, &RPP);
	cwdm4_202_txcdr_gn2105_i2c_read_byte(channel_reg[channel], &txmon_lo);
	cwdm4_202_txcdr_gn2105_i2c_read_byte(channel_reg[channel] + 1, &txmon_hi); //bit9 8
	txmon  = txmon_lo | ((txmon_hi & 0x03) << 8);
	txmon = (txmon / (1023 * 394.1 * (1.145845 - 0.00463 * RPP)))*1000*10;   //0.1mA
	return txmon;
}

#endif

static float cwdm4_202_ddmi_get_rssi(uint8_t channel)
{
	uint16_t rssi_mv = 0;
	rssi_mv =  ADC0_get_rssi(channel);
	return (float)rssi_mv / (float)RSSI_SERIES_RES * 1000.00;  //uA}
}

static void  cwdm4_202_drv_check_laser_ch(SFP_DRV_LASER_CFG_T *cfg)
{
	if(cfg->bias > 40)
	{
		cfg->bias = 40;
	}
	if(cfg->mod > 80)
	{
		cfg->mod = 80;
	}
}

static int8_t cwdm4_202_crossing_dac_tranfer_cross_lut(uint8_t cross)
{
	uint8_t value = 0;

	if(cross >= 0x0 && cross <= 0x7)
	{
		value = cross;
	}
	else if(cross >= 0x8 && cross <= 0x15)
	{
		value = cross - 16;
	}
	return (int8_t)value;
}

static uint8_t cwdm4_202_crossing_value_handle(int16_t crossing)
{
	uint8_t value = 0;

	if(crossing >= 0x0 && crossing <= 7)
	{
		value = crossing;
	}
	else if(crossing > 7)
	{
		value = 7;  //max value
	}
	else if(crossing < -9)
	{
		value = 8;   //min value
	}
	else
	{
		value = crossing + 16;
	}
	return value;
}


//***********************************************public function***************************************8

void CWDM4_202_chip_reset(void)
{
	cwdm4_202_txcdr_gn2105_reset();
	cwdm4_202_rxcdr_m37046_reset();
}

void CWDM4_202_port_init(void)// mcu_port_init
{
#if defined(HW_VER_AM1_202)

	cwdm4_201_port0_init();
	cwdm4_201_port1_init();
	cwdm4_201_port2_init();
	cwdm4_201_port3_init();		
		
#endif

#if defined(HW_VER_AM2_202)

	cwdm4_202_port0_init();
	cwdm4_202_port1_init();
	cwdm4_202_port2_init();
	cwdm4_202_port3_init();	
#endif
	//return ;
}

void CWDM4_202_chip_init(void)		// init
{
	CWDM4_202_chip_reset();
	cwdm4_202_txcdr_gn2105_init();
}

int16_t CWDM4_202_ddmi_get_temp(MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		DDMI_CALI_PARA_T *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		return (ddmi_cali->temp + ADC0_get_mcu_temp()); 		//MSA format uint: 1/256 C
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return  ((int16_t)ADC0_get_mcu_temp());  //MSA format uint:1/256ÉãÊÏ¶È
	}

	return GOS_FAIL;
}

uint16_t CWDM4_202_ddmi_get_vcc(MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		const DDMI_CALI_PARA_T *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		return (ddmi_cali->vcc + ADC0_get_vcc()); //uint:0.1mv
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return (uint16_t)(floor(0.5 + ADC0_get_vcc()));  //uint:mV
	}

	return GOS_FAIL;
}

uint16_t CWDM4_202_ddmi_get_bias(uint8_t channel, MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		DDMI_CALI_PARA_T *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		const MSA_LUT_T  *laser_stat = MSA_p4_get_laser_state();
		return ((ddmi_cali->current[0].bias.bias_offset1 + laser_stat->ch[channel].stat.txbias) >> 1);
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return (uint16_t)(cwdm4_202_get_gn2105_bias(channel));
	}
	return GOS_FAIL;
}

uint16_t CWDM4_202_ddmi_get_rx_power(uint8_t channel, MSA_DRV_VAL_TYPE type)
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
		return  (uint16_t)cwdm4_202_ddmi_get_rssi(channel);  //uint:ua
	}

	return GOS_FAIL;
}

uint16_t CWDM4_202_ddmi_get_tx_power(uint8_t channel, MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		float bias_rt_ma = 0.0, bias_ct_ma = 0.0, bias_N = 0.0;
		float txpower_result = 0.0, txpower1 = 0.0, txpower2 = 0.0, bias1 = 0.0, bias2 = 0.0;
		const DDMI_CALI_PARA_T    *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		const MSA_LUT_T 		  *laser_stat = MSA_p4_get_laser_state();
		const SFP_DRV_LASER_CFG_T *lo_cfg = (SFP_DRV_LASER_CFG_T *)MSA_p67_get_drv_lut();
		const SFP_DRV_LASER_CFG_T *m_cfg  = (lo_cfg + MSA_CHANNEL_NUM);

		bias_rt_ma = 4.0 + 0.1 * (float)((m_cfg + channel)->bias);  					//bias dac value tranfer to bias current

		bias_ct_ma = 4.0 + 0.1 * (float)laser_stat->ch[channel].config.bias;    //bias dac value tranfer to bias current

		//laser_stat->ch[channel].stat.txpwr uint:0.1mA
		bias_N = bias_rt_ma - (bias_ct_ma - (float)laser_stat->ch[channel].stat.txpwr * 0.1);

		//ddmi_cali->tx_rx_pwr[channel].txpower.txpower uint:0.1uw
		txpower1 = (float)ddmi_cali->current[channel].txpower.txpower1 * 0.1 / 1000.0; //uint:mW
		txpower2 = (float)ddmi_cali->current[channel].txpower.txpower2 * 0.1 / 1000.0; //uint:mW

		//ddmi_cali->tx_rx_pwr[channel].txpower.bias uint:0.1mA
		bias1 = (float)ddmi_cali->current[channel].txpower.bias1 * 0.1; //uint:mA
		bias2 = (float)ddmi_cali->current[channel].txpower.bias2 * 0.1; //uint:mA

		if(bias1 != bias2)
		{
			txpower_result = txpower1 + (txpower1 - txpower2)/(bias1 - bias2) * (bias_N - bias1);  //uint:mW
		}

		CHECK_POWER_MIN(txpower_result);
		CHECK_POWER_MAX(txpower_result);
		return (uint16_t)(txpower_result * 1000.0 * 10.0); //MSA format uint:0.1uW

	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return (uint16_t)(cwdm4_202_ddmi_get_txmon(channel)); //uint:0.1mA
	}
	return GOS_FAIL;
}

#if defined(HW_VER_AM1_202)

void CWDM4_202_drv_adjust_laser_ch(uint8_t ch, SFP_DRV_LASER_CFG_T *cfg)
{
	static code uint16_t bias_reg[]	= {0x082, 0x182, 0x282, 0x382};
	static code uint16_t mod_reg[]	= {0x085, 0x185, 0x285, 0x385};
	static code uint16_t cross_reg[] = {0x023, 0x123, 0x223, 0x323};

	cfg->cross = cwdm4_202_crossing_value_handle(cfg->cross);

	cwdm4_202_txcdr_gn2105_i2c_write_byte(bias_reg[ch], (uint8_t)(cfg->bias & 0xFF) );
	cwdm4_202_txcdr_gn2105_i2c_write_byte(bias_reg[ch] + 1, (uint8_t)((cfg->bias >> 8) & 0x03) );
	cwdm4_202_txcdr_gn2105_i2c_write_byte(mod_reg[ch], (uint8_t)(cfg->mod & 0xFF));
	cwdm4_202_txcdr_gn2105_i2c_write_byte(mod_reg[ch] + 1, (uint8_t)((cfg->mod >> 8) & 0x03));
	cwdm4_202_txcdr_gn2105_i2c_write_byte(cross_reg[ch], (uint8_t)(cfg->cross & 0x1F));
	cwdm4_202_txcdr_gn2105_i2c_write_byte(cross_reg[ch] + 1, (uint8_t)((cfg->cross >> 8) & 0x1F));

}

void CWDM4_202_drv_get_laser_full_state(uint8_t channel, MSA_LUT_T *lut)
{
	static code uint16_t bias_reg[]	= {0x082, 0x182, 0x282, 0x382};
	static code uint16_t mod_reg[]	= {0x085, 0x185, 0x285, 0x385};
	static code uint16_t cross_reg[] = {0x023, 0x123, 0x223, 0x323};

	uint8_t bias_low, mod_low, cross_low;
	uint8_t bias_high, mod_high, cross_high;

	lut->temp = DDM_GET_TEMP(MSA_DRV_VAL_HW);

	cwdm4_202_txcdr_gn2105_i2c_read_byte(bias_reg[channel],  &bias_low);
	cwdm4_202_txcdr_gn2105_i2c_read_byte(bias_reg[channel] + 1,  &bias_high);
	cwdm4_202_txcdr_gn2105_i2c_read_byte(mod_reg[channel],	 &mod_low);
	cwdm4_202_txcdr_gn2105_i2c_read_byte(mod_reg[channel] + 1,	 &mod_high);
	cwdm4_202_txcdr_gn2105_i2c_read_byte(cross_reg[channel], &cross_low);
	cwdm4_202_txcdr_gn2105_i2c_read_byte(cross_reg[channel] + 1, &cross_high);

	lut->ch[channel].config.bias = (uint16_t)(bias_low | (bias_high & 0x03) << 8);
	lut->ch[channel].config.mod  = (uint16_t)(mod_low | (mod_high & 0x03) << 8);
	//lut->ch[channel].config.cross = (uint16_t)((cross_low & 0x1F) | (cross_high & 0x1F) << 8);
	lut->ch[channel].config.cross = cwdm4_202_crossing_dac_tranfer_cross_lut((cross_low & 0x1F) | (cross_high & 0x1F) << 8);
}

#endif

#if defined(HW_VER_AM2_202) // for GN2105B

void CWDM4_202_drv_adjust_laser_ch(uint8_t ch, SFP_DRV_LASER_CFG_T *cfg)
{
	static code uint16_t bias_reg[]	= {0x082, 0x182, 0x282, 0x382};
	static code uint16_t mod_reg[]	= {0x085, 0x185, 0x285, 0x385};
	static code uint16_t cross_reg[] = {0x023, 0x123, 0x223, 0x323};

	cfg->cross = cwdm4_202_crossing_value_handle(cfg->cross);

	cwdm4_202_txcdr_gn2105_i2c_write_byte(bias_reg[ch], (uint8_t)(cfg->bias & 0xFF) );
	cwdm4_202_txcdr_gn2105_i2c_write_byte(bias_reg[ch + 1], (uint8_t)((cfg->bias >> 8) & 0x03) );
	cwdm4_202_txcdr_gn2105_i2c_write_byte(mod_reg[ch], (uint8_t)(cfg->mod & 0xFF));
	cwdm4_202_txcdr_gn2105_i2c_write_byte(mod_reg[ch + 1], (uint8_t)((cfg->mod >> 8) & 0x03));
	cwdm4_202_txcdr_gn2105_i2c_write_byte(cross_reg[ch], (uint8_t)(cfg->cross & 0x1F));
	cwdm4_202_txcdr_gn2105_i2c_write_byte(cross_reg[ch + 1], (uint8_t)((cfg->cross >> 8) & 0x1F));

	return ;
}

void CWDM4_202_drv_get_laser_full_state(uint8_t channel, MSA_LUT_T *lut)
{
	static code uint16_t bias_reg[]	= {0x082, 0x182, 0x282, 0x382};
	static code uint16_t mod_reg[]	= {0x085, 0x185, 0x285, 0x385};
	static code uint16_t cross_reg[] = {0x023, 0x123, 0x223, 0x323};

	uint8_t bias_low, mod_low, cross_low;
	uint8_t bias_high, mod_high, cross_high;

	lut->temp = DDM_GET_TEMP(MSA_DRV_VAL_HW);

	cwdm4_202_txcdr_gn2105_i2c_read_byte(bias_reg[channel],  &bias_low);
	cwdm4_202_txcdr_gn2105_i2c_read_byte(bias_reg[channel + 1],  &bias_high);
	cwdm4_202_txcdr_gn2105_i2c_read_byte(mod_reg[channel],	 &mod_low);
	cwdm4_202_txcdr_gn2105_i2c_read_byte(mod_reg[channel + 1],	 &mod_high);
	cwdm4_202_txcdr_gn2105_i2c_read_byte(cross_reg[channel], &cross_low);
	cwdm4_202_txcdr_gn2105_i2c_read_byte(cross_reg[channel + 1], &cross_high);

	lut->ch[channel].config.bias = (uint16_t)(bias_low | (bias_high & 0x03) << 8);
	lut->ch[channel].config.mod  = (uint16_t)(mod_low | (mod_high & 0x03) << 8);
//	lut->ch[channel].config.cross = (uint16_t)((cross_low & 0x1F) | (cross_high & 0x1F) << 8);
	lut->ch[channel].config.cross = cwdm4_202_crossing_dac_tranfer_cross_lut((cross_low & 0x1F) | (cross_high & 0x1F) << 8);
	return ;
}

#endif

void CWDM4_202_drv_ch_calc_cfg (int16_t cur_temp, uint8_t ch, const SFP_DRV_LASER_CFG_T *lut, SFP_DRV_LASER_CFG_T *ret)
{
	SFP_DRV_LASER_CFG_T *lo_cfg = (lut + ch);
	SFP_DRV_LASER_CFG_T *m_cfg	= (lo_cfg + MSA_CHANNEL_NUM);
	SFP_DRV_LASER_CFG_T *hi_cfg = (m_cfg + MSA_CHANNEL_NUM);

	ret->temp = cur_temp;
	if (cur_temp <= m_cfg->temp)
	{
		ret->bias = (uint16_t )(floor(0.5 + (double)lo_cfg->bias +
						((double)(cur_temp - lo_cfg->temp)) * ((double)(m_cfg->bias - lo_cfg->bias))
						/ ((double)(m_cfg->temp - lo_cfg->temp))));
		ret->mod = (uint16_t)(floor(0.5 + (double)lo_cfg->mod +
						((double)(cur_temp - lo_cfg->temp)) * ((double)(m_cfg->mod - lo_cfg->mod))
						/ ((double)(m_cfg->temp - lo_cfg->temp))));
		ret->cross = (int16_t)(floor(0.5 + (double)lo_cfg->cross +
						((double)(cur_temp - lo_cfg->temp)) * ((double)(m_cfg->cross - lo_cfg->cross))
						/ ((double)(m_cfg->temp - lo_cfg->temp))));
	}
	else
	{
		ret->bias = (uint16_t)(floor(0.5 + (double)m_cfg->bias * exp(
						((double)(cur_temp - m_cfg->temp))
						* log(((double)hi_cfg->bias)/((double)m_cfg->bias))
						/ ((double)(hi_cfg->temp - m_cfg->temp)))));
		ret->mod  = (uint16_t)(floor(0.5 + (double)m_cfg->mod * exp(
						((double)(cur_temp-m_cfg->temp))
						* log(((double)hi_cfg->mod)/((double)m_cfg->mod))
						/ ((double)(hi_cfg->temp - m_cfg->temp)))));
		ret->cross = (int16_t)(floor(0.5 + (double)m_cfg->cross +
						((double)(cur_temp - m_cfg->temp)) * ((double)(hi_cfg->cross - m_cfg->cross))
						/ ((double)(hi_cfg->temp - m_cfg->temp))));
		}
		cwdm4_202_drv_check_laser_ch(ret);
	return ;
}

void CWDM4_202_drv_poll(void)
{
	ADC0_poll();
	return ;
}

void CWDM4_202_set_low_power(uint8_t enable)
{
	if(enable)
	{
		IT01CF &= ~IT01CF_IN0PL__ACTIVE_HIGH;
		TX3V3_EN = 0;  //close tx3v3 in order to realize low power mode
	}
	else
	{
		MSA_set_low_power_mode(1);
		IT01CF |= IT01CF_IN0PL__ACTIVE_HIGH;
		TX3V3_EN = 1; //open tx3v3 in order to realize high power mode
	}

}

void CWDM4_202_set_tx_enable(uint8_t enale)
{
	uint8_t value = 0;
	cwdm4_202_txcdr_gn2105_i2c_read_byte(0x433,&value);
	value = CLEAR_LSB_4BITS(value)|CLEAR_MSB_4BITS(enale);
	cwdm4_202_txcdr_gn2105_i2c_write_byte(0x433, value);
	cwdm4_202_txcdr_gn2105_i2c_read_byte(0x433,&value);
}


MSA_INTR_STATUS_T *CWDM4_202_get_drv_intr_status(uint8_t intr_mask)
{
	static MSA_INTR_STATUS_T status;
	uint8_t value = 0;

	if(intr_mask & PORT_INTR_TX_FAULT)
	{
		cwdm4_202_txcdr_gn2105_i2c_read_byte(0x437, &value);  //get tx fault
		status.tx_fault = (value & 0x0F);
	}
	if(intr_mask & PORT_INTR_TX_LOS)
	{
		cwdm4_202_txcdr_gn2105_i2c_read_byte(0x413, &value);  //get tx los  
		status.tx_los = (value & 0x0F);
	}
	if(intr_mask & PORT_INTR_TX_LOL)
	{
		cwdm4_202_txcdr_gn2105_i2c_read_byte(0x413, &value);  //get tx lol  
		status.tx_lol = (value & 0xF0);
	}
	if(intr_mask & PORT_INTR_RX_LOL)
	{
		cwdm4_202_rxcdr_m37046_i2c_read_byte(0x14, &value);  //get rx lol 
		status.rx_lol = (value & 0xF0) >> 4;
	}	
	if(intr_mask & PORT_INTR_RX_LOS)
	{
		cwdm4_202_rxcdr_m37046_i2c_read_byte(0x14, &value);  //get rx los 
		status.rx_los = (value & 0x0F);
	}
	return &status;

}


void CWDM4_202_set_cdr_control(uint8_t value)
{
	value = 0;
}

void CWDM4_202_set_tx_eq(uint8_t channel, uint8_t value)
{
	channel = value = 0;
}

void CWDM4_202_set_rx_emphasis(uint8_t channel, uint8_t value)
{
	channel = value = 0;
}

void CWDM4_202_set_rx_amplitude(uint8_t channel, uint8_t value)
{
	channel = value = 0;
}

void CWDM4_202_set_cdr_sq_disable(uint8_t value)
{
	value = 0;
}

void CWDM4_202_set_rx_output_disable(uint8_t value)
{
	value = 0;
}

void CWDM4_202_set_tx_adapt_eq(uint8_t value1, uint8_t value2, uint8_t value3)
{
	value1 = value2 = value3 = 0;
}


#endif
