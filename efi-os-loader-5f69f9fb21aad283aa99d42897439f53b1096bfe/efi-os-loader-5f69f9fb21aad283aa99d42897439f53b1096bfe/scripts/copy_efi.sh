#!/usr/bin/bash

rm ../ovmf/hda/*.efi
cp ../edk2/Build/MdeModule/DEBUG_GCC5/X64/MdeModulePkg/Application/CourseWork/CourseWork/OUTPUT/CourseWork.efi ../ovmf/hda/CourseWork.efi