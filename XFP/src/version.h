#ifndef _VERSION_H
#define _VERSION_H


//#define  DEBUG_LOG_FUNC

#define  PRO_VERSION00         (0xF0)
#define  PRO_VERSION01         (0x00)
#define  PRO_VERSION02         (0x0E)
#define  SW_VERSION00		   (0x42)
#define  SW_VERSION01		   (0x10)
#define  SW_VERSION02		   (0x40)
#define  SW_VERSION03		   (0x05)   //after release add 1

uint8_t  get_year(const char *date);
uint8_t  get_month(const char *date);
uint8_t  get_day(const char *date);
uint8_t  get_hour(const char *time);
uint8_t  get_minute(const char *time);
uint8_t  get_second(const char *time);

#endif
