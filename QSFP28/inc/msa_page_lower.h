/*
 * msa_page_lower.h
 *
 *  Created on: 2017-10-9
 *      Author: cuilin
 */

#ifndef MSA_PAGE_LOWER_H_
#define MSA_PAGE_LOWER_H_

#ifdef MSA_PAGE_LOWER_PAGE

//channel    mask_offset
//	0			 3
//	1			 2
//	2			 1
//	3			 0
#define GET_OFFSET_MASK(x)  	  (3 - (x & 0x3))
#define GET_MASK_BY_CHANNEL(x, y) ((y % 2) ? (x & 0xF):(x >> 4))

#define GET_ALL_TX_LOS(x)  	 ((x & 0xF0) >> 4)
#define GET_ALL_RX_LOS(x)  	 ((x & 0x0F) >> 0)
#define GET_ALL_TX_LOL(x)  	 ((x & 0xF0) >> 4)
#define GET_ALL_RX_LOL(x)  	 ((x & 0x0F) >> 0)
#define GET_ALL_TX_FAULT(x)  ((x & 0x0F) >> 0)

typedef enum
{
	MSA_DRV_VAL_HW		= 0x0,	// hardware value
	MSA_DRV_VAL_NORM	= 0x1,	// normalized value
}MSA_DRV_VAL_TYPE;

typedef struct
{
	uint8_t intr_type;
	uint8_t intr_way;
}INTR_CONFIG;

typedef enum
{
	POLLING_WAY = 0x01,
	INTR_WAY    = 0x02,
}FLAG_WAY;


typedef struct
{
	uint8_t los;  	//byte 100
	uint8_t fault;  //byte 101
	uint8_t lol;    //byte 102
	uint8_t temp;   //byte 103
	uint8_t vcc;    //byte 104
}MSA_PAGE_LOWER_MASK_T;

typedef struct
{
	int16_t  temp;  	//byte 22, 23
	uint8_t  resv0[2];  //byte 24, 25
	uint16_t vcc;		//byte 26, 27
	uint8_t  resv1[6];  //byte 28, 29, 30, 31, 32, 33
	uint16_t rssi[MSA_CHANNEL_NUM];  	//byte 34,35,36,37,38,39,40,41
	uint16_t txbias[MSA_CHANNEL_NUM];	//byte 42,43,44,45,46,47,48,49
	uint16_t txpower[MSA_CHANNEL_NUM];  //byte 50,51,52,53,54,55,56,57
}MSA_PAGE_LOWER_MON_T;

typedef struct
{
	uint8_t	 txrx_los;  //byte 3
	uint8_t	 tx_fault;	//byte 4
	uint8_t  txrx_lol;	//byte 5
	uint8_t	 temp;    	//byte 6
	uint8_t	 vcc;	  	//byte 7
	uint8_t  resv0;		//byte 8
	uint16_t rx_power;  //byte 9,  10
	uint16_t tx_bias;	//byte 11, 12
	uint16_t tx_power;	//byte 13, 14
}MSA_PAGE_LOWER_ISR_FLAG_T;

/***************************************PAGE LOWER******************************/

/*Byte Addr	bit	name	    description*/
/*0		      identifier	11h表示QSFP28*/
#define MSA_OFFSET_LOWER_IDENTIFIER            0

/*
1		revision 	07h表示8636 rev2.7
*/
#define MSA_OFFSET_LOWER_REVISION              1

/*
2	7-3	reserved
	2	flat_mem	    0表示paging（1表示只有page0可用）
	1	intl	        显示intl中断 Pin的状态
	0	data-not-ready	显示模块数据和monitor状态未准备好。上电后保持高电平
	                    直到所有monitor状态正常工作
*/
#define MSA_OFFSET_LOWER_STATUS                2
	#define MSA_PAGE_LOWER_FLAT_MEM           (0x04)
	#define MSA_PAGE_LOWER_INTL_CLEAR		  (~0x02)
	#define MSA_PAGE_LOWER_INTL_SET			  (0x02)
	#define MSA_LOWER_PAGE_DATA_NOT_READY     (0x01)
	#define MSA_LOWER_PAGE_DATA_READY     	  (~0x01)

/*
    3	7	L-Tx4 LOS	4个TX通道的LOS标志位，状态位锁定直到被fixed side
                        读取操作或者ResetL Pin复位（可选）,正常工作时状态为0
    	6	L-Tx3 LOS
    	5	L-Tx2 LOS
    	4	L-Tx1 LOS
    	3	L-Rx4 LOS	4个RX通道的LOS标志位，状态位锁定直到被读取操作
    	                清除或者ResetL Pin复位,正常工作时状态为0
    	2	L-Rx3 LOS
    	1	L-Rx2 LOS
    	0	L-Rx1 LOS
*/
#define MSA_OFFSET_LOWER_LOS                   3
	#define MSA_PAGE_LOWER_TX_LOS_SET(i)		(1 << (4+i))
	#define MSA_PAGE_LOWER_RX_LOS_SET(i)		(1 << i)

/*
    4	7	L-Tx4 adapt EQ Fault	4个TX通道的adapt EQ Fault标志位
    	6	L-Tx3 adapt EQ Fault
    	5	L-Tx2 adapt EQ Fault
    	4	L-Tx1 adapt EQ Fault
    	3	L-Tx4 Fault	            4个TX通道的Fault标志位，状态位锁定直到被读取操作清除
    	                            或者ResetL Pin复位（可选）,正常工作时状态为0
    	2	L-Tx3 Fault
    	1	L-Tx2 Fault
    	0	L-Tx1 Fault
*/
#define MSA_OFFSET_LOWER_TX_FAULT                 4
	#define MSA_PAGE_LOWER_TX_FAULT_SET(i)		(1 << i)

/*
5	7	L-Tx4 LOL	4个TX通道的LOL标志位，状态位锁定直到被fixed side读取操作或者
                    ResetL Pin复位（可选）,正常工作时状态为0
	6	L-Tx3 LOL
	5	L-Tx2 LOL
	4	L-Tx1 LOL
	3	L-Rx4 LOL	4个RX通道的LOL标志位，状态位锁定直到被读取操作清除
	                或者ResetL Pin复位,正常工作时状态为0
	2	L-Rx3 LOL
	1	L-Rx2 LOL
	0	L-Rx1 LOL
*/
#define MSA_OFFSET_LOWER_LOL                   5

/*
6	7	L-Temp High Alarm	温度报警和警告位，状态位锁定直到被读取操作清除
                            或者ResetL Pin复位（可选）,正常工作时状态为0
	6	L-Temp low Alarm
	5	L-Temp High warning
	4	L-Temp low warning
	3-1	reserved
	0	Initializationcomplete flag

*/
#define MSA_OFFSET_LOWER_L_TEMP                6

/*
7	7	L-Vcc High Alarm	电压报警和警告位，状态位锁定直到被读取操作清除或者
                            ResetL Pin复位（可选）,正常工作时状态为0
	6	L-Vcc low Alarm
	5	L-Vcc High warning
	4	L-Vcc low warning
	3-0	reserved
*/
#define MSA_OFFSET_LOWER_L_VCC                 7

/*
8		reserved
*/
#define MSA_OFFSET_LOWER_RESERVED_BYTE8        8

/*
9	7	L-Rx1 power High Alarm	通道1RX功率报警和警告位，状态位锁定直到被读取操
                                作清除或者ResetL Pin复位（可选）,正常工作时状态为0
	6	L-Rx1 power low Alarm
	5	L-Rx1 power High warning
	4	L-Rx1 power low warning
	3	L-Rx2 power High Alarm	通道2RX功率报警和警告位，状态位锁定直到被读取操作
	                            清除或者ResetL Pin复位（可选）,正常工作时状态为0
	2	L-Rx2 power low Alarm
	1	L-Rx2 power High warning
	0	L-Rx2 power low warning
*/
#define MSA_OFFSET_LOWER_RX1_RX2_ALM_WARNING   9
	#define MSA_PAGE_LOWER_HIGH_ALARM_RX_POWER_MASK(i)	(1 << (4 * i + 3))
	#define MSA_PAGE_LOWER_LOW_ALARM_RX_POWER_MASK(i)	(1 << (4 * i + 2))
	#define MSA_PAGE_LOWER_HIGH_WARN_RX_POWER_MASK(i)	(1 << (4 * i + 1))
	#define MSA_PAGE_LOWER_LOW_WARN_RX_POWER_MASK(i)	(1 << (4 * i + 0))

/*
    10	7	L-Rx3 power High Alarm	通道3RX功率报警和警告位，状态位锁定直到被读取操作
                                    清除或者ResetL Pin复位（可选）,正常工作时状态为0
    	6	L-Rx3 power low Alarm
    	5	L-Rx3 power High warning
    	4	L-Rx3 power low warning
    	3	L-Rx4 power High Alarm	通道4RX功率报警和警告位，状态位锁定直到被读取操作
    	                            清除或者ResetL Pin复位（可选）,正常工作时状态为0
    	2	L-Rx4 power low Alarm
    	1	L-Rx4 power High warning
    	0	L-Rx4 power low warning
    */
#define MSA_OFFSET_LOWER_RX3_RX4_ALM_WARNING   10

/*
    11	7	L-Tx1 bias High Alarm	通道1bias电流报警和警告位，状态位锁定直到被读取操作
                                    清除或者ResetL Pin复位（可选）,正常工作时状态为0
    	6	L-Tx1 bias low Alarm
    	5	L-Tx1 bias High warning
    	4	L-Tx1 bias low warning
    	3	L-Tx2 bias High Alarm	通道2bias电流报警和警告位，状态位锁定直到被读取操作
                                	清除或者ResetL Pin复位（可选）,正常工作时状态为0
    	2	L-Tx2 bias low Alarm
    	1	L-Tx2 bias High warning
    	0	L-Tx2 bias low warning
*/
#define MSA_OFFSET_LOWER_TX1_TX2_BIAS          11
	#define MSA_PAGE_LOWER_HIGH_ALARM_TX_BIAS_MASK(i)	(1 << (4 * i + 3))
	#define MSA_PAGE_LOWER_LOW_ALARM_TX_BIAS_MASK(i)	(1 << (4 * i + 2))
	#define MSA_PAGE_LOWER_HIGH_WARN_TX_BIAS_MASK(i)	(1 << (4 * i + 1))
	#define MSA_PAGE_LOWER_LOW_WARN_TX_BIAS_MASK(i)		(1 << (4 * i + 0))

/*
12	7	L-Tx3 bias High Alarm	通道3bias电流报警和警告位，状态位锁定直到被读取操作
                                清除或者ResetL Pin复位（可选）,正常工作时状态为0
	6	L-Tx3 bias low Alarm
	5	L-Tx3 bias High warning
	4	L-Tx3 bias low warning
	3	L-Tx4 bias High Alarm	通道4bias电流报警和警告位，状态位锁定直到被读取操作
                            	清除或者ResetL Pin复位（可选）,正常工作时状态为0
	2	L-Tx4 bias low Alarm
	1	L-Tx4 bias High warning
	0	L-Tx4 bias low warning
*/
#define MSA_OFFSET_LOWER_TX3_TX4_BIAS          12

/*
        13	7	L-Tx1 power High Alarm	通道1TX功率报警和警告位，状态位锁定直到被读取操作
                                        清除或者ResetL Pin复位（可选）,正常工作时状态为0
        	6	L-Tx1 power low Alarm
        	5	L-Tx1 power High warning
        	4	L-Tx1 power low warning
        	3	L-Tx2 power High Alarm	通道2TX功率报警和警告位，状态位锁定直到被读取操作
                                    	清除或者ResetL Pin复位（可选）,正常工作时状态为0
        	2	L-Tx2 power low Alarm
        	1	L-Tx2 power High warning
        	0	L-Tx2 power low warning
*/
#define MSA_OFFSET_LOWER_TX1_TX2_ALM_WARNING   13
	#define MSA_PAGE_LOWER_HIGH_ALARM_TX_POWER_MASK(i)	(1 << (4 * i + 3))
	#define MSA_PAGE_LOWER_LOW_ALARM_TX_POWER_MASK(i)	(1 << (4 * i + 2))
	#define MSA_PAGE_LOWER_HIGH_WARN_TX_POWER_MASK(i)	(1 << (4 * i + 1))
	#define MSA_PAGE_LOWER_LOW_WARN_TX_POWER_MASK(i)	(1 << (4 * i + 0))

/*
        14	7	L-Tx3 power High Alarm	通道3TX功率报警和警告位，状态位锁定直到被读取操作
                                        清除或者ResetL Pin复位（可选）,正常工作时状态为0
        	6	L-Tx3 power low Alarm
        	5	L-Tx3 power High warning
        	4	L-Tx3 power low warning
        	3	L-Tx4 power High Alarm	通道4TX功率报警和警告位，状态位锁定直到被读取操作
                                    	清除或者ResetL Pin复位（可选）,正常工作时状态为0
        	2	L-Tx4 power low Alarm
        	1	L-Tx4 power High warning
        	0	L-Tx4 power low warning
*/
#define MSA_OFFSET_LOWER_TX3_TX4_ALM_WARNING   14
/*
15-21	reserved
*/
#define MSA_OFFSET_LOWER_RESERVED_BYTE15       15

/*
22		temp MSB	有符号，每个字节表示1/256C,所以表达范围为-128C--+128C
23		temp LSB
*/
#define MSA_OFFSET_LOWER_TEMP_MSB              22
#define MSA_OFFSET_LOWER_TEMP_LSB              23

/*
24-25	reserved
*/
#define MSA_OFFSET_LOWER_RESERVED_BYTE24       24

/*
26		Vol MSB	无符号，每个字节表示100uV,所以表达范围为0-6.55V
27		Vol LSB
*/
#define MSA_OFFSET_LOWER_VOL_MSB               26
#define MSA_OFFSET_LOWER_VOL_LSB               27

/*
28-33	reserved
*/
#define MSA_OFFSET_LOWER_RESERVED_BYTE28       28

/*
34		Rx1 Power MSB	无符号，每个字节表示0.1uW，表达范围0-6.55mW，-40--8.2dbm，
                        平均功率或OMA可选-bype220bit3，最低精度3dBm	0
35		Rx1 Power LSB		0
*/
#define MSA_OFFSET_LOWER_RX1_PWR_MSB           34
#define MSA_OFFSET_LOWER_RX1_PWR_LSB           35
/*
36		Rx2 Power MSB		0
37		Rx2 Power LSB		0
*/
#define MSA_OFFSET_LOWER_RX2_PWR_MSB           36
#define MSA_OFFSET_LOWER_RX2_PWR_LSB           37
/*
38		Rx3 Power MSB		0
39		Rx3 Power LSB		0
*/
#define MSA_OFFSET_LOWER_RX3_PWR_MSB           38
#define MSA_OFFSET_LOWER_RX3_PWR_LSB           39
/*
40		Rx4 Power MSB		0
41		Rx4 Power LSB		0
*/
#define MSA_OFFSET_LOWER_RX4_PWR_MSB           40
#define MSA_OFFSET_LOWER_RX4_PWR_LSB           41

/*
42		Tx1 Bias MSB	无符号，每个字节表示2uA，表达范围0-131mA最低精度10%	0
43		Tx1 Bias LSB		0
*/
#define MSA_OFFSET_LOWER_TX1_BIAS_MSB           42
#define MSA_OFFSET_LOWER_TX1_BIAS_LSB           43
/*
44		Tx2 Bias MSB		0
45		Tx2 Bias LSB		0
*/
#define MSA_OFFSET_LOWER_TX2_BIAS_MSB           44
#define MSA_OFFSET_LOWER_TX2_BIAS_LSB           45

/*
46		Tx3 Bias MSB		0
47		Tx3 Bias LSB		0
*/
#define MSA_OFFSET_LOWER_TX3_BIAS_MSB           46
#define MSA_OFFSET_LOWER_TX3_BIAS_LSB           47

/*
48		Tx4 Bias MSB		0
49		Tx4 Bias LSB		0
*/
#define MSA_OFFSET_LOWER_TX4_BIAS_MSB           48
#define MSA_OFFSET_LOWER_TX4_BIAS_LSB           49

/*
50		Tx1 Power MSB	无符号，每个字节表示0.1uW，表达范围0-6.55mW，-40--8.2dbm，
                        平均功率，最低精度3dBm	0
51		Tx1 Power LSB		0
*/
#define MSA_OFFSET_LOWER_TX1_PWR_MSB           50
#define MSA_OFFSET_LOWER_TX1_PWR_LSB           51
/*
52		Tx2 Power MSB		0
53		Tx2 Power LSB		0
*/
#define MSA_OFFSET_LOWER_TX2_PWR_MSB           52
#define MSA_OFFSET_LOWER_TX2_PWR_LSB           53
/*
54		Tx3 Power MSB		0
55		Tx3 Power LSB		0
*/
#define MSA_OFFSET_LOWER_TX3_PWR_MSB           54
#define MSA_OFFSET_LOWER_TX3_PWR_LSB           55
/*
56		Tx4 Power MSB		0
57		Tx4 Power LSB		0
*/
#define MSA_OFFSET_LOWER_TX4_PWR_MSB           56
#define MSA_OFFSET_LOWER_TX4_PWR_LSB           57
/*
58-85 reserved
*/
#define MSA_OFFSET_LOWER_RESERVED_BYTE58       58
/*
86	7-4	reserved
	3	tx4_disable	写1关闭TX4
	2	tx3_disable	写1关闭TX3
	1	tx2_disable	写1关闭TX2
	0	tx1_disable	写1关闭TX1
*/
#define MSA_OFFSET_LOWER_TX_DISABLE            86

/*
87	7-0	rx_rate_select	Rx速率选择，不支持速率选择
*/
#define MSA_OFFSET_LOWER_RX_RATE_SELECT        87

/*
88	7-0	tx_rate_select	tx速率选择，不支持速率选择
*/
#define MSA_OFFSET_LOWER_TX_RATE_SELECT        88

/*
    89-92		rx4-1_application_sel
*/
#define MSA_OFFSET_LOWER_RX4_APPL_SEL          89
#define MSA_OFFSET_LOWER_RX3_APPL_SEL          90
#define MSA_OFFSET_LOWER_RX2_APPL_SEL          91
#define MSA_OFFSET_LOWER_RX1_APPL_SEL          92

/*
93	7-3	reserved
	2	hign power class enable
	1	power_set
	0	power_over-ride
*/
#define MSA_OFFSET_LOWER_PWR_CTL               93
	#define MSA_LOWER_PWR_CTL_OVERRIDE  	0x1
	#define MSA_LOWER_PWR_CTL_LPMODE    	0x2

/*
94		tx4_application_sel
95		tx3_application_sel
96		tx2_application_sel
97		tx1_application_sel
*/
#define MSA_OFFSET_LOWER_TX4_APPL_SEL          94
#define MSA_OFFSET_LOWER_TX3_APPL_SEL          95
#define MSA_OFFSET_LOWER_TX2_APPL_SEL          96
#define MSA_OFFSET_LOWER_TX1_APPL_SEL          97
/*
98	7-4	tx4-1-CDR-control
	3-0	rx4-1-CDR-control
*/
#define MSA_OFFSET_LOWER_TX_CDR_CTL            98

#define MSA_OFFSET_LOWER_RESERVED_BYTE99       99

/*
100
    7-4 M-Tx4-1 LOS Mask
    3-0 M-Rx4-1 LOS Mask
*/
#define MSA_OFFSET_LOWER_LOS_MASK              100
	#define MSA_PAGE_LOWER_TX_LOS_MASK(i) 		(1 << (4+i))
	#define MSA_PAGE_LOWER_RX_LOS_MASK(i) 		(1 << i)

/*
101 tx adapt EQ fault mask
*/
#define MSA_OFFSET_LOWER_TX_FAULT_MASK         101
	#define MSA_PAGE_LOWER_TX_FAULT_MASK(i)		(1 << i)

/*
102
7-4		4个tx CDR LOL mask
3-0		4个rx CDR LOL mask
*/
#define MSA_OFFSET_LOWER_CDR_LOL_MASK          102
	#define MSA_PAGE_LOWER_TX_LOL_MASK(i) 		(1 << (4+i))
	#define MSA_PAGE_LOWER_RX_LOL_MASK(i) 		(1 << i)

/*
103
7-4		高低温alarm和warning的masking bit
3-0	    reserved
*/
#define MSA_OFFSET_LOWER_HIGH_LOW_TEMP_MASK    103
	#define MSA_PAGE_LOWER_HIGH_ALARM_TEMP_MASK		0x80
	#define MSA_PAGE_LOWER_LOW_ALARM_TEMP_MASK		0x40
	#define MSA_PAGE_LOWER_HIGH_WARN_TEMP_MASK		0x20
	#define MSA_PAGE_LOWER_LOW_WARN_TEMP_MASK		0x10

/*
104
7-4		high&low VCC alarm和warning的masking bit
3-0		reserved
*/
#define MSA_OFFSET_LOWER_HIGH_LOW_VCC_MASK     104
	#define MSA_PAGE_LOWER_HIGH_ALARM_VCC_MASK		0x80
	#define MSA_PAGE_LOWER_LOW_ALARM_VCC_MASK		0x40
	#define MSA_PAGE_LOWER_HIGH_WARN_VCC_MASK		0x20
	#define MSA_PAGE_LOWER_LOW_WARN_VCC_MASK		0x10

/*
105-107 reserved
*/
#define MSA_OFFSET_LOWER_RESERVED_BYTE105      105

/*
108		propagation delay	0000h，表示无延迟
109
*/
#define MSA_OFFSET_LOWER_PRO_DELAY_MSB         108
#define MSA_OFFSET_LOWER_PRO_DELAY_LSB         109

/*
110	7-4	advanced low power	0000表示低于1.5w不支持
	3	far side manegement	0，不支持
	2-0	vcc expend	不支持3.3V以下电压
*/
#define MSA_OFFSET_LOWER_BYTE110               110

/*
111-112  Assigned for use by PCI Express
*/
#define MSA_OFFSET_LOWER_USE_BY_PCI_MSB            111
#define MSA_OFFSET_LOWER_USE_BY_PCI_LSB            112
/*
113	7	reserved
	6-4	far end	000：unspecified
	3-0	near end	0000:4通道implement
*/
#define MSA_OFFSET_LOWER_USE_BYTE113           113

/*
114-118 reserved
*/
#define MSA_OFFSET_LOWER_RESERVED_BYTE114      114

/*
119-122		password change entry area
*/
#define MSA_OFFSET_LOWER_PSW_CHG_ENTRY_AREA_BYTE119    119
#define MSA_OFFSET_LOWER_PSW_CHG_ENTRY_AREA_BYTE120    120
#define MSA_OFFSET_LOWER_PSW_CHG_ENTRY_AREA_BYTE121    121
#define MSA_OFFSET_LOWER_PSW_CHG_ENTRY_AREA_BYTE122    122

/*
123-126		password entry area
*/
#define MSA_OFFSET_LOWER_PSW_ENTRY_AREA_BYTE123        123
#define MSA_OFFSET_LOWER_PSW_ENTRY_AREA_BYTE124        124
#define MSA_OFFSET_LOWER_PSW_ENTRY_AREA_BYTE125        125
#define MSA_OFFSET_LOWER_PSW_ENTRY_AREA_BYTE126        126

/*
127		page_select
*/
#define MSA_OFFSET_LOWER_PAGE_SELECT_BYTE       127

#define MSA_LOWER_PAGE_DATA_RAM_SIZE			128
#define MSA_LOWER_PAGE_DATA_ROM_SIZE			128

#if defined(EFM8LB12E64_100G_SFP_AOC_101)
#define MSA_OFFSET_LOWER_PWR_CTL_HIGH			0x0
#endif

#if defined(EFM8LB12E64_100G_SFP_CWDM4_202) || defined(EFM8LB12E64_100G_SFP_CWDM4_206)
#define MSA_OFFSET_LOWER_PWR_CTL_HIGH			0x0
#endif

#if defined(EFM8LB12E64_100G_SFP_LR4_301)
#define MSA_OFFSET_LOWER_PWR_CTL_HIGH			0x4
#endif

/*******************************************************************************/
bool 	MSA_lower_page_read_ready_intl();

void	MSA_lower_page_update_intl_pin_staus(void);

void 	MSA_page_lower_check_passwd();

void	MSA_page_lower_init(QSFP_MSA_PAGE_T *msa_data);

void 	MSA_page_lower_set_boot_status(bool enable);

void 	MSA_page_lower_set_intr_flag();

void 	MSA_page_lower_eeprom_init(void);

void 	MSA_page_lower_set_low_power_mode();

void 	MSA_page_lower_on_poll();

void 	MSA_page_lower_load_to_ram();

uint8_t MSA_lower_page_get_passwd_status();

uint8_t MSA_page_lower_get_page_select();

uint8_t MSA_page_lower_read_by_isr(uint8_t offset);

uint8_t MSA_page_lower_write_by_isr(uint8_t offset, uint8_t value);

uint8_t MSA_page_lower_intr_status_mask_handle(MSA_OP_TYPE type, uint8_t intr_mask);


#endif // MSA_PAGE_LOWER

#endif /* MSA_PAGE_LOWER_H_ */
