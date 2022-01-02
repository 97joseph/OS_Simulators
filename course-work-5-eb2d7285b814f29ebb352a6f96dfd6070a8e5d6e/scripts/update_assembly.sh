ASSEMBLY_DIR="../course_work/trampoline/src"
SRC_DIR="../course_work/uefi_application/src"

nasm -f bin -o ${ASSEMBLY_DIR}/trampoline.bin ${ASSEMBLY_DIR}/trampoline.asm
xxd -i ${ASSEMBLY_DIR}/trampoline.bin ${ASSEMBLY_DIR}/trampoline.inl
rm ${ASSEMBLY_DIR}/trampoline.bin

mv ${ASSEMBLY_DIR}/trampoline.inl ${SRC_DIR}