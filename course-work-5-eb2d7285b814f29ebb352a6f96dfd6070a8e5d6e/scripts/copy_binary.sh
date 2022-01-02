APP_NAME=${1}

rm ../run-ovmf/hda-contents/${APP_NAME}.efi -v
cp ../edk2/Build/MdeModule/DEBUG_GCC5/X64/MdeModulePkg/Application/${APP_NAME}/${APP_NAME}/OUTPUT/${APP_NAME}.efi ../run-ovmf/hda-contents/${APP_NAME}.efi -v
