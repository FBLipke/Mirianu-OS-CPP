BITS 64
section .text

global LoadIDT
LoadIDT:
lidt [rcx]
ret

save_interrupt_registers:
pop rax
push rcx
push rdx
push r8
push r9
push r10
push r11
jmp rax

save_fpu_registers:
pop rax
;stack is aligned
sub rsp, 16*6
movdqa [rsp], xmm0
movdqa [rsp+16], xmm1
movdqa [rsp+32], xmm2
movdqa [rsp+48], xmm3
movdqa [rsp+64], xmm4
movdqa [rsp+80], xmm5
jmp rax
extern x64_interrupt_dispatcher	; Function body in idt.cpp

restore_interrupt_registers:
pop rax
pop r11
pop r10
pop r9
pop r8
pop rdx
pop rcx
jmp rax

restore_fpu_registers:
pop rax
;stack is aligned
movdqa xmm0, [rsp]
movdqa xmm1, [rsp+16]
movdqa xmm2, [rsp+32]
movdqa xmm3, [rsp+48]
movdqa xmm4, [rsp+64]
movdqa xmm5, [rsp+80]
add rsp, 16*6
jmp rax

swap_gs_ifpriv:
mov rax, QWORD[rbp+8+0x10]
and rax, 0x3	;CPL
cmp rax, 0
je .next
swapgs
.next:
ret


%macro SAVE_VOLATILE_REGISTERS 0
push rax
call save_interrupt_registers
%endmacro

%macro RESTORE_VOLATILE_REGISTERS 0
call restore_interrupt_registers
pop rax
%endmacro

%macro INTERRUPT_HANDLER 2
global isr%2
isr%2:
cld
%if %1 == 0
push 0xDEADBEEF			;Dummy error code
%endif
;Stack frame
push rbp
mov rbp, rsp
SAVE_VOLATILE_REGISTERS
;Per CPU information
call swap_gs_ifpriv

;Align the stack
mov rcx, rsp
and sp, 0xFFF0
push rcx
push rcx

;Save volatile FPU registers
call save_fpu_registers

;call save_fpu_interrupt

;Now we pass the stack interrupt stack and vector
mov rcx, %2
mov rdx, rbp
;add rdx, 8

sub rsp, 32
call x64_interrupt_dispatcher
add rsp, 32

call restore_fpu_registers

;Restore the stack
pop rcx
mov rsp, rcx

;call restore_fpu_interrupt
call swap_gs_ifpriv

RESTORE_VOLATILE_REGISTERS
pop rbp
add rsp, 8		;Get rid of error code

iretq
%endmacro

%macro INTERRUPT_HANDLER_BLOCK 3
%assign i %2
%rep %3-%2
INTERRUPT_HANDLER %1, i
%assign i i+1
%endrep
%endmacro

INTERRUPT_HANDLER 0, 0
INTERRUPT_HANDLER 0, 1
INTERRUPT_HANDLER 0, 2
INTERRUPT_HANDLER 0, 3
INTERRUPT_HANDLER 0, 4
INTERRUPT_HANDLER 0, 5
INTERRUPT_HANDLER 0, 6
INTERRUPT_HANDLER 0, 7
INTERRUPT_HANDLER 1, 8
INTERRUPT_HANDLER 0, 9
INTERRUPT_HANDLER 1, 10
INTERRUPT_HANDLER 1, 11
INTERRUPT_HANDLER 1, 12
INTERRUPT_HANDLER 1, 13
INTERRUPT_HANDLER 1, 14
INTERRUPT_HANDLER 0, 15
INTERRUPT_HANDLER 0, 16
INTERRUPT_HANDLER 1, 17
INTERRUPT_HANDLER 0, 18
INTERRUPT_HANDLER 0, 19
INTERRUPT_HANDLER 0, 20
INTERRUPT_HANDLER_BLOCK 0, 21, 30
INTERRUPT_HANDLER 1, 30
INTERRUPT_HANDLER 0, 31
INTERRUPT_HANDLER_BLOCK 0, 32, 256

%macro dq_concat 2
dq %1%2
%endmacro

section .data
global default_irq_handlers
default_irq_handlers:
%assign i 0
%rep 256
dq_concat isr,i
%assign i i+1
%endrep
stack_str: dw __utf16__(`Interrupt frame:\n Error code %x\n Return RIP: %x\n Return CS: %x\n Return RFLAGS: %x\n Return RSP: %x\n Return SS: %x\n`), 0