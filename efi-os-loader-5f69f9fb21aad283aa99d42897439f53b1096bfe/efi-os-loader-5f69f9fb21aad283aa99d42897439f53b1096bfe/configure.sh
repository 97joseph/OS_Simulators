#!/usr/bin/bash

git submodule update --init --recursive
# shellcheck disable=SC2164
cd edk2
pip install -r pip-requirements.txt
make -C BaseTools
source ./edksetup.sh
make -C BaseTools
source ./edksetup.sh BaseTools

mv ../edk2/Conf/target.txt ../ovmf/ovmf-target-backup.txt
cp ../ovmf/ovmf-target.txt Conf/target.txt112.255.118.72
../edk2/BaseTools/BinWrappers/PosixLike/build
rm ../edk2/Conf/target.txt
mv ../ovmf/ovmf-target-backup.txt ../edk2/Conf/target.txt
cp ../edk2/Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd ../ovmf/bios.bin

# shellcheck disable=SC2164
cd MdeModulePkg
sed -i '/.*CourseWork.inf/d' MdeModulePkg.dsc
sed -i '/.*HelloWorld.inf/a \ \ MdeModulePkg\/Application\/CourseWork\/CourseWork.inf' MdeModulePkg.dsc
