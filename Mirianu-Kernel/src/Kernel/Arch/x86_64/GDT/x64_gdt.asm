section .text
bits 64
global LoadGDT  ; in gdt.cpp defined as extern "C" void LoadGDT
LoadGDT:        
    lgdt [rcx]  ; Load data in the RCX register an return.
    ret
