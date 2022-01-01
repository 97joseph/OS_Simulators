// Copyright <notsoserious2017@gmail.com> 2021 Kam1runetzLabs

#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Pi/PiDxeCis.h>
#include <Protocol/MpService.h>
#include <Uefi.h>

#include "MemoryMap.h"
#include "ProcessorsInfo.h"

#include "jumper.h"
#include "payload.h"

#define INIT_ENTRY_ADDRESS 0x10000
#define PAYLOAD_ENTRY_ADDRESS 0x1500000


static const CHAR16 *EfiMemoryTypes[] = {
        u"Reserved",
        u"Loader Code",
        u"Loader Data",
        u"BootServices Code",
        u"BootServices Data",
        u"RuntimeServices Code",
        u"RuntimeServices Data",
        u"Unusable",
        u"ACPI Reclaim",
        u"ACPI NVS",
        u"Mapped IO",
        u"Mapped IO Port Space",
        u"Pal Code",
        u"Persistent",
};

static void InitProcessor(UINT64 procID, void *initCodeAddr) {
    UINT32 lowBits = 0;
    UINT32 highBits = 0;

    __asm__ volatile(
            "rdmsr"
            : "=a"(lowBits), "=d"(highBits)
            : "c"(0x0000001B));
    UINT64 address = (((UINTN) highBits << 32) | lowBits) & 0xFFFFFFFFFFFFF000;

    volatile UINT32 *const APIC_ICR_LOW = (void *) (address + 0x300);
    volatile UINT32 *const APIC_ICR_HIG = (void *) (address + 0x310);

    *APIC_ICR_HIG = (UINT32) procID << 24;
    *APIC_ICR_LOW = 0x00000500;

    for (volatile UINT64 i = 0; i != 0xfffff; ++i)
        ;

    *APIC_ICR_LOW = ((UINT32) 0x00000600 | (UINT32) ((UINT64) initCodeAddr >> 12));
}

EFI_STATUS EFIAPI UefiMain(
        IN EFI_HANDLE ImageHandle,
        IN EFI_SYSTEM_TABLE *SystemTable) {
    EFI_SYSTEM_TABLE *gST = SystemTable;
    EFI_BOOT_SERVICES *gBS = gST->BootServices;

    MEMORY_MAP memoryMap;

    if (MEMORY_MAP_SUCCESS_OPERATION != MemoryMapInit(gBS, &memoryMap)) {
        Print(L"[ERROR] Unable to get memory map\r\nExiting....\r\n");
        return EFI_SUCCESS;
    }

    EFI_MEMORY_DESCRIPTOR descriptor;
    UINT64 descriptorsCount;
    UINT64 descriptorSize;
    UINT64 key;

    MemoryMapGetDescriptorSize(memoryMap, &descriptorSize);
    MemoryMapGetDescriptorsCount(memoryMap, &descriptorsCount);
    MemoryMapGetKey(memoryMap, &key);

    Print(L"DescSize: %X | DescCount: %X | MemMapSize: %X | Key: %X\r\n",
          descriptorSize, descriptorsCount, descriptorsCount * descriptorSize, key);

    for (UINT64 i = 0; i != descriptorsCount; ++i) {
        MemoryMapGetMemoryDescriptor(memoryMap, i, &descriptor);
        Print(L"Type: %s | Physical Start: %X | Pages Number: %X | Attributes: %X\r\n",
              (descriptor.Type > EfiPersistentMemory ? L"Unknown\0" : EfiMemoryTypes[descriptor.Type]),
              descriptor.PhysicalStart,
              descriptor.NumberOfPages,
              descriptor.Attribute);
    }

    PROCESSORS_INFO processorsInfo;
    EFI_MP_SERVICES_PROTOCOL *mpServicesProtocol;
    EFI_GUID mpServicesProtocolGuid = EFI_MP_SERVICES_PROTOCOL_GUID;

    EFI_STATUS status = gBS->LocateProtocol(&mpServicesProtocolGuid, NULL, (void **) &mpServicesProtocol);
    if (EFI_ERROR(status)) {
        Print(L"Unable to allocate MP_SERVICES_PROTOCOL\r\nExiting....\r\n");
        return EFI_SUCCESS;
    }

    if (PROCESSORS_INFO_SUCCESS_OPERATION != ProcessorsInfoInit(gBS, mpServicesProtocol, &processorsInfo)) {
        Print(L"Unable to get processors information\r\nExiting....\r\n");
        return EFI_SUCCESS;
    }

    UINT64 totalProcessorsCount;
    UINT64 enabledProcessorsCount;
    EFI_PROCESSOR_INFORMATION efiProcessorInformation;

    ProcessorsInfoGetTotalCountOfProcessors(processorsInfo, &totalProcessorsCount);
    ProcessorsInfoGetCountOfEnabledProcessors(processorsInfo, &enabledProcessorsCount);

    Print(L"\n\nTotal processors count: %d | Enabled processors count: %d\r\n", totalProcessorsCount, enabledProcessorsCount);

    for (UINT64 i = 0; i != totalProcessorsCount; ++i) {
        ProcessorsInfoGetProcInfoByIndex(processorsInfo, i, &efiProcessorInformation);
        Print(L"ID: %d | Status: %d | Pkg: %d | Core: %d | Thread: %d\r\n",
              efiProcessorInformation.ProcessorId,
              efiProcessorInformation.StatusFlag,
              efiProcessorInformation.Location.Package,
              efiProcessorInformation.Location.Core,
              efiProcessorInformation.Location.Thread);
    }

    void *initCodeAddr = (void *) INIT_ENTRY_ADDRESS;
    gBS->CopyMem(initCodeAddr, (void *) jumper_bin, jumper_bin_len);

    void *payloadEntryAddr = (void *) PAYLOAD_ENTRY_ADDRESS;
    gBS->CopyMem(payloadEntryAddr, (void *) payload_bin, payload_bin_len);

    gBS->ExitBootServices(ImageHandle, key);

    for (UINT64 i = 1; i != totalProcessorsCount; ++i) {
        ProcessorsInfoGetProcInfoByIndex(processorsInfo, i, &efiProcessorInformation);
        InitProcessor(efiProcessorInformation.ProcessorId, initCodeAddr);
    }
    for (;;)
        ;
    return EFI_SUCCESS;
}
