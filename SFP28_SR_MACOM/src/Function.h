#ifndef _FUNCTION_H
#define _FUNCTION_H


extern void InitPrivateData(void);
extern void InitDeviceData(void);

extern void UpdateModuleStatus(void);

extern void TOSA_LDChip_ON_OFF(void);
extern void SetDeviceCtrlData(void);
extern void TxPowerAdjust(void);
extern void ModCurrentAdjust(void);

#endif
