
#ifndef MEMORY_ACCESS_H
#define MEMORY_ACCESS_H


#define  A0_TABLE_SIZE           sizeof(struct Table_Size_256_struct)
#define  TABLE_LOWER_SIZE        sizeof(struct LOWER_MEMORY_128_struct)
#define  TABLE00_SIZE            sizeof(struct Table_Size_128_struct)
#define  TABLE02_SIZE            sizeof(struct Table2_Size_128_struct)
#define  TABLE80_SIZE			 sizeof(struct Page_80H_Module_Information_struct)

#define PAGE_SELECT			 		(127)
#define A2_DOM_START_ADDR 	 		(96)  		//96 ~ 105
#define A2_DOM_END_ADDR 	 		(105)

#define A2_MONITOR_DATA_START_ADDR   A2_DOM_END_ADDR + 1
#define A2_MONITOR_DATA_END_ADDR	 (119)

#define A2_DATA_READY_ADDR	(110)


#define  COM_LOCK_SEM(sem)     	((sem)--)
#define  COM_UNLOCK_SEM(sem)   	((sem)++)
#define  COM_GET_LOCK(sem)     	(sem)


extern void DataSynchronization(void);
extern void TransferA0_DataToRam (void);
extern void ProcessI2cBuffersData (void);
extern void RealTimeDataPoll(void);
extern void InitPrivateData(void);
#endif

