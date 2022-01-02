#include "print_functions.h"

void PrintHex(EFI_SYSTEM_TABLE* gST, UINT64 num)
{
    static CHAR16* str = u"0000000000000000";
    UINTN used = 0;
    for (UINTN i = 0; i < 16; ++i)
    {
        UINTN digit = num & 0xF;
        num >>= 4;

        str[15 - i] = ((digit >= 10) ? (u'A' + digit - 10) : (u'0' + digit));
        ++used;

        if (num == 0) break;
    }

    for (UINTN i = 0; i < 16 - used; ++i)
        str[i] = u'0';

    gST->ConOut->OutputString(gST->ConOut, u"0x");
    gST->ConOut->OutputString(gST->ConOut, str);
}

void PrintDec(EFI_SYSTEM_TABLE* gST, UINT64 num)
{
    static CHAR16* str = u"00000000000000000000"; // 20
    UINTN used = 0;
    for (UINTN i = 0; i < 20; ++i)
    {
        UINTN digit = num % 10;
        num /= 10;

        str[19 - i] = (u'0' + digit);
        ++used;

        if (num == 0) break;
    }

    for (UINTN i = 0; i < 20 - used; ++i)
        str[i] = u'0';

    gST->ConOut->OutputString(gST->ConOut, &str[20 - used]);
}

void PrintMemoryType(EFI_SYSTEM_TABLE* gST, EFI_MEMORY_TYPE type)
{
    switch (type)
    {
        case EfiReservedMemoryType:
            gST->ConOut->OutputString(gST->ConOut, u"Reserved   ");
            break;
        case EfiLoaderCode:
            gST->ConOut->OutputString(gST->ConOut, u"Loader code");
            break;
        case EfiLoaderData:
            gST->ConOut->OutputString(gST->ConOut, u"Loader data");
            break;
        case EfiBootServicesCode:
            gST->ConOut->OutputString(gST->ConOut, u"BS_Code    ");
            break;
        case EfiBootServicesData:
            gST->ConOut->OutputString(gST->ConOut, u"BS_Data    ");
            break;
        case EfiRuntimeServicesCode:
            gST->ConOut->OutputString(gST->ConOut, u"RT_Code    ");
            break;
        case EfiRuntimeServicesData:
            gST->ConOut->OutputString(gST->ConOut, u"RT_Data    ");
            break;
        case EfiUnusableMemory:
            gST->ConOut->OutputString(gST->ConOut, u"Unusable   ");
            break;
        case EfiACPIReclaimMemory:
            gST->ConOut->OutputString(gST->ConOut, u"ACPI_Recl  ");
            break;
        case EfiACPIMemoryNVS:
            gST->ConOut->OutputString(gST->ConOut, u"ACPI_NVS   ");
            break;
        case EfiMemoryMappedIO:
            gST->ConOut->OutputString(gST->ConOut, u"MMIO       ");
            break;
        case EfiMemoryMappedIOPortSpace:
            gST->ConOut->OutputString(gST->ConOut, u"MMIO_Port  ");
            break;
        case EfiPalCode:
            gST->ConOut->OutputString(gST->ConOut, u"PalCode    ");
            break;
        case EfiPersistentMemory:
            gST->ConOut->OutputString(gST->ConOut, u"Persistent ");
            break;
        default:
            gST->ConOut->OutputString(gST->ConOut, u"Available  ");
            break;
    }

}

void PrintMemoryDescriptor(EFI_SYSTEM_TABLE* gST, EFI_MEMORY_DESCRIPTOR* descriptor)
{
    gST->ConOut->OutputString(gST->ConOut, u"| ");
    PrintMemoryType(gST, descriptor->Type);
    gST->ConOut->OutputString(gST->ConOut, u" | ");
    PrintHex(gST, descriptor->PhysicalStart);
    gST->ConOut->OutputString(gST->ConOut, u" | ");
    PrintHex(gST, descriptor->NumberOfPages);
    gST->ConOut->OutputString(gST->ConOut, u" | ");
    PrintHex(gST, descriptor->Attribute);
    gST->ConOut->OutputString(gST->ConOut, u" |\r\n");
}

void PrintProcessorInfo(EFI_SYSTEM_TABLE* gST, EFI_PROCESSOR_INFORMATION* processorInfo)
{
    gST->ConOut->OutputString(gST->ConOut, u"ApicID: ");
    PrintDec(gST, processorInfo->ProcessorId);
    gST->ConOut->OutputString(gST->ConOut, u" | Status Flag: ");
    PrintHex(gST, processorInfo->StatusFlag);

    gST->ConOut->OutputString(gST->ConOut, u" | Package: ");
    PrintDec(gST, processorInfo->Location.Package);
    gST->ConOut->OutputString(gST->ConOut, u" | Core: ");
    PrintDec(gST, processorInfo->Location.Core);
    gST->ConOut->OutputString(gST->ConOut, u" | Thread: ");
    PrintDec(gST, processorInfo->Location.Thread);
    gST->ConOut->OutputString(gST->ConOut, u"\r\n");
}

void PrintMemoryMap(EFI_SYSTEM_TABLE* gST, EFI_BOOT_SERVICES* gBS, const MemoryMap* memoryMap, UINTN time)
{
    gST->ConOut->OutputString(gST->ConOut, u"        Memory Map:\r\n"
                                            "|-------------|--------------------|--------------------|--------------------|\r\n"
                                            "|    Type     |       Start        |     Num pages      |     Attributes     |\r\n"
                                            "|-------------|--------------------|--------------------|--------------------|\r\n");
    for (UINTN i = 0; i < memoryMap->memoryMapSize / memoryMap->descriptorSize; ++i)
    {
        EFI_MEMORY_DESCRIPTOR* current = (EFI_MEMORY_DESCRIPTOR*)((char*)(memoryMap->efiMemoryDescriptors) + i * memoryMap->descriptorSize);
        PrintMemoryDescriptor(gST, current);
        gBS->Stall(time);
    }
    gST->ConOut->OutputString(gST->ConOut, u"|-------------|--------------------|--------------------|--------------------|\r\n\r\n");
}

void PrintProcessorsInfo(EFI_SYSTEM_TABLE* gST, const ProcessorsInfo* processorsInfo)
{
    gST->ConOut->OutputString(gST->ConOut, u"Processors enabled: ");
    PrintDec(gST, processorsInfo->processorsEnabled);
    gST->ConOut->OutputString(gST->ConOut, u" / ");
    PrintDec(gST, processorsInfo->processorsTotal);
    gST->ConOut->OutputString(gST->ConOut, u" total:\r\n");
    for (UINTN i = 0; i < processorsInfo->processorsTotal; ++i)
    {
        PrintProcessorInfo(gST, &processorsInfo->efiProcessorsInfo[i]);
    }
    gST->ConOut->OutputString(gST->ConOut, u"\r\n");
}