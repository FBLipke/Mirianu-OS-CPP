#pragma once
#include <Mirianu-Compiler.h>
#include <Kernel/Drivers/Video/BasicRenderer.h>

#define	GDT_KERNEL_CODE_SEGMENT	0x9A
#define	GDT_KERNEL_DATA_SEGMENT	0x92
#define	GDT_USER_CODE_SEGMENT	0xFA
#define	GDT_USER_DATA_SEGMENT	0xF2

typedef PACK(struct gdt_entry_t
{
	uint16_t limit_low;           // The lower 16 bits of the limit.
	uint16_t base_low;            // The lower 16 bits of the base.
	uint8_t  base_middle;         // The next 8 bits of the base.
	uint8_t  access;              // Access flags, determine what ring this segment can be used in.
	uint8_t  granularity;
	uint8_t  base_high;           // The last 8 bits of the base.
}) gdt_entry_t;


#ifdef _MSC_VER
#ifdef _WIN64
typedef PACK(struct gdt_ptr_t
{
	uint16_t limit;               // The upper 16 bits of all selector limits.
	uintptr_t base;                // The address of the first gdt_entry_t struct.
}) gdt_ptr_t;

#else
typedef PACK(struct gdt_ptr_t
{
	uint16_t limit;               // The upper 16 bits of all selector limits.
	uint32_t base;                // The address of the first gdt_entry_t struct.
}) gdt_ptr_t;
#endif
#else
#endif

#ifdef _MSC_VER
#ifdef _WIN64
extern "C" void LoadGDT(struct gdt_ptr_t gdt_ptr);
#else
extern "C" void LoadGDT(struct gdt_ptr_t gdt_ptr);
#endif
#else
#endif

class GDT
{
public:
	GDT(BasicRenderer* pRenderer);
	~GDT();
	bool Init();

private:
#ifdef _MSC_VER
#ifdef _WIN64
	void Load(struct gdt_ptr_t gdt_ptr);
#else
	void Load(struct gdt_ptr_t gdt_ptr);
#endif
#else
#endif
	BasicRenderer* renderer;
	gdt_entry_t gdt_entries[5];
	gdt_ptr_t gdt_ptr;

	uint8_t num_entries = 0;
	void Set_Gate(uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

};
