cd ../edk2
. edksetup.sh BaseTools

APP_NAME=${1}

rm -rf MdeModulePkg/Application/${APP_NAME} -v
mkdir ./MdeModulePkg/Application/${APP_NAME} -v

cp -r ../course_work/uefi_application/src/* ./MdeModulePkg/Application/${APP_NAME} -v
cp    ../course_work/uefi_application/${APP_NAME}.inf ./MdeModulePkg/Application/${APP_NAME} -v

cp Conf/target.txt target_backup.txt -v
cp ../scripts/target_app.txt Conf/target.txt -v

build

rm Conf/target.txt -v
cp target_backup.txt Conf/target.txt -v

