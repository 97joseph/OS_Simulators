#ifndef COURSEWORK_UTILS_H
#define COURSEWORK_UTILS_H

#include "data_types.h"

UINTN GetMsrValue(UINTN MSR);

void SendSIPI(UINTN ApicID, UINTN InitCodeEntry);

void GetMemoryMap(EFI_BOOT_SERVICES* gBS, MemoryMap* memoryMap);

void GetProcessorsInfo(EFI_BOOT_SERVICES* gBS, EFI_MP_SERVICES_PROTOCOL* gMS, ProcessorsInfo* processorsInfo);

#endif //COURSEWORK_UTILS_H
