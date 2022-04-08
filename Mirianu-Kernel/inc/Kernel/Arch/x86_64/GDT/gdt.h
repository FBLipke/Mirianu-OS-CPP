#pragma once
#include <Mirianu-Compiler.h>
#include <stdio.h>

#define	GDT_KERNEL_CODE_SEGMENT	0x9A
#define	GDT_KERNEL_DATA_SEGMENT	0x92

#define	GDT_USER_CODE_SEGMENT	0xFA
#define	GDT_USER_DATA_SEGMENT	0xF2

#define GDT_TASK_STATE_SEGMENT	0x89

#define GDT_ENTRIES				5

typedef PACK(struct gdt_entry_struct
{
	uint16_t limit_low = 0;
	uint16_t base_low = 0;

	uint8_t  base_middle = 0;
	uint8_t  access = 0;
	uint8_t  granularity = 0;
	uint8_t  base_high = 0;
}) gdt_entry_t;

typedef PACK(struct gdt_ptr_struct
{
	uint16_t limit = 0;
	uint32_t base = 0;
}) gdt_ptr_t;

class GDT
{
public:
	GDT();
	~GDT();

	void Init();

private:
	void Set_Gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
	gdt_entry_struct gdt_entries[GDT_ENTRIES];
};
