#include <math.h>
#include "Efm8lb12_adc.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_smbus.h"
#include "Efm8lb12_common.h"
#include "Efm8lb12_msa_pin.h"
#include "Efm8lb12_100g_com_drv.h"

#ifdef EFM8LB12E64_100G_SFP_AOC_101

static uint8_t _i2c_dev_ready = 0;

/****************************************************mcu port function declaration**********************************************/
#ifdef HW_VER_AM3_101
static void _aoc_101_port0_init_for_am3();
static void _aoc_101_port1_init_for_am3();
static void _aoc_101_port2_init_for_am3();
static void _aoc_101_port3_init_for_am3();
#endif

#ifdef HW_VER_AM4_101
static void _aoc_101_port0_init_for_am4();
static void _aoc_101_port1_init_for_am4();
static void _aoc_101_port2_init_for_am4();
static void _aoc_101_port3_init_for_am4();
#endif

#ifdef HW_VER_AM3_101
static void _aoc_101_port0_init_for_am3()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;
	//1.set port default value
	P0 = P0_B0__HIGH | P0_B1__HIGH | P0_B2__HIGH | P0_B3__HIGH | P0_B4__HIGH | P0_B5__HIGH | P0_B6__HIGH | P0_B7__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P0MDIN =  P0MDIN_B0__DIGITAL   					    //not used
			| P0MDIN_B1__DIGITAL 						//set p0.1 is used for 3v3_en pin
			| P0MDIN_B2__DIGITAL                        //set p0.2 is used for reset mcu
			| P0MDIN_B3__DIGITAL						//not used
			| P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL   //set p0.4 and p0.5 digital mode is used for uart0
			| P0MDIN_B6__DIGITAL 					    //not used
			| P0MDIN_B7__DIGITAL;					    //set p0.7 digital mode is used for QSFP  interrupt output


	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P0MDOUT = P0MDOUT_B0__OPEN_DRAIN    					    //not used
			| P0MDOUT_B1__PUSH_PULL 							//set p0.1 is used for 3v3_en pin
			| P0MDOUT_B2__OPEN_DRAIN							//set p0.2 is used for reset mcu
			| P0MDOUT_B3__OPEN_DRAIN	    					//not used
			| P0MDOUT_B4__OPEN_DRAIN | P0MDOUT_B5__OPEN_DRAIN   //set p0.4 push pull and p0.5 open drain is uesd for uart0
			| P0MDOUT_B6__OPEN_DRAIN							//not used
			| P0MDOUT_B7__OPEN_DRAIN;						    //set p0.7 is used for QSFP interrupt output


	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P0SKIP = P0SKIP_B0__SKIPPED					//because P1.2 and p1.3 are used to Smbus,so P0.0 ~ P0.7 must be skipped
			| P0SKIP_B1__SKIPPED| P0SKIP_B2__SKIPPED   //not used
			| P0SKIP_B3__SKIPPED					   //not used
			| P0SKIP_B4__SKIPPED | P0SKIP_B5__SKIPPED
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

static void _aoc_101_port1_init_for_am3()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;
	//1.set port default value
	P1 = P1_B0__HIGH | P1_B1__LOW | P1_B2__HIGH | P1_B3__HIGH | P1_B4__HIGH | P1_B5__HIGH | P1_B6__HIGH | P1_B7__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P1MDIN =  P1MDIN_B0__ANALOG  					   //set p1.0 analog mode is used for mointor the 3.3V module input voltage
			| P1MDIN_B1__DIGITAL 					   //set p1.1 digital mode is used for TX close signal
			| P1MDIN_B2__DIGITAL | P1MDIN_B3__DIGITAL  //set p1.2 and p1.3 digital mode is used for i2c master mode

			| P1MDIN_B4__DIGITAL     			  //not used
			| P1MDIN_B5__DIGITAL				  //tx cdr interupt input
			| P1MDIN_B6__DIGITAL    			  //not used
			| P1MDIN_B7__DIGITAL;                 //not used

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN    			  			//because the p1.0 are set analog mode闂佹寧绋戦鎶�this bit can be not set
			| P1MDOUT_B1__PUSH_PULL					  			//set p1.1 push pull mode is used for TX close signal
			| P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN	//because p1.2 and p1.3 are used for i2c master mode,so the two bits must be open drain

			| P1MDOUT_B4__OPEN_DRAIN            //not used
			| P1MDOUT_B5__OPEN_DRAIN			//because p1.5 is input pin,so set p1.5 default value(open drain mode) is used for tx cdr interupt input
			| P1MDOUT_B6__OPEN_DRAIN			//not used
			| P1MDOUT_B7__OPEN_DRAIN;           //not used

	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P1SKIP = P1SKIP_B0__SKIPPED 			//because p1.0 is set analog mode and p1.2 and p1.3 are used to Smbus ,so this bit must be skipped are used for mointor the 3.3V module input voltage
			| P1SKIP_B1__SKIPPED			//set p1.1 skip in order to p1.2 and p1.3 are used for i2c master mode
			| P1SKIP_B2__NOT_SKIPPED | P1SKIP_B3__NOT_SKIPPED

			//because P2.0 and p2.1 are used for i2c slave mode,so P1.0 ~ P1.7 must be skipped,p1.5 must be skipped is uesed to tx interrupt for port match
			| P1SKIP_B4__SKIPPED | P1SKIP_B5__SKIPPED| P1SKIP_B6__SKIPPED | P1SKIP_B7__SKIPPED;

	//5. set p1MASK
	P1MASK = P1MASK_B0__IGNORED | P1MASK_B1__IGNORED | P1MASK_B2__IGNORED
			//p1.5 must be compared is uesed to tx interrupt for port match
			| P1MASK_B3__IGNORED | P1MASK_B4__IGNORED | P1MASK_B5__COMPARED
			| P1MASK_B6__IGNORED | P1MASK_B7__IGNORED;

	//6. set P1MAT
	P1MAT = P1MAT_B0__HIGH | P1MAT_B1__HIGH | P1MAT_B2__HIGH | P1MAT_B3__HIGH
			| P1MAT_B4__HIGH | P1MAT_B5__HIGH | P1MAT_B6__HIGH | P1MAT_B7__HIGH;

	SFRPAGE = SFRPAGE_SAVE;
}


static void _aoc_101_port2_init_for_am3()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x20;
	//1.set port default value
	P2 = P2_B0__HIGH | P2_B1__HIGH | P2_B2__HIGH | P2_B3__HIGH | P2_B4__HIGH | P2_B5__HIGH | P2_B6__HIGH ;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P2MDIN = P2MDIN_B0__DIGITAL | P2MDIN_B1__DIGITAL	 //set p2.0 and p2.1 digital mode are used for i2c slave pin

			| P2MDIN_B2__DIGITAL   //not used

			| P2MDIN_B3__DIGITAL    //set p2.3 is used for rx cdr interrupt input

			| P2MDIN_B4__DIGITAL 	//not used

			| P2MDIN_B5__ANALOG 	//set p2.5 analog mode is used for RSSI

			| P2MDIN_B6__DIGITAL ;  //not used

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).

	P2MDOUT = P2MDOUT_B0__OPEN_DRAIN | P2MDOUT_B1__OPEN_DRAIN //set p2.0 and p2.1 open drain mode are used for i2c slave mode

			| P2MDOUT_B2__OPEN_DRAIN   //not used

			| P2MDOUT_B3__OPEN_DRAIN    //because p1.5 is input pin,so set p1.5 default value(open drain mode) is used for rx cdr interrupt input

			| P2MDOUT_B4__OPEN_DRAIN   //not used

			| P2MDOUT_B5__OPEN_DRAIN   //because the p2.5 is set analog mode闂佹寧绋戦鎶�this bit can be not set is used for RSSI

			| P2MDOUT_B6__OPEN_DRAIN;  //not used

	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P2SKIP = P2SKIP_B0__SKIPPED | P2SKIP_B1__SKIPPED   		//set p2.0 and p2.1 skipped for i2c slave mode
			| P2SKIP_B2__NOT_SKIPPED
			| P2SKIP_B3__SKIPPED		//p2.3 must be skipped is uesed to rx interrupt for port match

			//because p2.5 is set analog mode ,so this bit must be skipped is used for for RSSI
			| 0x20;

	//5. set p2MASK
	P2MASK = P2MASK_B0__IGNORED | P2MASK_B1__IGNORED | P2MASK_B2__IGNORED

			//p2.3 must be compared is uesed to rx interrupt for port match
			| P2MASK_B3__COMPARED | P2MASK_B4__IGNORED | P2MASK_B5__IGNORED
			| P2MASK_B6__IGNORED ;

	//6. set P2MAT
	P2MAT = P2MAT_B0__HIGH | P2MAT_B1__HIGH | P2MAT_B2__HIGH | P2MAT_B3__HIGH
			| P2MAT_B4__HIGH | P2MAT_B5__HIGH | P2MAT_B6__HIGH ;

	SFRPAGE = SFRPAGE_SAVE;
}

static void _aoc_101_port3_init_for_am3()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x20;
	//1.set port default value
	P3 = P3_B0__HIGH | P3_B1__HIGH | P3_B2__HIGH | P3_B3__HIGH | P3_B4__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P3MDIN = P3MDIN_B0__DIGITAL    //set p3.0  is used for lpmode pin

			| P3MDIN_B1__DIGITAL   //not used

			| P3MDIN_B2__DIGITAL   //set p3.2  is used for modsel pin

			| P3MDIN_B3__DIGITAL   //not used

			| P3MDIN_B4__DIGITAL;  //not used

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P3MDOUT = P3MDOUT_B0__OPEN_DRAIN   //because p3.0 is input pin,so set p3.0 default value(open drain mode) is used for lpmode pin

			| P3MDOUT_B1__OPEN_DRAIN   //because p3.4 is not used,so set p3.4 default value(open drain mode)

			| P3MDOUT_B2__OPEN_DRAIN   //because the p3.2 and uart pin multiplexing,so this pin must be open drain when use uart

			| P3MDOUT_B3__OPEN_DRAIN   //because p3.3 is not used,so set p3.4 default value(open drain mode)

			| P3MDOUT_B4__OPEN_DRAIN;  //because p3.4 is not used,so set p3.4 default value(open drain mode)

	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).

	//p3 do't have p3SKIP registers

	//5. set p2MASK
	//p3 do't have P3MASK registers

	//6. set P2MAT
	//p3 do't have P3MAT registers
	SFRPAGE = SFRPAGE_SAVE;
}
#endif

#ifdef HW_VER_AM4_101
static void _aoc_101_port0_init_for_am4()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;
	//1.set port default value
	P0 = P0_B0__HIGH | P0_B1__HIGH | P0_B2__HIGH | P0_B3__HIGH | P0_B4__HIGH | P0_B5__HIGH | P0_B6__HIGH | P0_B7__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P0MDIN =  P0MDIN_B0__DIGITAL   					    //not used
			| P0MDIN_B1__DIGITAL 						//set p0.1 is used for lpmode pin
			| P0MDIN_B2__DIGITAL                        //set p0.2 is used for reset mcu
			| P0MDIN_B3__DIGITAL						//not used
			| P0MDIN_B4__DIGITAL | P0MDIN_B5__DIGITAL   //set p0.4 and p0.5 digital mode is used for uart0
			| P0MDIN_B6__DIGITAL 					    //set p0.6 is used for rx interrupt pin
			| P0MDIN_B7__DIGITAL;					    //set p0.7 is used for tx interrupt pin


	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P0MDOUT = P0MDOUT_B0__OPEN_DRAIN    					    //not used
			| P0MDOUT_B1__OPEN_DRAIN 							//set p0.1 is used for lpmode pin
			| P0MDOUT_B2__OPEN_DRAIN							//set p0.2 is used for reset mcu
			| P0MDOUT_B3__OPEN_DRAIN	    					//not used
			| P0MDOUT_B4__OPEN_DRAIN | P0MDOUT_B5__OPEN_DRAIN   //set p0.4 push pull and p0.5 open drain is uesd for uart0
			| P0MDOUT_B6__OPEN_DRAIN							//set p0.6 is used for rx interrupt pin
			| P0MDOUT_B7__OPEN_DRAIN;						    //set p0.7 is used for tx interrupt pin


	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P0SKIP = P0SKIP_B0__SKIPPED					//because P1.2 and p1.3 are used to Smbus,so P0.0 ~ P0.7 must be skipped
			| P0SKIP_B1__SKIPPED| P0SKIP_B2__SKIPPED   //not used
			| P0SKIP_B3__SKIPPED					   //not used
			| P0SKIP_B4__SKIPPED | P0SKIP_B5__SKIPPED
			| P0SKIP_B6__SKIPPED 				//not used
			| P0SKIP_B7__SKIPPED;

	//5. set p0MASK
	P0MASK = P0MASK_B0__IGNORED | P0MASK_B1__IGNORED | P0MASK_B2__IGNORED
			| P0MASK_B3__IGNORED | P0MASK_B4__IGNORED | P0MASK_B5__IGNORED
			| P0MASK_B6__COMPARED | P0MASK_B7__COMPARED;

	//6. set P0MAT
	P0MAT = P0MAT_B0__HIGH | P0MAT_B1__HIGH | P0MAT_B2__HIGH | P0MAT_B3__HIGH
			| P0MAT_B4__HIGH | P0MAT_B5__HIGH | P0MAT_B6__HIGH | P0MAT_B7__HIGH;

	SFRPAGE = SFRPAGE_SAVE;

}

static void _aoc_101_port1_init_for_am4()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x0;
	//1.set port default value
	P1 = P1_B0__HIGH | P1_B1__LOW | P1_B2__HIGH | P1_B3__HIGH | P1_B4__HIGH | P1_B5__HIGH | P1_B6__HIGH | P1_B7__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P1MDIN =  P1MDIN_B0__DIGITAL  					   //set p1.0 is used for modsel pin
			| P1MDIN_B1__DIGITAL 					   //set p1.1 is used for tx_dis pin
			| P1MDIN_B2__DIGITAL | P1MDIN_B3__DIGITAL  //set p1.2 and p1.3 digital mode is used for i2c master mode

			| P1MDIN_B4__DIGITAL     			  //not used
			| P1MDIN_B5__DIGITAL				  //not used
			| P1MDIN_B6__DIGITAL    			  //not used
			| P1MDIN_B7__ANALOG;                  //set p1.7 analog mode is used for 3v3mon

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P1MDOUT = P1MDOUT_B0__OPEN_DRAIN    			  			//set p1.0 is used for modsel pin
			| P1MDOUT_B1__PUSH_PULL					  			//set p1.1 is used for tx_dis pin
			| P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN	//because p1.2 and p1.3 are used for i2c master mode,so the two bits must be open drain

			| P1MDOUT_B4__OPEN_DRAIN            //not used
			| P1MDOUT_B5__OPEN_DRAIN			//not used
			| P1MDOUT_B6__OPEN_DRAIN			//not used
			| P1MDOUT_B7__OPEN_DRAIN;           //set p1.7 analog mode is used for 3v3mon

	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P1SKIP = P1SKIP_B0__SKIPPED 			//because p1.0 is set analog mode and p1.2 and p1.3 are used to Smbus ,so this bit must be skipped are used for mointor the 3.3V module input voltage
			| P1SKIP_B1__SKIPPED			//set p1.1 skip in order to p1.2 and p1.3 are used for i2c master mode
			| P1SKIP_B2__NOT_SKIPPED | P1SKIP_B3__NOT_SKIPPED

			//because P2.0 and p2.1 are used for i2c slave mode,so P1.0 ~ P1.7 must be skipped,p1.5 must be skipped is uesed to tx interrupt for port match
			| P1SKIP_B4__SKIPPED | P1SKIP_B5__SKIPPED| P1SKIP_B6__SKIPPED | P1SKIP_B7__SKIPPED;

	//5. set p1MASK
	P1MASK = P1MASK_B0__COMPARED  //p1.0 must be compared is uesed to modsel for port match
			| P1MASK_B1__IGNORED | P1MASK_B2__IGNORED
			| P1MASK_B3__IGNORED | P1MASK_B4__IGNORED | P1MASK_B5__IGNORED
			| P1MASK_B6__IGNORED | P1MASK_B7__IGNORED;

	//6. set P1MAT
	P1MAT = P1MAT_B0__HIGH | P1MAT_B1__HIGH | P1MAT_B2__HIGH | P1MAT_B3__HIGH
			| P1MAT_B4__HIGH | P1MAT_B5__HIGH | P1MAT_B6__HIGH | P1MAT_B7__HIGH;

	SFRPAGE = SFRPAGE_SAVE;
}


static void _aoc_101_port2_init_for_am4()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x20;
	//1.set port default value
	P2 = P2_B0__HIGH | P2_B1__HIGH | P2_B2__HIGH | P2_B3__HIGH | P2_B4__HIGH | P2_B5__HIGH | P2_B6__HIGH ;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P2MDIN = P2MDIN_B0__DIGITAL | P2MDIN_B1__DIGITAL	 //set p2.0 and p2.1 digital mode are used for i2c slave pin

			| P2MDIN_B2__ANALOG     //set p2.2 analog mode is used for mpd3

			| P2MDIN_B3__ANALOG     //set p2.3 analog mode is used for mpd2

			| P2MDIN_B4__ANALOG 	//set p2.4 analog mode is used for mpd1

			| P2MDIN_B5__ANALOG 	//set p2.5 analog mode is used for mpd0

			| P2MDIN_B6__ANALOG ;   //set p2.6 analog mode is used for RSSI

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).

	P2MDOUT = P2MDOUT_B0__OPEN_DRAIN | P2MDOUT_B1__OPEN_DRAIN //set p2.0 and p2.1 open drain mode are used for i2c slave mode

			| P2MDOUT_B2__OPEN_DRAIN   //set p2.2 analog mode is used for mpd3

			| P2MDOUT_B3__OPEN_DRAIN   //set p2.3 analog mode is used for mpd2

			| P2MDOUT_B4__OPEN_DRAIN   //set p2.4 analog mode is used for mpd1

			| P2MDOUT_B5__OPEN_DRAIN   //set p2.5 analog mode is used for mpd0

			| P2MDOUT_B6__OPEN_DRAIN;  //set p2.6 analog mode is used for RSSI

	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).
	P2SKIP = P2SKIP_B0__SKIPPED | P2SKIP_B1__SKIPPED   		//set p2.0 and p2.1 skipped for i2c slave mode
			| P2SKIP_B2__NOT_SKIPPED
			| P2SKIP_B3__SKIPPED		//p2.3 must be skipped is uesed to rx interrupt for port match

			//because p2.5 is set analog mode ,so this bit must be skipped is used for for RSSI
			| 0x20;

	//5. set p2MASK
	P2MASK = P2MASK_B0__IGNORED | P2MASK_B1__IGNORED | P2MASK_B2__IGNORED

			//p2.3 must be compared is uesed to rx interrupt for port match
			| P2MASK_B3__IGNORED | P2MASK_B4__IGNORED | P2MASK_B5__IGNORED
			| P2MASK_B6__IGNORED ;

	//6. set P2MAT
	P2MAT = P2MAT_B0__HIGH | P2MAT_B1__HIGH | P2MAT_B2__HIGH | P2MAT_B3__HIGH
			| P2MAT_B4__HIGH | P2MAT_B5__HIGH | P2MAT_B6__HIGH ;

	SFRPAGE = SFRPAGE_SAVE;
}


static void _aoc_101_port3_init_for_am4()
{
	uint8_t SFRPAGE_SAVE = SFRPAGE;

	SFRPAGE = 0x20;
	//1.set port default value
	P3 = P3_B0__HIGH | P3_B1__HIGH | P3_B2__HIGH | P3_B3__HIGH | P3_B4__HIGH;

	//2.Select the input mode (analog or digital) for all port pins, using the Port Input Mode register (PnMDIN).
	P3MDIN = P3MDIN_B0__DIGITAL    //set p3.0 digital mode is used for intl pin

			| P3MDIN_B1__DIGITAL   //set p3.1 digital mode is used for 3v3_en pin

			| P3MDIN_B2__DIGITAL   //not used

			| P3MDIN_B3__DIGITAL   //not used

			| P3MDIN_B4__DIGITAL;  //not used

	//3. Select the output mode (open-drain or push-pull) for all port pins, using the Port Output Mode register (PnMDOUT).
	P3MDOUT = P3MDOUT_B0__OPEN_DRAIN   //set p3.0 is used for intl pin

			| P3MDOUT_B1__PUSH_PULL    //set p3.1 is used for 3v3_en pin

			| P3MDOUT_B2__OPEN_DRAIN   //because p3.2 is not used,so set p3.2 default value(open drain mode)

			| P3MDOUT_B3__OPEN_DRAIN   //because p3.3 is not used,so set p3.3 default value(open drain mode)

			| P3MDOUT_B4__OPEN_DRAIN;  //because p3.4 is not used,so set p3.4 default value(open drain mode)

	//4. Select any pins to be skipped by the I/O crossbar using the Port Skip registers (PnSKIP).

	//p3 do't have p3SKIP registers

	//5. set p2MASK
	//p3 do't have P3MASK registers

	//6. set P2MAT
	//p3 do't have P3MAT registers
	SFRPAGE = SFRPAGE_SAVE;
}
#endif

static bool _aoc_101_txcdr_check_exist()
{
	uint8_t i = 0, Data = 0;

	if(TX3V3_EN == 0x1)
	{
		MSA_p2_log("\r\ntxcdr is lower power mode");
		return false;
	}

	for(i = 0; i < 3; i++)
	{
		if(GOS_OK == smb_read_byte(AOC_101_TXCDR_BASE_ADDR, 0x0, &Data))
		{
			MSA_p2_log("\r\ntxcdr smb read OK");
			return true;
		}
		else
		{
			MSA_p2_log("\r\ntxcdr smb read failed");
		}
		Hw_time5_delay_us(5000);
	}

	return false;
}

static uint8_t _aoc_101_txcdr_m37145_i2c_read_byte(uint8_t OffsetAddr, uint8_t *Data)
{
	if(TX3V3_EN || ((_i2c_dev_ready & TX_READY) == 0x0))  //if TX3V3_EN == 1,indicat low power, there is prohibited to access i2c master
	{
		return GOS_FAIL;
	}
	return smb_read_byte(AOC_101_TXCDR_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t _aoc_101_txcdr_m37145_i2c_write_byte(uint8_t OffsetAddr, uint8_t Data)
{
	if(TX3V3_EN || ((_i2c_dev_ready & TX_READY) == 0x0))  //if TX3V3_EN == 1,indicat low power, there is prohibited to access i2c master
	{
		return GOS_FAIL;
	}
	return smb_write_byte(AOC_101_TXCDR_BASE_ADDR, OffsetAddr, Data);
}

static void _aoc_101_txcdr_m37145_reset()
{
	if(_aoc_101_txcdr_check_exist())
	{
		_i2c_dev_ready |= TX_READY;
	}

	_aoc_101_txcdr_m37145_i2c_write_byte(0x2, 0xaa); 	//txcdr reset
	Soft_timer_delay_ms(100);
	_aoc_101_txcdr_m37145_i2c_write_byte(0x2, 0x00); 	//txcdr reset
}

static void _aoc_101_txcdr_m37145_init()
{
	_aoc_101_txcdr_m37145_i2c_write_byte(0x11, 0x0f);;
	_aoc_101_txcdr_m37145_i2c_write_byte(0x19, 0xff);
	_aoc_101_txcdr_m37145_i2c_write_byte(0x1d, 0x7f);
	_aoc_101_txcdr_m37145_i2c_write_byte(0x1e, 0x05);
	_aoc_101_txcdr_m37145_i2c_write_byte(0x21, 0xff);
	_aoc_101_txcdr_m37145_i2c_write_byte(0x25, 0x04);
	_aoc_101_txcdr_m37145_i2c_write_byte(0x26, 0x04);
	_aoc_101_txcdr_m37145_i2c_write_byte(0x27, 0x04);
	_aoc_101_txcdr_m37145_i2c_write_byte(0x28, 0x04);
	_aoc_101_txcdr_m37145_i2c_write_byte(0x29, 0x0f);
	_aoc_101_txcdr_m37145_i2c_write_byte(0x41, 0x00);
	_aoc_101_txcdr_m37145_i2c_write_byte(0x4e, 0x00);
}

/***********************************************RXCDR OPS********************************************************/
static bool _aoc_101_rxcdr_check_exist()
{
	uint8_t i = 0, Data = 0;

	if(TX3V3_EN == 0x1)
	{
		MSA_p2_log("\r\nrxcdr is lower power mode");
		return false;
	}

	for(i = 0; i < 3; i++)
	{
		if(GOS_OK == smb_read_byte(AOC_101_RXCDR_BASE_ADDR, 0x0, &Data))
		{
			MSA_p2_log("\r\nrxcdr smb read OK");
			return true;
		}
		else
		{
			MSA_p2_log("\r\nrxcdr smb read failed");
		}
		Hw_time5_delay_us(5000);
	}

	return false;
}

static uint8_t _aoc_101_rxcdr_m37144_i2c_read_byte(uint8_t OffsetAddr, uint8_t *Data)
{
	if(TX3V3_EN || ((_i2c_dev_ready & RX_READY) == 0x0))  //if TX3V3_EN == 1,indicat low power, there is prohibited to access i2c master
	{
		return GOS_FAIL;
	}
	return smb_read_byte(AOC_101_RXCDR_BASE_ADDR, OffsetAddr, Data);
}

static uint8_t _aoc_101_rxcdr_m37144_i2c_write_byte(uint8_t OffsetAddr,uint8_t Data)
{
	if(TX3V3_EN || ((_i2c_dev_ready & RX_READY) == 0x0))  //if LPMODE == 1,indicat low power, there is prohibited to access i2c master
	{
		return GOS_FAIL;
	}
	return smb_write_byte(AOC_101_RXCDR_BASE_ADDR, OffsetAddr, Data);
}

static void _aoc_101_rxcdr_m37144_reset()
{
	if(_aoc_101_rxcdr_check_exist())
	{
		_i2c_dev_ready |= RX_READY;
	}

	_aoc_101_rxcdr_m37144_i2c_write_byte(0x2, 0xaa); 	//rxcdr reset
	Soft_timer_delay_ms(100);
	_aoc_101_rxcdr_m37144_i2c_write_byte(0x2, 0x00); 	//rxcdr reset
}


static void _aoc_101_rxcdr_m37144_init()
{
	_aoc_101_rxcdr_m37144_i2c_write_byte(0x03,0xf0);
	_aoc_101_rxcdr_m37144_i2c_write_byte(0x19,0xff);
	_aoc_101_rxcdr_m37144_i2c_write_byte(0x1b,0x03);
	_aoc_101_rxcdr_m37144_i2c_write_byte(0x23,0xf0);
	_aoc_101_rxcdr_m37144_i2c_write_byte(0x40,0xf0);
	_aoc_101_rxcdr_m37144_i2c_write_byte(0x42,0x1e);
	_aoc_101_rxcdr_m37144_i2c_write_byte(0x43,0x1e);
	_aoc_101_rxcdr_m37144_i2c_write_byte(0x44,0x1e);
	_aoc_101_rxcdr_m37144_i2c_write_byte(0x45,0x1e);
}

static uint16_t _aoc_101_get_bias_average_value()
{
	uint16_t buff16[AVERAGE_MAX_COUNTER] = {0};
	uint8_t i = 0, value_h = 0, value_l = 0;

	for(i = 0; i < AVERAGE_MAX_COUNTER; i++)
	{
		_aoc_101_txcdr_m37145_i2c_read_byte(0x68, &value_h);
		_aoc_101_txcdr_m37145_i2c_read_byte(0x69, &value_l);
		buff16[i] = (value_h << 4) | (value_l & 0xF);
	}

	return com_average_value16(buff16, AVERAGE_MAX_COUNTER);
}

/* Return in 0.1mA */
static uint16_t _aoc_101_get_ddm_bias(uint8_t channel)
{
	uint8_t  channel_reg[] = {0x80,0x90,0xA0,0xB0};
	uint8_t  value1 = 0, value2 = 0;
	uint16_t var2 = 0, var3 = 0, var1 = 1;
	float Ibias_ua = 0;

	_aoc_101_txcdr_m37145_i2c_write_byte(0x60,0x00);

	_aoc_101_txcdr_m37145_i2c_write_byte(0x90, channel_reg[channel]);

	_aoc_101_txcdr_m37145_i2c_write_byte(0x92, 0x4);
	Hw_time5_delay_us(5000);  //waiting adc transform finish
	var2 = _aoc_101_get_bias_average_value();

	_aoc_101_txcdr_m37145_i2c_write_byte(0x92, 0x8);
	Hw_time5_delay_us(5000);   //waiting adc transform finish
	var3 = _aoc_101_get_bias_average_value();

	Ibias_ua = (float)((var2 - var3) * var1 * 40.1);  // uA

	Ibias_ua = Ibias_ua / 100.0;

	return (uint16_t)(Ibias_ua);  // 0.1 mA
}

/* return in 0.1mA */
static uint16_t _aoc_101_get_ddm_mpd(uint8_t channel)
{
	return _aoc_101_get_ddm_bias(channel);
}

static uint16_t _aoc_101_get_rssi_average_value()
{
	uint8_t i = 0;
	uint16_t buff16[AVERAGE_MAX_COUNTER] = {0};

	for(i = 0; i < AVERAGE_MAX_COUNTER; i++)
	{
		buff16[i] = ADC0_manual_poll(ADC0_RSSI0) * 0.1;   // uint:mv
		Hw_time5_delay_us(2000);
	}

	return com_average_value16(buff16, AVERAGE_MAX_COUNTER);
}

/* return in 1uA */
static uint16_t _aoc_101_get_ddm_rssi(uint8_t channel)
{
	uint8_t value = 0;
	uint16_t rssi_mv = 0;

	channel = 3 - channel;  //rxcdr pin is inverted sequence with qsfp28 pin

	_aoc_101_rxcdr_m37144_i2c_read_byte(0x3, &value);

	value = SET_BIT(value, 6); //rssi enable

	if(channel == MSA_PWR_CHANNEL_1)
	{
		//bit4 = 0, bit5 = 0
		value = CLEAR_BIT(value, 4);
		value = CLEAR_BIT(value, 5);
	}
	else if(channel == MSA_PWR_CHANNEL_2)
	{
		//bit5 = 0, bit4 = 1
		value = SET_BIT(value, 4);
		value = CLEAR_BIT(value, 5);
	}
	else if(channel == MSA_PWR_CHANNEL_3)
	{
		//bit5 = 1, bit4 = 0
		value = CLEAR_BIT(value, 4);
		value = SET_BIT(value, 5);
	}
	else if(channel == MSA_PWR_CHANNEL_4)
	{
		//bit4 = 1, bit5 = 1
		value = SET_BIT(value, 4);
		value = SET_BIT(value, 5);
	}
	_aoc_101_rxcdr_m37144_i2c_write_byte(0x3, value);

	rssi_mv = _aoc_101_get_rssi_average_value();

	return (uint16_t)((float)rssi_mv / (float)AOC_101_RSSI_SERIES_RES * 1000.00);  //uA
}


/************************************** DRV Interface API ************************************/

void AOC_101_chip_reset()
{
	_aoc_101_txcdr_m37145_reset();
	_aoc_101_rxcdr_m37144_reset();
}

void AOC_101_chip_init()
{
	AOC_101_chip_reset();
	_aoc_101_txcdr_m37145_init();
	_aoc_101_rxcdr_m37144_init();
}

void AOC_101_port_init()
{
#ifdef HW_VER_AM3_101
	_aoc_101_port0_init_for_am3();
	_aoc_101_port1_init_for_am3();
	_aoc_101_port2_init_for_am3();
	_aoc_101_port3_init_for_am3();
#endif

#ifdef HW_VER_AM4_101
	_aoc_101_port0_init_for_am4();
	_aoc_101_port1_init_for_am4();
	_aoc_101_port2_init_for_am4();
	_aoc_101_port3_init_for_am4();
#endif
}


void AOC_101_polling()
{
	ADC0_poll();
}

int16_t AOC_101_ddmi_get_temp(MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		DDMI_CALI_PARA_T *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		return ((int16_t)((float)(ddmi_cali->temp) * Timer_temp_slow_boot_cal()) + ADC0_get_mcu_temp()); 		//MSA format uint: 1/256 C
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return ADC0_get_mcu_temp();	//uint: 1/256 C
	}
	return GOS_FAIL;
}

/* return voltage in 0.1mV */
uint16_t AOC_101_ddmi_get_vcc(MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		const DDMI_CALI_PARA_T *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();

		return (ddmi_cali->vcc + ADC0_get_vcc()); //uint:0.1mv
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return ADC0_get_vcc();  //uint:0.1mv
	}
	return 0;
}

/* Return bias in 0.1mA */
uint16_t AOC_101_ddmi_get_bias(uint8_t channel, MSA_DRV_VAL_TYPE type)
{
	if(type == MSA_DRV_VAL_NORM)
	{
		uint16_t bias = 0;
		const DDMI_CALI_PARA_T *ddmi_cali = MSA_p67_get_ddmi_cali_cfg();
		const MSA_LUT_T	 *laser_stat = MSA_p4_get_laser_state();

		//ddmi_cali->bias uint:0.1mA
		bias  = (uint16_t)((ddmi_cali->current[channel].bias.bias_offset2 + laser_stat->ch[channel].stat.txbias) * 0.1 * 1000.0 * 0.5); //uint:2uA
		return  bias;  //uint:2uA
	}
	else if(type == MSA_DRV_VAL_HW)
	{
		return (uint16_t)(_aoc_101_get_ddm_bias(channel));
	}
	return GOS_FAIL;
}

/* return power in 0.1uW or in 1uA */
uint16_t AOC_101_ddmi_get_rx_power(uint8_t channel, MSA_DRV_VAL_TYPE type)
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
		return  (uint16_t)_aoc_101_get_ddm_rssi(channel);  //uint:uA
	}
	return GOS_FAIL;
}

/* return power in 0.1uW or in 0.1mA */
uint16_t AOC_101_ddmi_get_tx_power(uint8_t channel, MSA_DRV_VAL_TYPE type)
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
		return (_aoc_101_get_ddm_mpd(channel)); // uint:0.1mA
	}
	return GOS_FAIL;
}

static uint8_t _aoc_101_crossing_value_handle(int16_t crossing)
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
	//crossing     value
	//	 -8          8
	//	 -7          9
	//	 -6          10
	//	 -5          11
	//	 -4          12
	//	 -3          13
	//	 -2          14
	//	 -1          15
	else
	{
		value = crossing + 16;
	}
	return value;
}

static int8_t _aoc_101_crossing_dac_tranfer_cross_lut(uint8_t cross)
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
	//crossing     value
	//	 -8          8
	//	 -7          9
	//	 -6          10
	//	 -5          11
	//	 -4          12
	//	 -3          13
	//	 -2          14
	//	 -1          15
	return (int8_t)value;
}

static void  _aoc_101_sfp_drv_check_laser_ch(SFP_DRV_LASER_CFG_T *cfg)
{
	if(cfg->bias > 40)
	{
		cfg->bias = 40;
	}
	if(cfg->mod > 120)
	{
		cfg->mod = 120;
	}
}

void AOC_101_drv_adjust_laser_ch(uint8_t ch, SFP_DRV_LASER_CFG_T *cfg)
{
	static code uint8_t bias_reg[]  = {0x42,0x43,0x44,0x45};
	static code uint8_t mod_reg[]   = {0x46,0x47,0x48,0x49};
	static code uint8_t cross_reg[] = {0x4f,0x50,0x51,0x52};

	cfg->cross = _aoc_101_crossing_value_handle(cfg->cross);

	_aoc_101_txcdr_m37145_i2c_write_byte(bias_reg[ch], (uint8_t)cfg->bias);
	_aoc_101_txcdr_m37145_i2c_write_byte(mod_reg[ch], (uint8_t)cfg->mod);
	_aoc_101_txcdr_m37145_i2c_write_byte(cross_reg[ch], (uint8_t)cfg->cross);
}

void AOC_101_drv_get_laser_full_state(uint8_t channel, MSA_LUT_T *lut)
{
	static code uint8_t bias_reg[]  = {0x42,0x43,0x44,0x45};
	static code uint8_t mod_reg[]   = {0x46,0x47,0x48,0x49};
	static code uint8_t cross_reg[] = {0x4f,0x50,0x51,0x52};
	uint8_t bias, mod, cross;

	lut->temp = DDM_GET_TEMP(MSA_DRV_VAL_HW);

	_aoc_101_txcdr_m37145_i2c_read_byte(bias_reg[channel],  &bias);
	_aoc_101_txcdr_m37145_i2c_read_byte(mod_reg[channel],   &mod);
	_aoc_101_txcdr_m37145_i2c_read_byte(cross_reg[channel], &cross);

	lut->ch[channel].config.bias = (uint16_t)(bias & 0x7f);
	lut->ch[channel].config.mod  = (uint16_t)(mod & 0x7f);
	lut->ch[channel].config.cross = _aoc_101_crossing_dac_tranfer_cross_lut(cross);
}

void AOC_101_drv_ch_calc_cfg (int16_t cur_temp, uint8_t ch, const SFP_DRV_LASER_CFG_T *lut, SFP_DRV_LASER_CFG_T *ret)
{
	SFP_DRV_LASER_CFG_T *lo_cfg = (lut + ch);
	SFP_DRV_LASER_CFG_T *m_cfg  = (lo_cfg + MSA_CHANNEL_NUM);
	SFP_DRV_LASER_CFG_T *hi_cfg = (m_cfg + MSA_CHANNEL_NUM);

	ret->temp = cur_temp;

	ret->bias  = 40;
	ret->mod   = 80;
	ret->cross = 15;

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
		ret->cross = (int16_t)(floor(0.5 + (double)lo_cfg->cross +
						((double)(cur_temp - lo_cfg->temp)) * ((double)(m_cfg->cross - lo_cfg->cross))
						/ ((double)(m_cfg->temp - lo_cfg->temp))));
	}
	else
	{
		if((m_cfg->temp == hi_cfg->temp))
		{
			return;
		}

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

	_aoc_101_sfp_drv_check_laser_ch(ret);
}

MSA_INTR_STATUS_T *AOC_101_get_drv_intr_status(uint8_t intr_mask)
{
	static MSA_INTR_STATUS_T status;
	uint8_t value = 0;

	if(intr_mask & PORT_INTR_TX_FAULT)
	{
		_aoc_101_txcdr_m37145_i2c_read_byte(0x18, &value);  //get tx fault
		status.tx_fault = (value & 0x0F);
	}

	if(intr_mask & PORT_INTR_TX_LOS)
	{
		_aoc_101_txcdr_m37145_i2c_read_byte(0x17, &value);  //get tx los
		status.tx_los = (value & 0x0F);
	}

	if(intr_mask & PORT_INTR_TX_LOL)
	{
		_aoc_101_txcdr_m37145_i2c_read_byte(0x17, &value);  //get tx lol
		status.tx_lol = (value & 0xF0) >> 4;
	}

	if(intr_mask & PORT_INTR_RX_LOS)
	{
		_aoc_101_rxcdr_m37144_i2c_read_byte(0x17, &value);  //get rx los
		status.rx_los = REVERSE_LSB_4BITS(value & 0x0F);
	}

	if(intr_mask & PORT_INTR_RX_LOL)
	{
		_aoc_101_rxcdr_m37144_i2c_read_byte(0x17, &value);  //get rx lol
		status.rx_lol = REVERSE_LSB_4BITS(((value & 0xF0) >> 4));
	}
	return &status;
}

//MSA Lower Page byte86 for tx disable
void AOC_101_set_tx_enable(uint8_t enale)
{
	uint8_t value = 0;

	_aoc_101_txcdr_m37145_i2c_read_byte(0x41,&value);

	value = CLEAR_LSB_4BITS(value)|CLEAR_MSB_4BITS(enale);

	_aoc_101_txcdr_m37145_i2c_write_byte(0x41,value);
	_aoc_101_txcdr_m37145_i2c_read_byte(0x41,&value);
}

//MSA Lower Page byte93 for power override
void  AOC_101_set_low_power(uint8_t enable)
{
	if(enable)
	{
		IT01CF &= ~IT01CF_IN0PL__ACTIVE_HIGH;
		TX_DIS   = 1;  //close txcdr inder
		TX3V3_EN = 1;  //close tx3v3 in order to realize low power mode
		SDA_M = 0;
		SCL_M = 0;
	}
	else
	{
		MSA_set_low_power_mode(1);
		IT01CF |= IT01CF_IN0PL__ACTIVE_HIGH;
		TX_DIS   = 0;  //open txcdr inder
		TX3V3_EN = 0; //open tx3v3 in order to realize high power mode
		SDA_M = 1;
		SCL_M = 1;
	}
}

//MSA Lower Page byte98 for tx and rx cdr control
void AOC_101_set_cdr_control(uint8_t value)
{
	uint8_t tx_cdr_control = 0, rx_cdr_control = 0, tmp = 0;

	tx_cdr_control = ((~value) & 0xF0) >> 4;
	rx_cdr_control = ((~value) & 0x0F);

	rx_cdr_control = REVERSE_LSB_4BITS(rx_cdr_control);

	_aoc_101_txcdr_m37145_i2c_read_byte(0x10,&tmp);
	tmp = CLEAR_LSB_4BITS(tmp)| tx_cdr_control;
	_aoc_101_txcdr_m37145_i2c_write_byte(0x10, tmp);  //set tx cdr control

	_aoc_101_rxcdr_m37144_i2c_read_byte(0x10,&tmp);
	tmp = CLEAR_LSB_4BITS(tmp)| rx_cdr_control;
	_aoc_101_rxcdr_m37144_i2c_write_byte(0x10, tmp);  //set rx cdr control
}

//MSA Page3 byte234 and byte235 for tx eq
void AOC_101_set_tx_eq(uint8_t channel, uint8_t value)
{
	uint8_t reg_addr[] = {0x25, 0x26,0x27,0x28};
	uint8_t tx_eq[] = {0x0, 0x0, 0x1, 0x2, 0x3, 0x4, 0x7 ,0x8, 0xa, 0xb, 0xd};
	uint8_t tmp = 0;

	if(value > TX_EQ_MAX_VALID)
	{
		return;
	}

	if(value == 0xFF)  //indicates there is not adaptive mode
	{
		tx_eq[value] = 0x6;
	}

	_aoc_101_txcdr_m37145_i2c_read_byte(reg_addr[channel], &tmp);

	value = (CLEAR_LSB_4BITS(tmp) | tx_eq[value]);

	_aoc_101_txcdr_m37145_i2c_write_byte(reg_addr[channel], value);
}

//MSA Page3 byte236 and byte237 for rx emphasis
void AOC_101_set_rx_emphasis(uint8_t channel, uint8_t value)
{
	uint8_t reg_addr[] = {0x46, 0x46,0x47,0x47};
	uint8_t rx_emphasis[] = {0x0, 0x3, 0x5, 0x7,0x9, 0xb, 0xd, 0xf};
	uint8_t tmp = 0;

	if(value > RX_EMPHASIS_MAX_VALID)
	{
		return ;
	}

	//rx channel reverse, rxcdr pin is inverted sequence with qsfp28 pin
	channel = 3 - channel;

	_aoc_101_rxcdr_m37144_i2c_read_byte(reg_addr[channel], &tmp);

	if(channel == 0x0 || channel == 0x2)
	{
		value = CLEAR_LSB_4BITS(tmp) | rx_emphasis[value];
	}
	else if(channel == 0x1 || channel == 0x3)
	{
		value = CLEAR_MSB_4BITS(tmp) | (rx_emphasis[value] << 4);
	}

	_aoc_101_rxcdr_m37144_i2c_write_byte(reg_addr[channel], value);//rxcdr pin is inverted sequence with qsfp28 pin
}

//MSA Page3 byte238 and byte239 rx amplitude
//0x3 900  --0x3c
//0x2 600  --0x1e
//0x1 450  --0x0f
//0x0 250  --0x00
void AOC_101_set_rx_amplitude(uint8_t channel, uint8_t value)
{
	uint8_t reg_addr[] = {0x42,0x43,0x44,0x45};
	uint8_t rx_amplitude[] = {0x00, 0x0f, 0x1e, 0x3c};
	uint8_t tmp = 0;

	if(value > RX_AMPLITUDE_MAX_VALID)
	{
		return;
	}

	//rx channel reverse
	channel = 3 - channel;

	_aoc_101_rxcdr_m37144_i2c_read_byte(reg_addr[channel], &tmp);//rxcdr pin is inverted sequence with qsfp28 pin

	value = (tmp & 0xC0) | rx_amplitude[value];

	_aoc_101_rxcdr_m37144_i2c_write_byte(reg_addr[channel], value);
}


//MSA Page3 byte240 sq disable
void AOC_101_set_cdr_sq_disable(uint8_t value)
{
	uint8_t tx_sq = 0, rx_sq = 0, tmp = 0;

	rx_sq = (value & 0xF0) >> 4;
	tx_sq  = (value & 0x0F);
	rx_sq = REVERSE_LSB_4BITS(rx_sq);

	_aoc_101_txcdr_m37145_i2c_read_byte(0x40,&tmp);
	tmp = CLEAR_MSB_4BITS(tmp)| (tx_sq << 4);
	_aoc_101_txcdr_m37145_i2c_write_byte(0x40, tmp);  //set tx cdr control

	_aoc_101_rxcdr_m37144_i2c_read_byte(0x40,&tmp);
	tmp = CLEAR_MSB_4BITS(tmp)| (rx_sq << 4);
	_aoc_101_rxcdr_m37144_i2c_write_byte(0x40, tmp);  //set rx cdr control
}


//MSA Page3 byte241 for rx output disable
void AOC_101_set_rx_output_disable(uint8_t value)
{
	uint8_t rx_output_disable = 0,  tmp = 0;

	rx_output_disable = (value & 0xF0) >> 4;
	rx_output_disable = REVERSE_LSB_4BITS(rx_output_disable);

	_aoc_101_rxcdr_m37144_i2c_read_byte(0x10,&tmp);
	tmp = CLEAR_MSB_4BITS(tmp)| (rx_output_disable << 4);
	_aoc_101_rxcdr_m37144_i2c_write_byte(0x10, tmp);
}

//MSA Page3 byte241 for tx adapt eq
void AOC_101_set_tx_adapt_eq(uint8_t value1, uint8_t value2, uint8_t value3)
{
	uint8_t channel = 0, tmp = 0, tx_adapt_eq = 0, i = 0;
	uint8_t reg_addr[] = {0x25,0x26,0x27,0x28};

	tx_adapt_eq = (value1 & 0x0F);

	for(channel = 0; channel < MSA_CHANNEL_NUM; channel ++)
	{
		if(GET_BIT(tx_adapt_eq, channel) == 0x1)
		{
			_aoc_101_txcdr_m37145_i2c_read_byte(reg_addr[channel],&tmp);
			tmp = CLEAR_LSB_4BITS(tmp)| (0x6);
			_aoc_101_txcdr_m37145_i2c_write_byte(reg_addr[channel], tmp);
		}
		else
		{
			if(channel >= 0x0 && channel < 0x2)
			{
				AOC_101_set_tx_eq(channel,(value2 >> (4 * ((channel + 1) % 2)))& 0x0f);
			}
			else if(channel >= 0x2 && channel < MSA_CHANNEL_NUM)
			{
				AOC_101_set_tx_eq(channel,(value3 >> (4 * ((channel + 1) % 2)))& 0x0f);
			}

		}
	}
}

#endif

