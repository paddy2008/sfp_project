//-----------------------------------------------------------------------------------
//      Copyright (c) 2005-2018
//-----------------------------------------------------------------------------------
//
// file name  :       EFM8LB1_FlashUtils.h
//
// description:       Contains several useful utilities for writing and updating
//
// Target     :       EFM8LB1
//
// version    :       V100.001
//					  2019/06/16
//
//-----------------------------------------------------------------------------------

#ifndef _EFM8LB1_FLASHUTILS_H_
#define _EFM8LB1_FLASHUTILS_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8LB1_Register_Enums.h>
//#include "EFM8LB1_FlashPrimitives.h"

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------
// Flash read/write/erase routines
extern void FLASH_Write (FLADDR dest, uint8_t *src, uint16_t numbytes);
extern uint8_t * FLASH_Read (uint8_t *dest, FLADDR src, uint16_t numbytes);
extern void FLASH_Clear (FLADDR addr, uint16_t numbytes);

// Flash update/copy routines
extern void FLASH_Update (FLADDR dest, uint8_t *src, uint16_t numbytes);
extern void FLASH_Copy (FLADDR dest, FLADDR src, uint16_t numbytes);

// Flash test routines
extern void FLASH_Fill (FLADDR addr, uint32_t length, uint8_t fill);


#endif    // _EFM8LB1_FLASHUTILS_H_
