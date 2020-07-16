#include "include.h"

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
