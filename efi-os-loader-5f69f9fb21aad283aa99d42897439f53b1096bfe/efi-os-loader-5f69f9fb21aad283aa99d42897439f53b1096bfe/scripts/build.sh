#!/usr/bin/bash

# shellcheck disable=SC2164
cd ../src
nasm jumper.S -o jumper.bin
xxd -i jumper.bin >jumper.h
rm jumper.bin

cd ../payload

# shellcheck disable=SC1007
#PAYLOAD_CFLAGS=" -Wall -Wextra -Wdisabled-optimization -Wdiv-by-zero -Werror \
#-Wfloat-equal -Wint-to-pointer-cast \
#-Wmissing-include-dirs -Wnested-externs \
#-Wno-main -Woverflow -Wparentheses -Wpointer-arith \
#-Wpointer-to-int-cast -Wredundant-decls -Wshadow \
#-Wstrict-prototypes -Wtrigraphs -Wundef -Wunused-parameter \
#-Wvariadic-macros -Wvla -Wwrite-strings -fpie \
#-fno-asynchronous-unwind-tables -fno-unwind-tables \
#-fno-stack-protector -fno-builtin -nostdinc -nostdlib \
#-fno-strict-aliasing -Wl,--build-id=none -std=c11 -pedantic \
#-c -s -O0"

# shellcheck disable=SC2034
gcc -m64 -Wall -Wextra -Wdisabled-optimization -Wdiv-by-zero -Werror -Wfloat-equal -Wint-to-pointer-cast \
  -Wmissing-include-dirs -Wnested-externs -Wno-main -Woverflow -Wparentheses -Wpointer-arith -Wpointer-to-int-cast \
  -Wredundant-decls -Wshadow -Wstrict-prototypes -Wtrigraphs -Wundef -Wunused-parameter -Wvariadic-macros -Wvla \
  -Wwrite-strings -fpie -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-stack-protector -fno-builtin -nostdinc \
  -nostdlib -fno-strict-aliasing -Wl,--build-id=none -std=c11 -pedantic -c -s -O0 \
  main.c -o main.o
ld -melf_x86_64 -s -T linker-script.ld --nmagic main.o -o payload.elf
objcopy -O binary payload.elf payload.bin
xxd -i payload.bin >../src/payload.h
rm main.o
rm payload.elf
rm payload.bin

cd ../edk2
. edksetup.sh BaseTools

rm -rf MdeModulePkg/Application/CourseWork
mkdir MdeModulePkg/Application/CourseWork
cp -r ../src/* MdeModulePkg/Application/CourseWork

mv Conf/target.txt target_backup.txt
cp ../src/app-target.txt Conf/target.txt

build

rm Conf/target.txt
cp target_backup.txt Conf/target.txt
