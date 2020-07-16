/*
 * msa_page6.h
 *
 *  Created on: 2017-10-10
 *      Author: cuilin
 */

#ifndef MSA_PAGE6_H_
#define MSA_PAGE6_H_

#ifdef MSA_PAGE_6

#define MSA_P6_LT_TEMP_L				(0x80)
#define MSA_P6_LT_TEMP_H				(0x81)
#define MSA_P6_RT_TEMP_L				(0xB4)
#define MSA_P6_RT_TEMP_H				(0xB5)
#define MSA_P7_HT_TEMP_L				(0x80)
#define MSA_P7_HT_TEMP_H				(0x81)

#define MSA_P7_TEC_PID_TARGET			(0xFE)

#define MSA_P8_RSSI0_DARKA_THLD_MSB		(0x91)
#define MSA_P8_RSSI0_DARKA_THLD_LSB		(0x92)
#define MSA_P8_RSSI0_DARKD_THLD_MSB		(0x93)
#define MSA_P8_RSSI0_DARKD_THLD_LSB		(0x94)
#define MSA_P8_RSSI1_DARKA_THLD_MSB		(0x95)
#define MSA_P8_RSSI1_DARKA_THLD_LSB		(0x96)
#define MSA_P8_RSSI1_DARKD_THLD_MSB		(0x97)
#define MSA_P8_RSSI1_DARKD_THLD_LSB		(0x98)
#define MSA_P8_RSSI2_DARKA_THLD_MSB		(0x99)
#define MSA_P8_RSSI2_DARKA_THLD_LSB		(0x9A)
#define MSA_P8_RSSI2_DARKD_THLD_MSB		(0x9B)
#define MSA_P8_RSSI2_DARKD_THLD_LSB		(0x9C)
#define MSA_P8_RSSI3_DARKA_THLD_MSB		(0x9D)
#define MSA_P8_RSSI3_DARKA_THLD_LSB		(0x9E)
#define MSA_P8_RSSI3_DARKD_THLD_MSB		(0x9F)
#define MSA_P8_RSSI3_DARKD_THLD_LSB		(0xA0)



typedef struct {
	uint16_t		bias;
	uint16_t		mod;
	int16_t			cross;
}MSA_LASER_CH_CFG_T;

typedef struct {
	uint16_t		txbias;	// in 0.1mA
	uint16_t		rssi;	// in 0.1uW
	uint16_t		txpwr;	// in 0.1uW
}MSA_LASER_CH_STAT_T;

typedef struct {
	MSA_LASER_CH_STAT_T	stat;
	MSA_LASER_CH_CFG_T	config;
}MSA_LASER_CH_T;

typedef struct {
	int16_t 		temp;	// MSA_DATA_LB and MSA_DATA_HB
	uint16_t		vcc;
	MSA_LASER_CH_T	ch[MSA_CHANNEL_NUM];
}MSA_LUT_T;

typedef struct {
	int16_t		temp;
	int16_t		bias;
	int16_t		mod;
	int16_t		cross;
}SFP_DRV_LASER_CFG_T;
	
/*Ddmi cali parameter*/
typedef struct {
	uint16_t txpower1;	// in 0.1uW
	uint16_t bias1;		// in 0.1mA
	uint16_t txpower2;	// in 0.1uW
	uint16_t bias2;		// in 0.1mA
}TXPOWER_CALI_PARA_T;

typedef struct {
	uint16_t rxpower1;	// in 0.1uW
	uint16_t rssi1;		// in uA
	uint16_t rxpower2;	// in 0.1uW
	uint16_t rssi2;		// in uA
}RXPOWER_CALI_PARA_T;

typedef struct{
	uint16_t bias_offset1;  // in 0.1mA
	uint16_t bias_offset2;
}BIAS_CALI_PARA_T;

typedef struct {
	BIAS_CALI_PARA_T	bias;
	RXPOWER_CALI_PARA_T	rxpower;
	TXPOWER_CALI_PARA_T	txpower;
}CURRENT_CALI_PARA_T;

typedef struct {
	int16_t temp;	// in 1/256 C
	int16_t vcc;	// in 0.1mV
	CURRENT_CALI_PARA_T current[MSA_CHANNEL_NUM];
}DDMI_CALI_PARA_T;

typedef struct{
	uint16_t losa;
	uint16_t losd;
}RX_LOS_THLD;

typedef struct{
	uint8_t        rx_los_mod;
	RX_LOS_THLD    rx_los_thld[MSA_CHANNEL_NUM];
}AVG_RX_LOS_THLD;


typedef struct{
	uint16_t darka;
	uint16_t darkd;
}RSSI_DARK;

typedef struct{
	RSSI_DARK  rssi_dark_thld[MSA_CHANNEL_NUM];
}RSSI_DARK_THLD;

typedef struct{
	AVG_RX_LOS_THLD avg_rx_los;
	RSSI_DARK_THLD  rssi_dark;
}RX_CALI_PARA_T;



void		MSA_p67_init(QSFP_MSA_PAGE_T *page);

void 	    MSA_p67_eeprom_init(void);

void 	    MSA_p67_resume_normal_mode();

void		MSA_p67_on_poll();

bool		MSA_p67_check_apc_lut_tab(SFP_DRV_LASER_CFG_T *lut);

uint8_t    *MSA_p67_get_drv_lut();

uint8_t  	MSA_p67_set_lut_now(uint8_t channel, MSA_LUT_TYPE type);

uint8_t		MSA_p67_read_by_isr(uint8_t pg_num, uint8_t offset);

uint8_t		MSA_p67_write_by_isr(uint8_t pg_num, uint8_t offset, uint8_t value);

uint8_t 	MSA_p67_handler(MSA_ACTION action, uint8_t offset, uint8_t *pValue);

DDMI_CALI_PARA_T 	  *MSA_p67_get_ddmi_cali_cfg();

RX_CALI_PARA_T 	  	  *MSA_p67_get_rx_cali_cfg();

#endif

#endif /* MSA_PAGE6_H_ */
