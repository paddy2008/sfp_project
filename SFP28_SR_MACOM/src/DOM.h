#ifndef _DOM_H
#define _DOM_H

extern void GetDOM_Temperature(void);
extern void GetDOM_Voltage(void);

extern void GetDOM_LDBias(void);
extern void GetDOM_TxPower(void);
extern void GetDOM_RxPower(void);

extern void UpdateAlarmAndWarningBits(void);
extern void UpdateDOMData(void);

#endif
