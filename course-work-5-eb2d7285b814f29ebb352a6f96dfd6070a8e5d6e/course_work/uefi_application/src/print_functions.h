#ifndef COURSEWORK_PRINT_FUNCTIONS_HPP
#define COURSEWORK_PRINT_FUNCTIONS_HPP

#include "data_types.h"

void PrintHex(EFI_SYSTEM_TABLE* gST, UINT64 num);

void PrintDec(EFI_SYSTEM_TABLE* gST, UINT64 num);

void PrintMemoryMap(EFI_SYSTEM_TABLE* gST, EFI_BOOT_SERVICES* gBS, const MemoryMap* map, UINTN time);

void PrintProcessorsInfo(EFI_SYSTEM_TABLE* gST, const ProcessorsInfo* processorsInfo);

#endif //COURSEWORK_PRINT_FUNCTIONS_HPP
