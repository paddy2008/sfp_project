#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "Efm8lb12_common.h"
#include "Efm8lb12_smbus.h"
#include "Efm8lb12_msa.h"
#include "Efm8lb12_100g_com_drv.h"

uint16_t com_average_value16(uint16_t *buffer, uint8_t length)
{
	uint8_t  i  = 0;
	uint16_t max = 0, min = 0;
	uint32_t sum = 0;

	max = buffer[0];
	min = buffer[0];

	for(i = 0; i < length; i++)
	{
		if(buffer[i] > max)
		{
			max = buffer[i];
		}

		if(buffer[i] < min)
		{
			min = buffer[i];
		}
		sum += buffer[i];
	}

	sum = sum - min - max;

	return (uint16_t)(sum / (length -2));
}

bit get_bit(uint8_t *p, uint8_t bit_pos) //bit_pos is range from 0 ~ 127
{
	bit ret;
	uint8_t byte_pos = bit_pos >> 3;

	if(ASSERT(p))
	{
		return 0;
	}

	ret = (p[byte_pos] >> (bit_pos & 0x7)) & 0x01;

	return ret;
}

bit get_bit_by_isr(uint8_t *p, uint8_t bit_pos)
{
	bit ret;
	uint8_t byte_pos = bit_pos >> 3;

	if(ASSERT(p))
	{
		return 0;
	}

	ret = (p[byte_pos] >> (bit_pos & 0x7)) & 0x01;
	return ret;
}

void set_bit(unsigned char *p, uint8_t bit_pos, bit v)
{
	bit b;
	uint8_t byte_pos = bit_pos >> 3;

	if(ASSERT(p))
	{
		return;
	}

	b = p[byte_pos] ^ (bit_pos & 0x7);
	b = v;
}

/*
 * date is in format: Oct 09 2017
 */
uint8_t get_year(const char *date)
{
	uint8_t ret;
	char *s = strrchr(date, ' ');

	ret = (uint8_t)atoi((char *)(s+3));
	return ret;
}

uint8_t get_month(const char *date)
{
	if (strncmp(date, "Jan", 3) == 0)
		return 1;
	else if(strncmp(date, "Feb", 3) == 0)
		return 2;
	else if(strncmp(date, "Mar", 3) == 0)
		return 3;
	else if(strncmp(date, "Apr", 3) == 0)
		return 4;
	else if(strncmp(date, "May", 3) == 0)
		return 5;
	else if(strncmp(date, "Jun", 3) == 0)
		return 6;
	else if(strncmp(date, "Jul", 3) == 0)
		return 7;
	else if(strncmp(date, "Aug", 3) == 0)
		return 8;
	else if(strncmp(date, "Sep", 3) == 0)
		return 9;
	else if(strncmp(date, "Oct", 3) == 0)
		return 10;
	else if(strncmp(date, "Nov", 3) == 0)
		return 11;
	else if(strncmp(date, "Dec", 3) == 0)
		return 12;
	else
		return 0;
}

uint8_t get_day(const char *date)
{
	uint8_t ret;
	char    _day[3] = {0};
	char *s = strchr(date, ' ');

	strncpy(_day, s+1, 2);
	ret = (uint8_t)atoi(_day);
	return ret;
}

/*
 * time: 13:13:30
 */
uint8_t get_hour(const char *time)
{
	uint8_t ret;
	char    _h[3] = {0};
	char *s = time; // strrchr(time, ' ');

	strncpy(_h, s, 2);
	ret = (uint8_t)atoi(_h);
	return ret;
}

uint8_t get_minute(const char *time)
{
	uint8_t ret;
	char    _m[3] = {0};
	char *s = strchr(time, ':');

	strncpy(_m, s+1, 2);
	ret = (uint8_t)atoi(_m);
	return ret;
}

uint8_t get_second(const char *time)
{
	uint8_t ret;
	char    _s[3] = {0};
	char *s = strrchr(time, ':');

	strncpy(_s, s+1, 2);
	ret = (uint8_t)atoi(_s);

	return ret;
}

void com_drv_on_poll(void)
{
	SFP_DRV_POLL();
}

