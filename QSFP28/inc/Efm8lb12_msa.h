#ifndef __EFM8LB12_msa_h__
#define __EFM8LB12_msa_h__

/*
 * Include MSA Page here ...
 */
#define MSA_PAGE_LOWER_PAGE
#define MSA_PAGE_0
#define MSA_PAGE_1
#define MSA_PAGE_2
#define MSA_PAGE_3
#define MSA_PAGE_4
#define MSA_PAGE_5
#define MSA_PAGE_6
#define MSA_PAGE_9
//#define MSA_PAGE_10  //there is no need to use page10

#define MSA_PAGE_SIZE				(128)
#define MSA_PAGE_MASK_LEN   		(MSA_PAGE_SIZE/8)
#define MSA_CHANNEL_NUM 			(4)
#define DDMI_TEMP_MSA_SCALE 		(0x100)
#define MSA_CONTROL_BYTE_MASK  		(0x7FF)

#define EEP_INIT_OK   		(0x1)

typedef enum
{
	ANY_PWD_MATCH_FAILED    = 0x0,
	FACTORY_PWD_MATCH_OK 	= 0x1,
	OEM_PWD_MATCH_OK 	 	= 0x2,
}PASSWD_TYPE;


typedef struct
{
	uint8_t  temp;
	uint8_t  vcc;
	uint16_t rxpower;
	uint16_t txbias;
	uint16_t txpower;
}MSA_DDMI_MSAK;

typedef struct
{
	uint8_t lower_page;
	uint8_t page0;
	uint8_t page1;
	uint8_t page2;
	uint8_t page3;
}EEP_CHANGE_FLAG_T;

typedef enum {
	MSA_OP_IDLE,
	MSA_OP_READ,
	MSA_OP_READ_WITH_OFFSET,
	MSA_OP_WRITE,
	MSA_OP_WRITE_WITH_OFFSET,
}MSA_OP_TYPE;

typedef enum {
    MSA_PWR_CHANNEL_1	= 0,
    MSA_PWR_CHANNEL_2 	= 1,
    MSA_PWR_CHANNEL_3 	= 2,
    MSA_PWR_CHANNEL_4 	= 3,
}MSA_PWR_CHANNEL;

typedef enum {
	MSA_PAGE_LOWER = 0xFE,
    MSA_PAGE_00H   = 0x00,
    MSA_PAGE_01H   = 0x01,
    MSA_PAGE_02H   = 0x02,
    MSA_PAGE_03H   = 0x03,
    MSA_PAGE_04H   = 0x04,
    MSA_PAGE_05H   = 0x05,
    MSA_PAGE_06H   = 0x06,
    MSA_PAGE_07H   = 0x07,
	MSA_PAGE_08H   = 0x08,
	MSA_PAGE_09H   = 0x09,
	MSA_PAGE_0AH   = 0x0A,
/*
    MSA_PAGE_08H   = 0x08,
    MSA_PAGE_09H   = 0x09,
    MSA_PAGE_10H   = 0x0A,
    MSA_PAGE_11H   = 0x0B,
    MSA_PAGE_12H   = 0x0C,
*/
    MSA_PAGE_MAX,
} MSA_PAGE_NUM;

typedef enum {
	//MSA_FLASH_ADDR_PAGE_LOWER	= 0xF480, //0xEF40, page2 is reserved space for user
	MSA_FLASH_ADDR_PAGE_00H		= 0xEE00, //0xEE12, after 59k
	MSA_FLASH_ADDR_PAGE_01H		= 0xEE80, //0xEE92,
	MSA_FLASH_ADDR_PAGE_02H		= 0xEF00, //0xEF12,
	MSA_FLASH_ADDR_PAGE_03H		= 0xEF80, //0xEF92,
	MSA_FLASH_ADDR_PAGE_05H		= 0xF000,
	MSA_FLASH_ADDR_PAGE_06H		= 0xF080, //0xF000,
	MSA_FLASH_ADDR_PAGE_07H		= 0xF100, //0xF080,
	MSA_FLASH_ADDR_PAGE_08H 	= 0xF180, //0xF080,
	MSA_FLASH_ADDR_PAGE_09H 	= 0xF200, //0xF080,
	MSA_FLASH_ADDR_PAGE_0AH		= 0xF280,
	MSA_FLASH_ADDR_PAGE_LOWER	= 0xF300, 
/*
	MSA_FLASH_ADDR_PAGE_08H		= 0xF100,
	MSA_FLASH_ADDR_PAGE_09H		= 0xF180,

	MSA_FLASH_ADDR_PAGE_0AH		= 0xF200,
	MSA_FLASH_ADDR_PAGE_0BH		= 0xF280,
	MSA_FLASH_ADDR_PAGE_0CH		= 0xF300,

	MSA_FLASH_ADDR_PAGE_0DH		= 0xF380,
	MSA_FLASH_ADDR_PAGE_0EH		= 0xF400,
	MSA_FLASH_ADDR_PAGE_0FH		= 0xF480,
	MSA_FLASH_ADDR_PAGE_10H		= 0xF500,
	MSA_FLASH_ADDR_PAGE_11H		= 0xF580,
	MSA_FLASH_ADDR_PAGE_12H		= 0xF600,
	MSA_FLASH_ADDR_PAGE_13H		= 0xF680,
	MSA_FLASH_ADDR_PAGE_14H		= 0xF700,
	MSA_FLASH_ADDR_PAGE_15H		= 0xF780,
	MSA_FLASH_ADDR_PAGE_16H		= 0xF800,
	MSA_FLASH_ADDR_PAGE_17H		= 0xF880,
	MSA_FLASH_ADDR_PAGE_18H		= 0xF900,
	MSA_FLASH_ADDR_PAGE_19H		= 0xF980,
*/
}MSA_FLASH_ADDR_PAGE;

typedef enum {
	MSA_PIN_INVAID,
	MSA_PIN_IN_MOD_SEL,		// Module Select
	MSA_PIN_IN_RESET,		// Reset
	MSA_PIN_IN_LP_MODE,		// Low Power Mode
	MSA_PIN_OUT_INTR,		// Interrupt indication to Host
}MSA_PIN_ID;

typedef enum {
	MSA_PIN_ACTION_NONE,
	MSA_PIN_ACTION_DBG_GET,
	MSA_PIN_ACTION_DBG_SET,
	MSA_PIN_ACTION_TRIGGER,
}MSA_PIN_ACTION;

typedef struct
{
	uint8_t tx_los;
	uint8_t tx_lol;
	uint8_t rx_los;
	uint8_t rx_lol;
	uint8_t tx_fault;
}MSA_INTR_STATUS_T;

typedef enum
{
	TX_ENABLE 	    	  =  0x01,
	LOW_POWER_MODE  	  =  0x02,
	TX_RX_CDR_CONTROL     =  0x04,
	TX1_TX2_EQ			  =  0x08,
	TX3_TX4_EQ			  =  0x10,
	RX1_RX2_EMPHASIS 	  =  0x20,
	RX3_RX4_EMPHASIS 	  =  0x40,
	RX1_RX2_AMPLITUDE	  =  0x80,
	RX3_RX4_AMPLITUDE	  =  0x100,
	TX_RX_SQ_DISABLE 	  =  0x200,
	RX_OUTPUT_TX_ADAPT_EQ =  0x400,

}MSA_CONTRAL_DRV_TYPE;

typedef struct {
	uint8_t page_num;
	uint8_t len;
	uint8_t *value;
}MSA_WRITE_PARAM_T;

#if 0
/***********************************A0 01h**************************************
Byte Bit range Name of Field Description
128 7-0 CC_APPS Check code for the AST; the check code shall be the
low order 8 bits of the sum of the contents of all the
bytes from byte 129 to byte 255, inclusive.
129 7,6 Reserved
129 5-0 AST Table Length, TL A 6-bit binary number, TL, specifies how many
application table entries are defined in bytes 130-255
addresses. TL is valid between 0 (1 entry) and 62 (for
a total of 63 entries).
130, 131 7-0, 7-0 Application code 0 Definition of first application supported
бн Other table entries
130+2*TL,
131+2*TL
7-0, 7-0 Application code TL Definition of last application supported
*******************************************************************************/
#define MSA_OFFSET_PAGE_01H_START                       128
#define MSA_OFFSET_PAGE_01H_END                         255
/***********************************A0 02h**************************************
RESERVED
******************************************************************************/
#define MSA_OFFSET_PAGE_02H_START                       128
#define MSA_OFFSET_PAGE_02H_END                         255

/***********************************A0 03h**************************************
Byte 	# Bytes 	Name 	Description 	PC 	AC 	AO 	SM
128-129 	2	Temp High Alarm 	"MSB at lower byteaddress"	C 	C 	C 	C
130-131 	2	Temp Low Alarm 	"MSB at lower byteaddress"	C 	C 	C 	C
132-133 	2	Temp High Warning 	"MSB at lower byteaddress"	O 	O 	O 	O
134-135 	2	Temp Low Warning 	"MSB at lower byteaddress"	O 	O 	O 	O
136-143 	8	Reserved 	- 	- 	- 	-	
144-145 	2	Vcc High Alarm 	"MSB at lower byteaddress"	C 	C 	C 	C
146-147 	2	Vcc Low Alarm 	"MSB at lower byteaddress"	C 	C 	C 	C
148-149 	2	Vcc High Warning 	"MSB at lower byteaddress"	O 	O 	O 	O
150-151 	2	Vcc Low Warning 	"MSB at lower byteaddress"	O 	O 	O 	O
152-159 	8	Reserved 	- 	- 	- 	-	
160-175 	16	Vendor Specific 	- 	- 	- 	-	
176-177 	2	RX Power High Alarm 	"MSB at lower byteaddress"	C 	C 	C 	C
178-179 	2	RX Power Low Alarm 	"MSB at lower byteaddress"	C 	C 	C 	C
180-181 	2	RX Power High Warning 	"MSB at lower byteaddress"	O 	O 	O 	O
182-183 	2	RX Power Low Warning 	"MSB at lower byteaddress"	O 	O 	O 	O
184-185 	2	Tx Bias High Alarm 	"MSB at lower byteaddress"	C 	C 	C 	C
186-187 	2	"Tx Bias LowAlarm"	"MSB at lower byteaddress"	C 	C 	C 	C
188-189 	2	Tx Bias High Warning 	"MSB at lower byteaddress"	O 	O 	O 	O
190-191 	2	Tx Bias Low Warning 	"MSB at lower byteaddress"	O 	O 	O 	O
192-193 	2	"Tx Power HighAlarm"	"MSB at lower byteaddress"	C 	C 	C 	C
194-195 	2	"Tx Power LowAlarm"	"MSB at lower byteaddress"	C 	C 	C 	C
196-197 	2	Tx Power High Warning 	"MSB at lower byteaddress"	O 	O 	O 	O
198-199 	2	Tx Power Low Warning 	"MSB at lower byteaddress"	O 	O 	O 	O
200-207 	8	Reserved 	"Reserved thresholdsfor channel parameterset 4"	- 	- 	- 	-
208-215 	8	Reserved 	"Reserved thresholdsfor channel parameterset 5"	- 	- 	- 	-
216-223 	8	Vendor Specific					
******************************************************************************/
#define MSA_OFFSET_PAGE_03H_START                           128
#define MSA_OFFSET_PAGE_03H_TEMP_HIGH_ALARM_MSB             128
#define MSA_OFFSET_PAGE_03H_TEMP_HIGH_ALARM_LSB             129

#define MSA_OFFSET_PAGE_03H_TEMP_LOW_ALARM_MSB              130
#define MSA_OFFSET_PAGE_03H_TEMP_LOW_ALARM_LSB              131

#define MSA_OFFSET_PAGE_03H_TEMP_HIGH_WARNING_MSB           132
#define MSA_OFFSET_PAGE_03H_TEMP_HIGH_WARNING_LSB           133

#define MSA_OFFSET_PAGE_03H_TEMP_LOW_WARNING_MSB            134
#define MSA_OFFSET_PAGE_03H_TEMP_LOW_WARNING_LSB            135

#define MSA_OFFSET_PAGE_03H_RESERVED_BYTE136                136

#define MSA_OFFSET_PAGE_03H_VCC_HIGH_ALARM_MSB              144
#define MSA_OFFSET_PAGE_03H_VCC_HIGH_ALARM_LSB              145

#define MSA_OFFSET_PAGE_03H_VCC_LOW_ALARM_MSB               146
#define MSA_OFFSET_PAGE_03H_VCC_LOW_ALARM_LSB               147

#define MSA_OFFSET_PAGE_03H_VCC_HIGH_WARNING_MSB            148
#define MSA_OFFSET_PAGE_03H_VCC_HIGH_WARNING_LSB            149

#define MSA_OFFSET_PAGE_03H_VCC_LOW_WARNING_MSB             150
#define MSA_OFFSET_PAGE_03H_VCC_LOW_WARNING_LSB             151

#define MSA_OFFSET_PAGE_03H_RESERVED_BYTE152                152

#define MSA_OFFSET_PAGE_03H_RX_POWER_HIGH_ALARM_MSB         176
#define MSA_OFFSET_PAGE_03H_RX_POWER_HIGH_ALARM_LSB         177

#define MSA_OFFSET_PAGE_03H_RX_POWER_LOW_ALARM_MSB          178
#define MSA_OFFSET_PAGE_03H_RX_POWER_LOW_ALARM_LSB          179

#define MSA_OFFSET_PAGE_03H_RX_POWER_HIGH_WARNING_MSB       180
#define MSA_OFFSET_PAGE_03H_RX_POWER_HIGH_WARNING_LSB       181

#define MSA_OFFSET_PAGE_03H_RX_POWER_LOW_WARNING_MSB        182
#define MSA_OFFSET_PAGE_03H_RX_POWER_LOW_WARNING_LSB        183

#define MSA_OFFSET_PAGE_03H_TX_BIAS_HIGH_ALARM_MSB          184
#define MSA_OFFSET_PAGE_03H_TX_BIAS_HIGH_ALARM_LSB          185

#define MSA_OFFSET_PAGE_03H_TX_BIAS_LOW_ALARM_MSB           186
#define MSA_OFFSET_PAGE_03H_TX_BIAS_LOW_ALARM_LSB           187

#define MSA_OFFSET_PAGE_03H_TX_BIAS_HIGH_WARNING_MSB        188
#define MSA_OFFSET_PAGE_03H_TX_BIAS_HIGH_WARNING_LSB        189

#define MSA_OFFSET_PAGE_03H_TX_BIAS_LOW_WARNING_MSB         190
#define MSA_OFFSET_PAGE_03H_TX_BIAS_LOW_WARNING_LSB         191

#define MSA_OFFSET_PAGE_03H_TX_POWER_HIGH_ALARM_MSB     	192
#define MSA_OFFSET_PAGE_03H_TX_POWER_HIGH_ALARM_LSB     	193

#define MSA_OFFSET_PAGE_03H_TX_POWER_LOW_ALARM_MSB      	194
#define MSA_OFFSET_PAGE_03H_TX_POWER_LOW_ALARM_LSB      	195

#define MSA_OFFSET_PAGE_03H_TX_POWER_HIGH_WARNING_MSB   	196
#define MSA_OFFSET_PAGE_03H_TX_POWER_HIGH_WARNING_LSB   	197

#define MSA_OFFSET_PAGE_03H_TX_POWER_LOW_WARNING_MSB     	198
#define MSA_OFFSET_PAGE_03H_TX_POWER_LOW_WARNING_LSB     	199

#define MSA_OFFSET_PAGE_03H_RESERVED_BYTE200                200
#define MSA_OFFSET_PAGE_03H_RESERVED_BYTE208                208

/*Tx EQ & Rx Emphasis Magnitude ID*/
#define MSA_OFFSET_PAGE_03H_TX_EQ_AND_RX_EMPH_MAGN_ID       224

#define MSA_OFFSET_PAGE_03H_RESERVED_BYTE226                226

//tx1 input equalization control 
#define MSA_OFFSET_PAGE_03H_TX1_TX2_INPUT_EQUAL_CTL     234
#define MSA_OFFSET_PAGE_03H_TX3_TX4_INPUT_EQUAL_CTL     235
#define MSA_OFFSET_PAGE_03H_RX1_RX2_OUTPUT_EQUAL_CTL    236
#define MSA_OFFSET_PAGE_03H_RX3_RX4_OUTPUT_EQUAL_CTL    237

//rx1 output emphasis control
#define MSA_OFFSET_PAGE_03H_RX1_RX2_OUTPUT_AMPL_CTL     238
#define MSA_OFFSET_PAGE_03H_RX3_RX4_OUTPUT_AMPL_CTL     239
#define MSA_OFFSET_PAGE_03H_RX_TX_SQ                    240
#define MSA_OFFSET_PAGE_03H_RX_OUTPUT_DISABLE           241
#define MSA_OFFSET_PAGE_03H_RX1_RX2_PWR_MSK             242
#define MSA_OFFSET_PAGE_03H_RX3_RX4_PWR_MSK             243
#define MSA_OFFSET_PAGE_03H_TX1_TX2_BIAS_MSK            244
#define MSA_OFFSET_PAGE_03H_TX3_TX4_BIAS_MSK            245
#define MSA_OFFSET_PAGE_03H_TX1_TX2_PWR_MSK             246
#define MSA_OFFSET_PAGE_03H_TX3_TX4_PWR_MSK             247
#define MSA_OFFSET_PAGE_03H_RESERVED_BYTE248            248
#define MSA_OFFSET_PAGE_03H_MAX                         255

#endif

typedef enum
{
    MSA_ACTION_READ_BYTE 		= 1,
    MSA_ACTION_WRITE_BYTE		= 2,
    // MSA_ACTION_ENABLE_LOG	= 4,
	MSA_ACTION_DEBUG_LOG		= 4,
    MSA_ACTION_START_POLL		= 5,
    MSA_ACTION_STOP_POLL		= 6,
    MSA_ACTION_DRYRUN_CALI		= 7,

	MSA_ACTION_JUMP_TO_BOOT		= 9,
    
    MSA_ACTION_START_DDMI		= 0xa,
    MSA_ACTION_STOP_DDMI		= 0xb,
    MSA_ACTION_RESET			= 0xc,
    MSA_ACTION_INIT_EEP_REG		= 0xd,
    MSA_ACTION_SET_EEP_REG		= 0xe,
    MSA_ACTION_SET_LUT			= 0xf,
    MSA_ACTION_ENABLE_CONSOLE	= 0x10,
    MSA_ACTION_TEC_ENABLE		= 0x11,
    MSA_ACTION_LASER_ON			= 0x12,
    MSA_ACTION_LASER_OFF		= 0x13,
    MSA_ACTION_FACTORY_RESET	= 0x14,
    MSA_ACTION_START_AGING		= 0x15,
    MSA_ACTION_STOP_AGING		= 0x16,
    MSA_ACTION_AGINIG_EEP_INIT	= 0x17,
    MSA_ACTION_ENTER_BONDING	= 0x18,
    MSA_ACTION_EEP_ERASE	 	= 0x1B,
	// internally used
	MSA_ACTION_FORCE_WRITE		= 0xF0,
	MSA_ACTION_POLL 			= 0xF1,
	MSA_ACTION_IDEL				= 0xF2,
	
} MSA_ACTION;

typedef enum {
	MSA_DEBUG_LOG_DISABLE	= 0,
	MSA_DEBUG_LOG_ENABLE	= 1,
	MSA_DEBUG_LOG_CLEAR		= 0xc,
} MSA_DEBUG_LOG_OP;

typedef enum {
	MSA_LUT_TYPE_NONE		= 0,
	MSA_LUT_TYPE_LOW_TEMP	= 1,
	MSA_LUT_TYPE_MID_TEMP	= 2,
	MSA_LUT_TYPE_HI_TEMP	= 3,
	MSA_LUT_TYPE_MAX,
}MSA_LUT_TYPE;

typedef struct
{
	uint8_t                  *ptr;
	uint8_t	code             *page_flash_base;	// MSA_FLASH_ADDR_PAGE
	uint8_t                   len;
	// indicates read-only or read-write byte
	// 0: read-only
	// 1: read-write
	uint8_t                    *msa_writable_mask;  	  //0:read-only, 1:read-write
	uint8_t                    *vendor_writable_mask;     //0:read-only, 1:read-write
	uint8_t 				   *eep_mask;  		          //0:power off not save, 1:power off save
}QSFP_MSA_PAGE_T;

typedef struct
{
	QSFP_MSA_PAGE_T    lower_page; // [MSA_PAGE_SIZE];
	QSFP_MSA_PAGE_T    pages[MSA_PAGE_MAX];
	MSA_OP_TYPE        op;		// current op: read or write
	uint8_t            offset;	// current op offset
}QSFP_MSA_DRV_DATA;

typedef struct
{
	uint8_t page;   //bit7:1---buff has value,0--- buff do't have value,bit0~bit6:replace the value of page
	uint8_t offset;
	uint8_t value;
}MSA_WRTTE_EEP_VALUE_T;

typedef struct
{
	MSA_WRTTE_EEP_VALUE_T buff[MSA_PAGE_SIZE];
	uint8_t num;
}MSA_WRITE_EEP_BUFF_T;

extern QSFP_MSA_DRV_DATA msa_drv_data;

void 	 MSA_init(void);

void 	 MSA_write_eep_poll();

void 	 MSA_set_control_drv_mask(uint16_t value);

void 	 MSA_control_byte_poll();

void 	 MSA_factory_reset(void);

void 	 MSA_write_eep(uint8_t offset, const uint8_t value);

bool     MSA_write(uint8_t offset, const uint8_t value);

uint8_t  MSA_read(uint8_t offset);

uint8_t  MSA_check_code(uint8_t *start, uint8_t len);

uint16_t MSA_get_poll_timestamp();

uint16_t MSA_get_control_drv_mask();

QSFP_MSA_PAGE_T	* MSA_get_page_ptr(MSA_PAGE_NUM pg_num);


#include "Efm8lb12_eep_interface.h"
#include "msa_page_lower.h"
#include "msa_page0.h"
#include "msa_page1.h"
#include "msa_page2.h"
#include "msa_page3.h"
#include "msa_page6.h"
#include "msa_page4.h"
#include "msa_page5.h"
#include "msa_page9.h"
#include "msa_page10.h"

#endif
