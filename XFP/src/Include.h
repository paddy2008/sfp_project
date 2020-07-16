#ifndef _INCLUDE_H
#define _INCLUDE_H

#include <SI_EFM8LB1_Register_Enums.h>  // SFR declarations
#include <string.h>
#include <stdlib.h>
#include <LIMITS.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>


#include "wdt_0.h"
#include "I2C_SlaveBuffer.h"
#include "I2C_Slave.h"
#include "timer.h"
#include "sfr_page.h"
#include "system.h"

#include "version.h"
#include "InitDevice.h"
#include "i2c_master.h"
#include "sfp_data_struct_public_def.h"
#include "Convert.h"

#include "I2cSlaveCode.h"
#include "I2cSlavePrefetch.h"
#include "MemoryAccess.h"

#include "EFM8LB1_FlashPrimitives.h"
#include "EFM8LB1_FlashUtils.h"
#include "EEPROM.h"
#include "log.h"

#endif

