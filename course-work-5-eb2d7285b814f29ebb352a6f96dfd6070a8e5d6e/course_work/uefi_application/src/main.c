#include "Uefi.h"
#include "Pi/PiMultiPhase.h"
#include "Protocol/MpService.h"

#include "print_functions.h"
#include "utils.h"

EFI_STATUS EFIAPI UefiMain (
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
{
    EFI_SYSTEM_TABLE* gST = SystemTable;
    EFI_BOOT_SERVICES* gBS = gST->BootServices;
    EFI_MP_SERVICES_PROTOCOL* gMS = NULL;

    EFI_GUID MpServicesGuid = EFI_MP_SERVICES_PROTOCOL_GUID;
    gBS->LocateProtocol(&MpServicesGuid, NULL, (VOID**)&gMS);

    MemoryMap memoryMap;
    ProcessorsInfo processorsInfo;

    GetProcessorsInfo(gBS, gMS, &processorsInfo);
    GetMemoryMap(gBS, &memoryMap);

    //PrintMemoryMap(gST, gBS, &memoryMap, 100 * 1000);
    //PrintProcessorsInfo(gST, &processorsInfo);

#include "trampoline.inl"
    const UINTN InitCodeEntry = 0x10000;
    gBS->CopyMem((VOID*)InitCodeEntry, (VOID*)&___course_work_trampoline_src_trampoline_bin[0], ___course_work_trampoline_src_trampoline_bin_len);

#include "c_code.inl"
/*    unsigned char ___c_code_bin[] = {
            0xeb, 0xfe
    };
    unsigned int ___c_code_bin_len = 2;
*/

    const UINTN CCodeEntry = 0x1500000;
    gBS->CopyMem((VOID*)CCodeEntry, (VOID*)&___c_code_bin[0], ___c_code_bin_len);

    gBS->ExitBootServices(ImageHandle, memoryMap.mapKey);

    UINTN ApicID = processorsInfo.efiProcessorsInfo[1].ProcessorId;
    SendSIPI(ApicID, InitCodeEntry);

    while (TRUE);
    //return EFI_SUCCESS;
}