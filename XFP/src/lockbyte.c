//-----------------------------------------------------------------------------
//      Copyright (c) 2018-2019,
//-----------------------------------------------------------------------------
//
// file name  :       lockbyte.c
//
// description:       Contains Normal function
//
// Target     :       C8051F34x / C8051F38x
//
// version    :       V100.001
//					  2019.07.30
//
//-----------------------------------------------------------------------------

#include "include.h"

// SEGMENTS (?CO?LOCKBYTE(C:0xFBFF)) OVERLAY (* ! ?CO?LOCKBYTE)
const uint8_t code lockbyte = 0xFF;
