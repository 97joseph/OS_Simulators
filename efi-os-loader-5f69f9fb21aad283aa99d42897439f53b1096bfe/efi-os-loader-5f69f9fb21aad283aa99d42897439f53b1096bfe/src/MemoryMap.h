// Copyright <notsoserious2017@gmail.com> 2021 Kam1runetzLabs

#ifndef COURSEWORK_MEMORYMAP_H
#define COURSEWORK_MEMORYMAP_H

#include <Uefi.h>
#include <Uefi/UefiBaseType.h>

typedef struct sMemoryMap {
    UINT64 key;
    UINT64 descriptorSize;
    UINT64 descriptorCount;
    EFI_MEMORY_DESCRIPTOR *descriptors;
} MEMORY_MAP;

typedef enum eMemMapErrors {
    MEMORY_MAP_SUCCESS_OPERATION,
    MEMORY_MAP_ERROR_INVALID_ARGUMENTS,
} MEMORY_MAP_ERROR_CODE;

MEMORY_MAP_ERROR_CODE MemoryMapInit(EFI_BOOT_SERVICES *BootServices, MEMORY_MAP *memMap);

MEMORY_MAP_ERROR_CODE MemoryMapGetKey(MEMORY_MAP memoryMap, UINT64 *key);

MEMORY_MAP_ERROR_CODE MemoryMapGetMemoryDescriptor(MEMORY_MAP memoryMap, UINT64 descriptorIndex, EFI_MEMORY_DESCRIPTOR *descriptor);

MEMORY_MAP_ERROR_CODE MemoryMapGetDescriptorsCount(MEMORY_MAP memoryMap, UINT64 *descriptorCount);

MEMORY_MAP_ERROR_CODE MemoryMapGetDescriptorSize(MEMORY_MAP memoryMap, UINT64 *descriptorSize);

#endif//COURSEWORK_MEMORYMAP_H
