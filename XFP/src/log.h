#ifndef __LOG_H__
#define __LOG_H__

#include <SI_EFM8LB1_Register_Enums.h>

#ifdef DEBUG_LOG_FUNC

#define  LOG_RAM_SIZE    sizeof(struct Table_Size_128_struct)

#define  ASSERT(exp) 	if ((exp) == NULL) return

void log_clear();
void log_init(char *log);
void log_save(char *fmt,...);

#endif

#endif
