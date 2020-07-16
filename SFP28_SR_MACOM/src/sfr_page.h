
#ifndef __SFR_PAGE_H__
#define __SFR_PAGE_H__
 
#include "SI_EFM8LB1_Register_Enums.h"

// ÌØÊâ¼Ä´æÆ÷Ò³¶¨Òå
#define PG_I2CSLAVE PG3_PAGE

#define PG_DAC      PG4_PAGE

/*
#define LEGACY_PAGE 0x00 ///< Legacy SFR Page
#define PG2_PAGE    0x10 ///< Page2
#define PG3_PAGE    0x20 ///< Page3
#define PG4_PAGE    0x30 ///< Page4
*/

// declare variable needed for autopage enter/exit
#define DECL_PAGE uint8_t savedPage

// enter autopage section
#define SET_PAGE(p)     do                                                    \
                        {                                                     \
							savedPage = SFRPAGE;  /* save current SFR page */   \
                          	SFRPAGE = (p);        /* set SFR page */            \
                        } while(0)
// exit autopage section
#define RESTORE_PAGE    do                                                    \
                        {                                                     \
							SFRPAGE = savedPage;  /* restore saved SFR page */  \
                        } while(0)

#endif
