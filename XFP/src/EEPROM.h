#ifndef EEPROM_H
#define EEPROM_H

#define  WRITEENABLE  0x01
#define  WRITEDISABLE 0x00


#define  A0_TABLE_SIZE           sizeof(struct Table_Size_256_struct)
#define  TABLE_LOWER_SIZE        sizeof(struct LOWER_MEMORY_128_struct)
#define  TABLE00_SIZE            sizeof(struct Table_Size_128_struct)
#define  DOM_SIZE				 sizeof(struct LOWER_MEMORY_DOM_struct)

#define  A0_TABLE_ADDRESS        (0x7000)  // 14k
#define  TABLE_LOWER_ADDRESS     (A0_TABLE_ADDRESS    + A0_TABLE_SIZE)
#define  TABLE00_ADDRESS         (TABLE_LOWER_ADDRESS + TABLE_LOWER_SIZE)

#endif

