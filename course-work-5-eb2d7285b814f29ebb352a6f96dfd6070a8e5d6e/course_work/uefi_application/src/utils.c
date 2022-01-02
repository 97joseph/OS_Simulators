#include "utils.h"

UINTN GetMsrValue(UINTN MSR)
{
    UINT32 low_bits = 0, high_bits = 0;

    __asm__ volatile (
            "rdmsr"
            : "=a"(low_bits), "=d"(high_bits)
            : "c"(MSR)
    );

    return (((UINTN) high_bits << 32) | low_bits) & 0xFFFFFFFFFFFFF000;
}

void SendSIPI(UINTN ApicID, UINTN InitCodeEntry)
{
    UINTN addr = GetMsrValue(0x0000001B);

    volatile UINT32* const APIC_ICR_LOW = (void *) (addr + 0x300);
    volatile UINT32* const APIC_ICR_HIG = (void *) (addr + 0x310);

    // INIT
    *APIC_ICR_HIG = (UINT32)ApicID << 24;
    *APIC_ICR_LOW = 0x00000500;

    // Wait
    for (volatile unsigned i = 0; i < 0xFFFFFF; ++i);

    // SIPI
    *APIC_ICR_LOW = ((UINT32) 0x00000600 | (UINT32)(InitCodeEntry >> 12));

    // Wait
    for (volatile unsigned i = 0; i < 0xFFFFFF; ++i) ;

    // SIPI
    *APIC_ICR_LOW = ((UINT32) 0x00000600 | (UINT32)(InitCodeEntry >> 12));
}

void GetMemoryMap(EFI_BOOT_SERVICES* gBS, MemoryMap* memoryMap)
{
    UINTN bufferSize = 0;
    UINTN memoryMapSize = bufferSize;
    EFI_MEMORY_DESCRIPTOR* efiMemoryDescriptors = NULL;
    UINTN mapKey;
    UINTN descriptorSize = 0;
    UINT32 descriptorVersion = 0;
    BOOLEAN success = FALSE;

    while (!success)
    {
        if (efiMemoryDescriptors != NULL) {
            gBS->FreePool((VOID*)efiMemoryDescriptors);
            efiMemoryDescriptors = NULL;
        }

        bufferSize = memoryMapSize + descriptorSize * 2;
        memoryMapSize = bufferSize;
        EFI_STATUS status = gBS->AllocatePool(0x80000000, bufferSize, (VOID**)&efiMemoryDescriptors);
        if (status != EFI_SUCCESS)
            continue;

        status = gBS->GetMemoryMap(
            &memoryMapSize,
            efiMemoryDescriptors,
            &mapKey,
            &descriptorSize,
            &descriptorVersion
        );

        success = (status == EFI_SUCCESS);
    }

    memoryMap->descriptorSize = descriptorSize;
    memoryMap->memoryMapSize = memoryMapSize;
    memoryMap->efiMemoryDescriptors = efiMemoryDescriptors;
    memoryMap->mapKey = mapKey;
}

void GetProcessorsInfo(EFI_BOOT_SERVICES* gBS, EFI_MP_SERVICES_PROTOCOL* gMS, ProcessorsInfo* processorsInfo)
{
    UINTN processorsEnabled = 0;
    UINTN processorsTotal = 0;
    gMS->GetNumberOfProcessors(gMS, &processorsTotal, &processorsEnabled);

    EFI_PROCESSOR_INFORMATION* efiProcessorsInfo = NULL;
    gBS->AllocatePool(0x80000000, sizeof(EFI_PROCESSOR_INFORMATION) * processorsTotal, (VOID**)&efiProcessorsInfo);
    for (UINTN i = 0; i < processorsTotal; ++i)
    {
        gMS->GetProcessorInfo(gMS, i, &efiProcessorsInfo[i]);
    }

    processorsInfo->processorsTotal = processorsTotal;
    processorsInfo->processorsEnabled = processorsEnabled;
    processorsInfo->efiProcessorsInfo = efiProcessorsInfo;
}
