#include <Kernel/Arch/x86_64/IDT/idt.h>

#define		INTR_DIVISION_BY_ZERO		0
#define		INTR_SINGLE_STEP_INTR		1
#define		INTR_NONMASKABLE_INTR		2
#define		INTR_BREAKPOINT				3
#define		INTR_OVERFLOW				4
#define		INTR_BOUND_RANGE_EXCEEDED	5
#define		INTR_INVALID_OPCODE			6
#define		INTR_COCPU_NOT_AVAIL		7
#define		INTR_DOUBLE_FAULT			8
#define		INTR_SEGMENT_OVERRUN		9
#define		INTR_INVALID_TSS			10
#define		INTR_SEGMENT_NOT_PRESENT	11
#define		INTR_STACK_SEGMENT_FAULT	12
#define		INTR_GENERAL_PROTECTION		13
#define		INTR_PAGE_FAULT				14
#define		INTR_INTEL_0_RESERVED_15	15
#define		INTR_X87_FLOATING_ERROR		16

#define		INTR_ALIGNMENT_CHECK		17
#define		INTR_MACHINE_CHECK			18
#define		INTR_SIMD_FLOAT_EXCEPTION	19
#define		INTR_VIRT_EXCEPTION			20
#define		INTR_CONTROL_PROTECTION		21

#define		IDT_FLAGS_INTERRUPT_GATE	0x8E
#define		IDT_FLAGS_TRAP_GATE			0x8F
#define		IDT_FLAGS_TASK_GATE			0x85

void IDT::Init()
{
	___memset(&idt_entries, 0, sizeof(idt_entry_t) * MAX_BYTE);

	Set_Gate(INTR_DIVISION_BY_ZERO, (uint32_t)isr0, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_SINGLE_STEP_INTR, (uint32_t)isr1, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_NONMASKABLE_INTR, (uint32_t)isr2, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_BREAKPOINT, (uint32_t)isr3, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_OVERFLOW, (uint32_t)isr4, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_BOUND_RANGE_EXCEEDED, (uint32_t)isr5, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_INVALID_OPCODE, (uint32_t)isr6, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_COCPU_NOT_AVAIL, (uint32_t)isr7, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_DOUBLE_FAULT, (uint32_t)isr8, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_SEGMENT_OVERRUN, (uint32_t)isr9, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_INVALID_TSS, (uint32_t)isr10, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_SEGMENT_NOT_PRESENT, (uint32_t)isr11, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_STACK_SEGMENT_FAULT, (uint32_t)isr12, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_GENERAL_PROTECTION, (uint32_t)isr13, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_PAGE_FAULT, (uint32_t)isr14, 0x08, IDT_FLAGS_INTERRUPT_GATE);

	Set_Gate(INTR_INTEL_0_RESERVED_15, (uint32_t)isr15, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_X87_FLOATING_ERROR, (uint32_t)isr16, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_ALIGNMENT_CHECK, (uint32_t)isr17, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_MACHINE_CHECK, (uint32_t)isr18, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_SIMD_FLOAT_EXCEPTION, (uint32_t)isr19, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_VIRT_EXCEPTION, (uint32_t)isr20, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(INTR_CONTROL_PROTECTION, (uint32_t)isr21, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(22, (uint32_t)isr22, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(23, (uint32_t)isr23, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(24, (uint32_t)isr24, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(25, (uint32_t)isr25, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(26, (uint32_t)isr26, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(27, (uint32_t)isr27, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(28, (uint32_t)isr28, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(29, (uint32_t)isr29, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(30, (uint32_t)isr30, 0x08, IDT_FLAGS_INTERRUPT_GATE);
	Set_Gate(31, (uint32_t)isr31, 0x08, IDT_FLAGS_INTERRUPT_GATE);

	idt_ptr.limit = sizeof(idt_entry_t) * MAX_BYTE - 1;
	idt_ptr.base = (uint32_t)&idt_entries;

	LoadIDT((uint32_t)&idt_ptr);
	___Sti();
}

void x64_interrupt_dispatcher(size_t vector, interrupt_stack_frame* istack_frame)
{
}

IDT::IDT() {}

void IDT::Set_Gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
	idt_entries[num].base_lo = base & 0xFFFF;
	idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

	idt_entries[num].sel = sel;
	idt_entries[num].reserved = NULL;
	idt_entries[num].flags = flags;
}

IDT::~IDT()
{
}
