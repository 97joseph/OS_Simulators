cd ../edk2
. edksetup.sh BaseTools

mv ../edk2/Conf/target.txt ../run-ovmf/target_backup.txt -v
cp ../run-ovmf/target_ovmf.txt ../edk2/Conf/target.txt -v
build
rm ../edk2/Conf/target.txt -v
mv ../run-ovmf/target_backup.txt ../edk2/Conf/target.txt -v
