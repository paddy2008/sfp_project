#ifndef __100g_sfp_EFM8LB1_common_h__
#define __100g_sfp_EFM8LB1_common_h__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "dac_1.h"
#include "version.h"
#include "Efm8lb12_timer.h"
#include "Efm8lb12_eep_interface.h"
#include <SI_EFM8LB1_Register_Enums.h>

#define  SYSCLK             	(72000000)     		// System clock frequency in Hz
#define  GOS_OK             	(0xFE)
#define  GOS_FAIL		    	(0xFF)
#define  LINE_FEED   			(0xA)   //换行符
#define  BACKSPACE          	(0x8)   //退格符
#define  CARRIAGE_RETURN    	(0xD)   //回车符
#define  BLANKSPACE				(0x20)  //空格符
#define  ESCAPE      			(0x1B)  //键盘上的向上箭头符
#define  UNDERLINE 	        	(0x5F)  //下划线
#define  INVALID_CHAR       	(0xBE)
#define  CHECK_POWER_MIN(x)  	(((x) < 0.0) ? ((x) = 0.0):((x)))
#define  CHECK_POWER_MAX(x)  	(((x) > 6.5535) ? ((x) = 6.5535):((x)))
#define  GET_BIT(x, y)			(((x) >> (y)) & 0x01)
#define  SET_BIT(x, y)			((x) | (1 << (y)))
#define  CLEAR_BIT(x, y)		((x) & (~(1 << (y))))
#define  CLEAR_MSB_4BITS(x)		((x) & 0x0F)
#define  CLEAR_LSB_4BITS(x)		((x) & 0xF0)
#define  OUTPUT_ENABLE      	(1)
#define  OUTPUT_DISABLE     	(0)
#define  OFFSET_INDEX(offset)	(offset & 0x7f)
#define  U16_MSB(val)			((uint8_t)((val) >> 8))
#define  U16_LSB(val)			((uint8_t)((val) & 0xFF))
#define  U8_MSB_4BITS(val)		(((val) >> 4))
#define  U8_LSB_4BITS(val)		(((val) & 0xF))
#define  REVERSE_LSB_4BITS(x)	((GET_BIT((x), 3) << 0) | (GET_BIT((x), 2) << 1) | (GET_BIT((x), 1) << 2) | (GET_BIT((x), 0) << 3))
#define  MAX(x, y) 				(((x) > (y)) ? (x) : (y))
#define  MIN(x, y)				(((x) > (y)) ? (y) : (x))
#define  ASSERT(exp) 			((exp) == NULL)
#define  COM_LOCK_SEM(sem)     	((sem)--)
#define  COM_UNLOCK_SEM(sem)   	((sem)++)
#define  COM_GET_LOCK(sem)     	(sem)


#define  AVERAGE_MAX_COUNTER 	(8)

typedef struct
{
	uint8_t   start;
	uint16_t  counter;
	uint16_t  max_counter;
}UART_RSSI_SHOW_CMD;


#ifdef LITTLE_ENDIAN
#define  TO_BE(v)		(((v) & 0xFF) << 8 | ((v) >> 8) & 0xFF)
#else
#define  TO_BE(v)		(v)
#endif

void 	 com_drv_on_poll(void);

uint16_t com_average_value16(uint16_t *buffer, uint8_t length);

bit 	 get_bit(unsigned char *p, uint8_t bit_pos);
bit 	 get_bit_by_isr(uint8_t *p, uint8_t bit_pos);
void 	 set_bit(unsigned char *p, uint8_t bit_pos, bit v);

uint8_t  get_year(const char *date);
uint8_t  get_month(const char *date);
uint8_t  get_day(const char *date);
uint8_t  get_hour(const char *time);
uint8_t  get_minute(const char *time);
uint8_t  get_second(const char *time);

#endif

