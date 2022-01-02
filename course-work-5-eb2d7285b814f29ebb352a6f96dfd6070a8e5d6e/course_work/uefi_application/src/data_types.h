#ifndef COURSEWORK_DATA_TYPES_H
#define COURSEWORK_DATA_TYPES_H

#include "Uefi.h"
#include "Pi/PiMultiPhase.h"
#include "Protocol/MpService.h"

typedef struct {
    UINTN mapKey;
    EFI_MEMORY_DESCRIPTOR* efiMemoryDescriptors;
    UINTN descriptorSize;
    UINTN memoryMapSize;
} MemoryMap;

typedef struct {
    EFI_PROCESSOR_INFORMATION* efiProcessorsInfo;
    UINTN processorsEnabled;
    UINTN processorsTotal;
} ProcessorsInfo;

#endif // COURSEWORK_DATA_TYPES_H
