CC=/usr/bin/gcc
LD=/usr/bin/ld
CFLAGS=\
"-Wall -Wextra -Wdisabled-optimization -Wdiv-by-zero -Werror \
-Wfloat-equal -Wint-to-pointer-cast \
-Wmissing-include-dirs -Wnested-externs \
-Wno-main -Woverflow -Wparentheses -Wpointer-arith \
-Wpointer-to-int-cast -Wredundant-decls -Wshadow \
-Wstrict-prototypes -Wtrigraphs -Wundef -Wunused-parameter \
-Wvariadic-macros -Wvla -Wwrite-strings -fpie \
-fno-asynchronous-unwind-tables -fno-unwind-tables \
-fno-stack-protector -fno-builtin -nostdinc -nostdlib \
-fno-strict-aliasing -Wl,--build-id=none -std=c11 -pedantic \
-c -s -O0"
LDFLAGS="-s -T ../linker_script.ld --nmagic"

${CC} -m64 ${CFLAGS} ${@}

OBJ_FILES=""
for var in "$@"
do
    OBJ_FILES="${OBJ_FILES} ${var%.c}.o"
done

${LD} -melf_x86_64 ${LDFLAGS} -o ../c_code.elf ${OBJ_FILES}

for var in "$@"
do
    rm "${var%.c}.o"
done

objcopy -O binary ../c_code.elf ../c_code.bin
rm ../c_code.elf

xxd -i ../c_code.bin ../../uefi_application/src/c_code.inl
rm ../c_code.bin