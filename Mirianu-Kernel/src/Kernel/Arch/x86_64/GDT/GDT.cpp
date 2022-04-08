#include <Kernel/Arch/x86_64/GDT/gdt.h>

extern "C" void LoadGDT(gdt_ptr_t & gdt_ptr);

void GDT::Init()
{

	___memset(&gdt_entries, 0, sizeof(gdt_entry_t) * GDT_ENTRIES);
	Set_Gate(0, 0, 0, 0, 0);
	Set_Gate(1, 0, 0xFFFFF, GDT_KERNEL_CODE_SEGMENT, 0xA);
	Set_Gate(2, 0, 0xFFFFF, GDT_KERNEL_DATA_SEGMENT, 0xC);

	Set_Gate(3, 0, 0xFFFFF, GDT_USER_CODE_SEGMENT, 0xA);
	Set_Gate(4, 0, 0xFFFFF, GDT_USER_DATA_SEGMENT, 0xC);
	// TODO: Implement TSS to set this
	// Set_Gate(5, &TSS, sizeof(struct TSS), GDT_TASK_STATE_SEGMENT, 0x0); // User mode data segment

	{
		gdt_ptr_t gdt_ptr;
		
		gdt_ptr.limit = (sizeof(gdt_entry_t) * GDT_ENTRIES) - 1;
		gdt_ptr.base = (uint32_t)&gdt_entries;

		___Cli();
		LoadGDT(gdt_ptr);
	}
}

GDT::GDT()
{
}

GDT::~GDT()
{
}

void GDT::Set_Gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
	gdt_entries[num].base_low = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high = (base >> 24) & 0xFF;

	gdt_entries[num].limit_low = (limit & 0xFFFF);
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;

	gdt_entries[num].granularity |= gran & 0xF0;
	gdt_entries[num].access = access;
}
