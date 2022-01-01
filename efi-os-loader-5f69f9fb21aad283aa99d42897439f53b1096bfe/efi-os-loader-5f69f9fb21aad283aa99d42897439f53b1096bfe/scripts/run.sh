#!/usr/bin/bash

# shellcheck disable=SC2164
cd ../ovmf
qemu-system-x86_64 -enable-kvm -pflash bios.bin -hda fat:rw:hda -net none -device VGA -cpu host -smp cores=4
