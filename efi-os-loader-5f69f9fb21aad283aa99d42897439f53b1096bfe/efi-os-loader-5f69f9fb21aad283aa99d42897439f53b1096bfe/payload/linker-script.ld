ENTRY(_entry);

PHDRS
{
    load PT_LOAD;
}

SECTIONS
{
    . = 0x00;
    .entry  ALIGN(0x10) : {   *(.entry*)         } : load
    .text   ALIGN(0x10) : {   *(.text*)          } : load
    .rodata ALIGN(0x10) : {   *(.rodata*)        } : load
    .data   ALIGN(0x10) : {   *(.data*)          } : load
    .bss    ALIGN(0x10) : {   *(.bss*)           } : load

    /DISCARD/           : {   *(.comment)       }
}