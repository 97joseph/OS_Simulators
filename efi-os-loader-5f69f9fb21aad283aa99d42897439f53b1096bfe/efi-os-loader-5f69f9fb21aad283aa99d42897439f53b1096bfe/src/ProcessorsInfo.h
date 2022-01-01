// Copyright <notsoserious2017@gmail.com> 2021 Kam1runetzLabs

#ifndef COURSEWORK_PROCESSORSINFO_H
#define COURSEWORK_PROCESSORSINFO_H

#include <Pi/PiDxeCis.h>
#include <Protocol/MpService.h>
#include <Uefi.h>

typedef struct sProcessorsInfo {
    EFI_PROCESSOR_INFORMATION *processors;
    UINT64 enabledProcessorsCount;
    UINT64 totalProcessorsCount;
} PROCESSORS_INFO;

typedef enum eProcInfoErrors {
    PROCESSORS_INFO_SUCCESS_OPERATION,
    PROCESSORS_INFO_ERROR_INVALID_ARGUMENTS,
    PROCESSORS_INFO_ERROR_INTERNAL_ERROR
} PROCESSORS_INFO_ERROR_CODE;

PROCESSORS_INFO_ERROR_CODE ProcessorsInfoInit(EFI_BOOT_SERVICES *BootServices, EFI_MP_SERVICES_PROTOCOL *mpServicesProtocol, PROCESSORS_INFO *processorsInfo);

PROCESSORS_INFO_ERROR_CODE ProcessorsInfoGetCountOfEnabledProcessors(PROCESSORS_INFO processorsInfo, UINT64 *enabledProcessorsCount);

PROCESSORS_INFO_ERROR_CODE ProcessorsInfoGetTotalCountOfProcessors(PROCESSORS_INFO processorsInfo, UINT64 *totalCount);

PROCESSORS_INFO_ERROR_CODE ProcessorsInfoGetProcInfoByIndex(PROCESSORS_INFO processorsInfo, UINT64 index, EFI_PROCESSOR_INFORMATION *result);

#endif//COURSEWORK_PROCESSORSINFO_H
