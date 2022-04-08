#pragma once
#include <Mirianu-Compiler.h>
#include <stdio.h>

typedef PACK(struct idt_entry_t
{
	uint16_t base_lo = 0;
	uint16_t sel = 0;
	uint16_t reserved = 0;
	uint16_t flags = 0;
	uint16_t base_hi = 0;
}) idt_entry_t;

typedef PACK(struct idt_ptr_t
{
	uint16_t limit = 0;
	uint16_t base = 0;
}) idt_ptr_t;

struct stack_frame {
	stack_frame* baseptr = 0;
	size_t rip = 0;
};

PACK(struct interrupt_stack_frame {
	stack_frame* baseptr = 0;
	size_t error = 0;
	size_t rip = 0;
	size_t cs = 0;
	size_t rflags = 0;
	size_t rsp = 0;
	size_t ss = 0;
});

extern "C" void x64_interrupt_dispatcher(size_t vector, interrupt_stack_frame * istack_frame);
extern "C" void LoadIDT(uint32_t ptr);
extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();

class IDT
{
public:
	IDT();
	~IDT();
	void Init();

private:
	void Set_Gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

	idt_entry_t idt_entries[MAX_BYTE];
	idt_ptr_t idt_ptr;
};
