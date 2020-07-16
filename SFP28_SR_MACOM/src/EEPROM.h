#ifndef EEPROM_H
#define EEPROM_H

#define  WRITEENABLE  0x01
#define  WRITEDISABLE 0x00


#define  A0_TABLE_SIZE           sizeof(struct Table_Size_256_struct)
#define  TABLE_LOWER_SIZE        sizeof(struct LOWER_MEMORY_MAP_struct)
#define  TABLE00_SIZE            sizeof(struct Table_Size_128_struct)
#define  TABLE80_SIZE            sizeof(struct Table_General_Control_struct)
#define  TABLE81_SIZE            sizeof(struct Table_TX_Cal_Ctrl_struct)
#define  TABLE82_SIZE            sizeof(struct Table_RX_Cal_Ctrl_struct)
#define  TABLE83_SIZE            sizeof(struct Table_Temperature_Compensation_struct)
#define  TABLE84_SIZE            sizeof(struct Table_ModSet_Calibration_struct)
#define  TABLE86_SIZE            sizeof(struct Table_Device_MALD_37645B_struct)
#define  TABLE87_SIZE            sizeof(struct Table_Device_MATA_37644B_struct)

#define  TABLE90_SIZE            sizeof(struct Table_Monitor_struct)
#define  TABLE91_SIZE            sizeof(struct Table_Device_Debug_struct)
#define  TABLE92_SIZE            sizeof(struct Table_System_Debug_struct)

#define  A0_TABLE_ADDRESS        (0x3800)  // 14k
#define  TABLE_LOWER_ADDRESS     (A0_TABLE_ADDRESS    + A0_TABLE_SIZE)
#define  TABLE00_ADDRESS         (TABLE_LOWER_ADDRESS + TABLE_LOWER_SIZE)
#define  TABLE80_ADDRESS         (TABLE00_ADDRESS     + TABLE00_SIZE)
#define  TABLE81_ADDRESS         (TABLE80_ADDRESS     + TABLE80_SIZE)
#define  TABLE82_ADDRESS         (TABLE81_ADDRESS     + TABLE81_SIZE)
#define  TABLE83_ADDRESS         (TABLE82_ADDRESS     + TABLE82_SIZE)
#define  TABLE84_ADDRESS         (TABLE83_ADDRESS     + TABLE83_SIZE)
#define  TABLE86_ADDRESS         (TABLE84_ADDRESS     + TABLE84_SIZE)
#define  TABLE87_ADDRESS         (TABLE86_ADDRESS     + TABLE86_SIZE)

// 低128字节 只有部分数据能被修改保存 0x00 - 0x5F
#define  TABLE_LOWER_CTRL_SIZE (sizeof( Threshold_Values_s)   +   \
                                sizeof( ExternalCal_Values_s) +   \
                                sizeof( uint8_t) )

#endif

