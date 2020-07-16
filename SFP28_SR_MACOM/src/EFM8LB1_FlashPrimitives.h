//-----------------------------------------------------------------------------------
//      Copyright (c) 2005-2018,
//-----------------------------------------------------------------------------------
//
// file name  :       EFM8LB1_FlashPrimitives.h
//
// description:       Contains several useful utilities for writing and updating
//
// Target     :       EFM8LB1
//
// version    :       V100.001
//					  2019/06/16
//
//-----------------------------------------------------------------------------------


#ifndef _EFM8LB1_FLASHPRIMITIVES_H_
#define _EFM8LB1_FLASHPRIMITIVES_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8LB1_Register_Enums.h>

//-----------------------------------------------------------------------------
// Structures, Unions, Enumerations, and Type Definitions
//-----------------------------------------------------------------------------
typedef uint16_t FLADDR;

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------
#ifndef FLASH_PAGESIZE
#define FLASH_PAGESIZE 512
#endif

#ifndef FLASH_TEMP
#define FLASH_TEMP 0xFA00L             // For 64 kB Flash devices
//#define FLASH_TEMP 0x7C00L           // For 32 kB Flash devices
//#define FLASH_TEMP 0x3C00L           // For 16 kB Flash devices
#endif

#ifndef FLASH_LAST
#define FLASH_LAST 0xFC00L             // For 64 kB Flash devices
//#define FLASH_TEMP 0x7E00L           // For 32 kB Flash devices
//#define FLASH_TEMP 0x3E00L           // For 16 kB Flash devices
#endif

//-----------------------------------------------------------------------------
// Exported Function Prototypes
//-----------------------------------------------------------------------------
extern void          FLASH_ByteWrite (FLADDR addr, uint8_t byte);
extern uint8_t       FLASH_ByteRead  (FLADDR addr);
extern void          FLASH_PageErase (FLADDR addr);


#endif    // _EFM8LB1_FLASHPRIMITIVES_H_
