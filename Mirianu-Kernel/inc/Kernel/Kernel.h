#pragma once
#include <efi_mirianu.h>
#include <Mirianu-Compiler.h>
#include <Kernel/Drivers/Drivers.h>
#include <Kernel/Memory/EFIMemory.h>
#include <List.h>
#include <Kernel/Arch/x86_64/GDT/gdt.h>
#include <Kernel/Arch/x86_64/IDT/idt.h>

class Kernel
{
public:
	Kernel() {}
	Kernel(MemoryManager* memory, BasicRenderer* pRenderer, BLOCKINFO* bi);
	~Kernel();
	
	bool Init();
	void Run();
	void Close();
private:
	BLOCKINFO* blockinfo = 0;
	DriverManager* drvManager = 0;
	BasicRenderer* renderer = 0;
	MemoryManager* MemManager = 0;
	
	GDT gdt;
	IDT idt;
};
