[bits 16]
; Forbid all interrupts
cli

in  al  , 0x70
or  al  , 0x80
out 0x70, al

; Open A20 gate
in  al  , 0x92
or  al  , 0x02
out 0x92, al

mov ax, cs
mov ds, ax

; Calculate GDT linear address
xor eax, eax
mov eax, GDT
add eax, 0x10000
mov dword[GDTR_GDT_LA], eax

; Calculate PROT_MODE linear address
xor eax, eax
mov eax, PROT_MODE
add eax, 0x10000
mov dword[PROT_MODE_LA], eax

; Calculate LONG_MODE linear address
xor eax, eax
mov eax, LONG_MODE
add eax, 0x10000
mov dword[LONG_MODE_LA], eax

; Load GDTR register
o32 lgdt [GDTR]

; Switch to protected mode
mov eax, cr0
or  al , 0x01
mov cr0, eax

o32 jmp far [PROT_MODE_PTR]

PROT_MODE_PTR:
PROT_MODE_LA:
  dd PROT_MODE
  dw 0x8 ; SELECTOR

GDT:
  D_NULL   db 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  D_CODE32 db 0xFF, 0xFF, 0x00, 0x00, 0x00, 10011010b, 11001111b, 0x00; 0x08
  D_DATA32 db 0xFF, 0xFF, 0x00, 0x00, 0x00, 10010010b, 11001111b, 0x00; 0x10
  D_CODE64 db 0x00, 0x00, 0x00, 0x00, 0x00, 10011010b, 00100000b, 0x00; 0x18
  D_DATA64 db 0x00, 0x00, 0x00, 0x00, 0x00, 10010010b, 00100000b, 0x00; 0x20
  D_DATAM  db 0xFF, 0xFF, 0x00, 0x00, 0x01, 10010010b, 11001111b, 0x00; 0x28
  GDT_SIZE equ $-GDT

GDTR:
  dw GDT_SIZE
GDTR_GDT_LA:
  dd GDT


[bits 32]
PROT_MODE:

; Set DS and ES register to data desc
mov ax, 0x10
mov ds, ax
mov es, ax

; Turn on PAE (5 in CR4)
mov eax, cr4
bts eax, 5   ; CR4-PAE
mov cr4, eax

; Create pages
; Clear everything
mov edi, 0x100000
mov ecx, 0x0C0000
xor eax, eax
clear_all:
	mov dword[es:edi], eax
loop clear_all

; PML4E
mov dword[es:0x100000], 0x110000 + 111b

; PDPTE (4GB memory/1 GB catalogue = 4 catalogues)
mov dword[es:0x110000], 0x120000 + 111b
mov dword[es:0x110008], 0x121000 + 111b
mov dword[es:0x110010], 0x122000 + 111b
mov dword[es:0x110018], 0x123000 + 111b

; PDE (4GB memory/2MB pages = 2048 pages = 0x800 )
mov edi, 0x120000
mov eax, 0x0 + 10000111b
mov ebx, 0x0
mov ecx, 0x800
generate_pdes:
	mov dword[es:edi], eax
	add edi, 0x4
	mov dword[es:edi], ebx
	add eax, 0x200000
	jnc no_overflow
	    mov ebx, 0x1
	no_overflow:
	add edi, 0x4
loop generate_pdes

; Load PML4 to CR3
mov eax, 0x100000 ; PML4 address
mov cr3, eax

; Switch LME in EFER
mov ecx, 0x0C0000080 ; EFER
rdmsr
bts eax, 8 ; EFER.LME
wrmsr

; Turn on paging (PG in CR0)
mov eax, cr0
bts eax, 31 ; CR0-PG
mov cr0, eax

; Set ES register to 10K offset
mov ax, 0x28
mov es, ax

jmp far [es:LONG_MODE_PTR]

LONG_MODE_PTR:
LONG_MODE_LA:
	dd 0x0
	dw 0x18

LONG_MODE:
[bits 64]

; Set up segment registers
mov ax, 0x20
mov es, ax
;   cs - will be set by jumping
mov ss, ax
mov ds, ax
mov fs, ax
mov gs, ax

; Setup stack
mov rsp, 0x2000000

; Jump to C code
mov rax, 0x1500000
jmp rax
