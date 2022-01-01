// Copyright <notsoserious2017@gmail.com> 2021 Kam1runetzLabs

#include "ProcessorsInfo.h"

PROCESSORS_INFO_ERROR_CODE ProcessorsInfoInit(EFI_BOOT_SERVICES *BootServices, EFI_MP_SERVICES_PROTOCOL *mpServicesProtocol, PROCESSORS_INFO *processorsInfo) {
    if (!BootServices || !processorsInfo || !mpServicesProtocol)
        return PROCESSORS_INFO_ERROR_INVALID_ARGUMENTS;
    UINT64 enabledProcessorsCount = 0;
    UINT64 totalProcessorsCount = 0;

    EFI_PROCESSOR_INFORMATION *efiProcessorsInformation;
    EFI_STATUS status;

    status = mpServicesProtocol->GetNumberOfProcessors(mpServicesProtocol, &totalProcessorsCount, &enabledProcessorsCount);
    if (EFI_ERROR(status))
        return PROCESSORS_INFO_ERROR_INTERNAL_ERROR;

    status = BootServices->AllocatePool(EfiLoaderData, sizeof(EFI_PROCESSOR_INFORMATION) * totalProcessorsCount, (VOID **) &efiProcessorsInformation);
    if (EFI_ERROR(status))
        return PROCESSORS_INFO_ERROR_INTERNAL_ERROR;

    for (UINTN i = 0; i < totalProcessorsCount; ++i) {
        mpServicesProtocol->GetProcessorInfo(mpServicesProtocol, i, &efiProcessorsInformation[i]);
    }

    processorsInfo->totalProcessorsCount = totalProcessorsCount;
    processorsInfo->enabledProcessorsCount = enabledProcessorsCount;
    processorsInfo->processors = efiProcessorsInformation;

    return PROCESSORS_INFO_SUCCESS_OPERATION;
}

PROCESSORS_INFO_ERROR_CODE ProcessorsInfoGetCountOfEnabledProcessors(PROCESSORS_INFO processorsInfo, UINT64 *enabledProcessorsCount) {
    if (!enabledProcessorsCount)
        return PROCESSORS_INFO_ERROR_INVALID_ARGUMENTS;
    *enabledProcessorsCount = processorsInfo.enabledProcessorsCount;
    return PROCESSORS_INFO_SUCCESS_OPERATION;
}

PROCESSORS_INFO_ERROR_CODE ProcessorsInfoGetTotalCountOfProcessors(PROCESSORS_INFO processorsInfo, UINT64 *totalCount) {
    if (!totalCount)
        return PROCESSORS_INFO_ERROR_INVALID_ARGUMENTS;
    *totalCount = processorsInfo.totalProcessorsCount;
    return PROCESSORS_INFO_SUCCESS_OPERATION;
}

PROCESSORS_INFO_ERROR_CODE ProcessorsInfoGetProcInfoByIndex(PROCESSORS_INFO processorsInfo, UINT64 index, EFI_PROCESSOR_INFORMATION *result) {
    if (!result)
        return PROCESSORS_INFO_ERROR_INVALID_ARGUMENTS;
    if (index >= processorsInfo.totalProcessorsCount)
        return PROCESSORS_INFO_ERROR_INVALID_ARGUMENTS;
    *result = *(processorsInfo.processors + index);
    return PROCESSORS_INFO_SUCCESS_OPERATION;
}
