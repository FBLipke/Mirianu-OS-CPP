#pragma once
#include <Mirianu-Compiler.h>
#include <Kernel/Drivers/Video/BasicRenderer.h>

#define idt_ta_interrupt_gate	0b10001110
#define idt_ta_call_gate		0b10001100
#define idt_ta_trap_gate		0b10001111

typedef PACK(struct idt_desc_entry_t {
	uint16_t offset0;
	uint16_t selector;
	uint8_t ist;
	uint8_t type_attr;
	uint16_t offset1;
	uint32_t offset2;
	uint32_t ignore;
	void set_offset(uint64_t offset);
	uint64_t get_offset();
}) idt_desc_entry_t;

typedef PACK(struct idt_t {
	uint16_t Limit;
	uint64_t Offset;
}) idt_t;

class IDT
{
public:
	IDT(BasicRenderer* pRenderer);
	~IDT();

	void Init();

private:
#ifdef _MSC_VER
#ifdef _WIN64
	void Load(uint64_t idt_ptr);
#else
	void Load(uint32_t idt_ptr);
#endif
#else
#endif
	BasicRenderer* renderer;
};

