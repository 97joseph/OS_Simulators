// Copyright <notsoserious2017@gmail.com> 2021 Kam1unetzLabs

#include "MemoryMap.h"


MEMORY_MAP_ERROR_CODE MemoryMapInit(EFI_BOOT_SERVICES *BootServices, MEMORY_MAP *memMap) {
    if (!BootServices || !memMap)
        return MEMORY_MAP_ERROR_INVALID_ARGUMENTS;

    UINT64 key = 0;
    UINT64 descriptorSize = 0;
    UINT32 descriptorVersion = 0;
    UINT64 memoryMapSize = 0;
    UINT64 bufferSize = memoryMapSize;
    EFI_MEMORY_DESCRIPTOR *descriptors = NULL;
    EFI_STATUS status;
    BOOLEAN success = FALSE;

    while (!success) {
        if (descriptors != NULL) {
            BootServices->FreePool((VOID *) descriptors);
            descriptors = NULL;
        }

        bufferSize = memoryMapSize + descriptorSize * 2;
        memoryMapSize = bufferSize;
        status = BootServices->AllocatePool(EfiLoaderData, bufferSize, (VOID **) &descriptors);
        if (status != EFI_SUCCESS)
            continue;

        status = BootServices->GetMemoryMap(
                &memoryMapSize,
                descriptors,
                &key,
                &descriptorSize,
                &descriptorVersion);

        success = (status == EFI_SUCCESS);
    }

    memMap->descriptorSize = descriptorSize;
    memMap->key = key;
    memMap->descriptors = descriptors;
    memMap->descriptorCount = memoryMapSize / descriptorSize;

    return MEMORY_MAP_SUCCESS_OPERATION;
}

MEMORY_MAP_ERROR_CODE MemoryMapGetKey(MEMORY_MAP memoryMap, UINT64 *key) {
    if (!key)
        return MEMORY_MAP_ERROR_INVALID_ARGUMENTS;
    *key = memoryMap.key;
    return MEMORY_MAP_SUCCESS_OPERATION;
}

MEMORY_MAP_ERROR_CODE MemoryMapGetMemoryDescriptor(MEMORY_MAP memoryMap, UINT64 descriptorIndex, EFI_MEMORY_DESCRIPTOR *descriptor) {
    if (!descriptor)
        return MEMORY_MAP_ERROR_INVALID_ARGUMENTS;
    if (descriptorIndex >= memoryMap.descriptorCount)
        return MEMORY_MAP_ERROR_INVALID_ARGUMENTS;
    *descriptor = *(memoryMap.descriptors + descriptorIndex);
    return MEMORY_MAP_SUCCESS_OPERATION;
}

MEMORY_MAP_ERROR_CODE MemoryMapGetDescriptorsCount(MEMORY_MAP memoryMap, UINT64 *descriptorCount) {
    if (!descriptorCount)
        return MEMORY_MAP_ERROR_INVALID_ARGUMENTS;
    *descriptorCount = memoryMap.descriptorCount;
    return MEMORY_MAP_SUCCESS_OPERATION;
}

MEMORY_MAP_ERROR_CODE MemoryMapGetDescriptorSize(MEMORY_MAP memoryMap, UINT64 *descriptorSize) {
    if (!descriptorSize)
        return MEMORY_MAP_ERROR_INVALID_ARGUMENTS;
    *descriptorSize = memoryMap.descriptorSize;
    return MEMORY_MAP_SUCCESS_OPERATION;
}
