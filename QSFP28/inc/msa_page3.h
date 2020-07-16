/*
 * msa_page3.h
 *
 *  Created on: 2017-11-7
 *      Author: cuilin
 */

#ifndef MSA_PAGE3_H_
#define MSA_PAGE3_H_

#ifdef MSA_PAGE_3

typedef struct {
	uint16_t	hi_alarm;
	uint16_t	lo_alarm;
	uint16_t	hi_warn;
	uint16_t	lo_warn;
}MSA_PAGE3_THRESHOLD_T;

typedef struct
{
	MSA_PAGE3_THRESHOLD_T  temp;
	MSA_PAGE3_THRESHOLD_T  reserved0;
	MSA_PAGE3_THRESHOLD_T  vcc;
	MSA_PAGE3_THRESHOLD_T  reserved1;
	MSA_PAGE3_THRESHOLD_T  reserved2;
	MSA_PAGE3_THRESHOLD_T  reserved3;
	MSA_PAGE3_THRESHOLD_T  rxpower;
	MSA_PAGE3_THRESHOLD_T  txbias;
	MSA_PAGE3_THRESHOLD_T  txpower;
}MSA_PAGE3_DDMI_THRESHOLD_T;

typedef struct
{
	uint16_t rxpower;
	uint16_t txbias;
	uint16_t txpower;
}MSA_PAGE3_MASK_T;


#define MSA_OFFSET_PAGE_03H_START                           (128)
#define MSA_OFFSET_PAGE_03H_TEMP_HIGH_ALARM_MSB             (128)
#define MSA_OFFSET_PAGE_03H_TEMP_HIGH_ALARM_LSB             (129)

#define MSA_OFFSET_PAGE_03H_TEMP_LOW_ALARM_MSB              (130)
#define MSA_OFFSET_PAGE_03H_TEMP_LOW_ALARM_LSB              (131)

#define MSA_OFFSET_PAGE_03H_TEMP_HIGH_WARNING_MSB           (132)
#define MSA_OFFSET_PAGE_03H_TEMP_HIGH_WARNING_LSB           (133)

#define MSA_OFFSET_PAGE_03H_TEMP_LOW_WARNING_MSB            (134)
#define MSA_OFFSET_PAGE_03H_TEMP_LOW_WARNING_LSB            (135)

#define MSA_OFFSET_PAGE_03H_RESERVED_BYTE136                (136)

#define MSA_OFFSET_PAGE_03H_VCC_HIGH_ALARM_MSB              (144)
#define MSA_OFFSET_PAGE_03H_VCC_HIGH_ALARM_LSB              (145)

#define MSA_OFFSET_PAGE_03H_VCC_LOW_ALARM_MSB               (146)
#define MSA_OFFSET_PAGE_03H_VCC_LOW_ALARM_LSB               (147)

#define MSA_OFFSET_PAGE_03H_VCC_HIGH_WARNING_MSB            (148)
#define MSA_OFFSET_PAGE_03H_VCC_HIGH_WARNING_LSB            (149)

#define MSA_OFFSET_PAGE_03H_VCC_LOW_WARNING_MSB             (150)
#define MSA_OFFSET_PAGE_03H_VCC_LOW_WARNING_LSB             (151)

#define MSA_OFFSET_PAGE_03H_RESERVED_BYTE152                (152)

#define MSA_OFFSET_PAGE_03H_RX_POWER_HIGH_ALARM_MSB         (176)
#define MSA_OFFSET_PAGE_03H_RX_POWER_HIGH_ALARM_LSB         (177)

#define MSA_OFFSET_PAGE_03H_RX_POWER_LOW_ALARM_MSB          (178)
#define MSA_OFFSET_PAGE_03H_RX_POWER_LOW_ALARM_LSB          (179)

#define MSA_OFFSET_PAGE_03H_RX_POWER_HIGH_WARNING_MSB       (180)
#define MSA_OFFSET_PAGE_03H_RX_POWER_HIGH_WARNING_LSB       (181)

#define MSA_OFFSET_PAGE_03H_RX_POWER_LOW_WARNING_MSB        (182)
#define MSA_OFFSET_PAGE_03H_RX_POWER_LOW_WARNING_LSB        (183)

#define MSA_OFFSET_PAGE_03H_TX_BIAS_HIGH_ALARM_MSB          (184)
#define MSA_OFFSET_PAGE_03H_TX_BIAS_HIGH_ALARM_LSB          (185)

#define MSA_OFFSET_PAGE_03H_TX_BIAS_LOW_ALARM_MSB           (186)
#define MSA_OFFSET_PAGE_03H_TX_BIAS_LOW_ALARM_LSB           (187)

#define MSA_OFFSET_PAGE_03H_TX_BIAS_HIGH_WARNING_MSB        (188)
#define MSA_OFFSET_PAGE_03H_TX_BIAS_HIGH_WARNING_LSB        (189)

#define MSA_OFFSET_PAGE_03H_TX_BIAS_LOW_WARNING_MSB         (190)
#define MSA_OFFSET_PAGE_03H_TX_BIAS_LOW_WARNING_LSB         (191)

#define MSA_OFFSET_PAGE_03H_TX_POWER_HIGH_ALARM_MSB     	(192)
#define MSA_OFFSET_PAGE_03H_TX_POWER_HIGH_ALARM_LSB     	(193)

#define MSA_OFFSET_PAGE_03H_TX_POWER_LOW_ALARM_MSB      	(194)
#define MSA_OFFSET_PAGE_03H_TX_POWER_LOW_ALARM_LSB      	(195)

#define MSA_OFFSET_PAGE_03H_TX_POWER_HIGH_WARNING_MSB   	(196)
#define MSA_OFFSET_PAGE_03H_TX_POWER_HIGH_WARNING_LSB   	(197)

#define MSA_OFFSET_PAGE_03H_TX_POWER_LOW_WARNING_MSB     	(198)
#define MSA_OFFSET_PAGE_03H_TX_POWER_LOW_WARNING_LSB     	(199)

#define MSA_OFFSET_PAGE_03H_RESERVED_BYTE200                (200)
#define MSA_OFFSET_PAGE_03H_RESERVED_BYTE208                (208)

/*Tx EQ & Rx Emphasis Magnitude ID*/
#define MSA_OFFSET_PAGE_03H_TX_EQ_AND_RX_EMPH_MAGN_ID       (224)

#define MSA_OFFSET_PAGE_03H_RESERVED_BYTE226                (226)

//tx1 input equalization control
#define MSA_OFFSET_PAGE_03H_TX1_TX2_INPUT_EQUAL_CTL     	(234)
#define MSA_OFFSET_PAGE_03H_TX3_TX4_INPUT_EQUAL_CTL     	(235)
#define MSA_OFFSET_PAGE_03H_RX1_RX2_OUTPUT_EQUAL_CTL    	(236)
#define MSA_OFFSET_PAGE_03H_RX3_RX4_OUTPUT_EQUAL_CTL    	(237)

//rx1 output emphasis control
#define MSA_OFFSET_PAGE_03H_RX1_RX2_OUTPUT_AMPL_CTL     	(238)
#define MSA_OFFSET_PAGE_03H_RX3_RX4_OUTPUT_AMPL_CTL     	(239)
#define MSA_OFFSET_PAGE_03H_RX_TX_SQ                    	(240)
#define MSA_OFFSET_PAGE_03H_RX_OUTPUT_DISABLE           	(241)
#define MSA_OFFSET_PAGE_03H_RX1_RX2_PWR_MASK             	(242)
#define MSA_OFFSET_PAGE_03H_RX3_RX4_PWR_MASK             	(243)
	#define MSA_OFFSET_PAGE_03H_HIGH_ALARM_RX_PWR_MASK      (0x08)
	#define MSA_OFFSET_PAGE_03H_LOW_ALARM_RX_PWR_MASK       (0x04)
	#define MSA_OFFSET_PAGE_03H_HIGH_WARNING_RX_PWR_MASK    (0x02)
	#define MSA_OFFSET_PAGE_03H_LOW_WARNING_RX_PWR_MASK     (0x01)
#define MSA_OFFSET_PAGE_03H_TX1_TX2_BIAS_MASK            	(244)
#define MSA_OFFSET_PAGE_03H_TX3_TX4_BIAS_MASK            	(245)
	#define MSA_OFFSET_PAGE_03H_HIGH_ALARM_TX_BIAS_MASK     (0x08)
	#define MSA_OFFSET_PAGE_03H_LOW_ALARM_TX_BIAS_MASK      (0x04)
	#define MSA_OFFSET_PAGE_03H_HIGH_WARNING_TX_BIAS_MASK   (0x02)
	#define MSA_OFFSET_PAGE_03H_LOW_WARNING_TX_BIAS_MASK    (0x01)

#define MSA_OFFSET_PAGE_03H_TX1_TX2_PWR_MASK             	(246)
#define MSA_OFFSET_PAGE_03H_TX3_TX4_PWR_MASK             	(247)
	#define MSA_OFFSET_PAGE_03H_HIGH_ALARM_TX_PWR_MASK      (0x08)
	#define MSA_OFFSET_PAGE_03H_LOW_ALARM_TX_PWR_MASK       (0x04)
	#define MSA_OFFSET_PAGE_03H_HIGH_WARNING_TX_PWR_MASK    (0x02)
	#define MSA_OFFSET_PAGE_03H_LOW_WARNING_TX_PWR_MASK     (0x01)
#define MSA_OFFSET_PAGE_03H_RESERVED_BYTE248            	(248)
#define MSA_OFFSET_PAGE_03H_MAX                         	(255)



void	MSA_p3_init(QSFP_MSA_PAGE_T *page);

void 	MSA_p3_eeprom_init(void);

void 	MSA_p3_load_to_ram();

uint8_t MSA_p3_read_by_isr(uint8_t offset);

uint8_t MSA_p3_write_by_isr(uint8_t offset, uint8_t value);

MSA_PAGE3_MASK_T *MSA_p3_get_mask();

MSA_PAGE3_DDMI_THRESHOLD_T * MSA_p3_get_ddmi_threshold();


#endif

#endif /* MSA_PAGE3_H_ */



