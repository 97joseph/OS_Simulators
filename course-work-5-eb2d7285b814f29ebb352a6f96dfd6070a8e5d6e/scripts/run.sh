cd ../run-ovmf
qemu-system-x86_64 -enable-kvm -pflash bios.bin -hda fat:rw:hda-contents -net none -cpu host -smp cores=4 -device VGA -m 4G
