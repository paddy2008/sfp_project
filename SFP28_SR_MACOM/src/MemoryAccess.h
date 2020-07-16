
#ifndef MEMORY_ACCESS_H
#define MEMORY_ACCESS_H

void SubProcess_Table_A0 ( uint8_t regAddress, uint8_t regData );
void SubProcess_TableLower ( uint8_t regAddress, uint8_t regData );
void SubProcess_Table00 (uint8_t regAddress, uint8_t regData);
void SubProcess_Table80 (uint8_t regAddress, uint8_t regData);
void SubProcess_Table81 (uint8_t regAddress, uint8_t regData);
void SubProcess_Table82 (uint8_t regAddress, uint8_t regData);
void SubProcess_Table83 (uint8_t regAddress, uint8_t regData);
void SubProcess_Table84 (uint8_t regAddress, uint8_t regData);
void SubProcess_Table86 (uint8_t regAddress, uint8_t regData);
void SubProcess_Table87 (uint8_t regAddress, uint8_t regData );

void SubProcess_Table91 (uint8_t regAddress, uint8_t regData);
void SubProcess_Table92 (uint8_t regAddress, uint8_t regData);


extern void TransferDataToRam (void);
extern void ProcessI2cBuffersData (void);


#endif

