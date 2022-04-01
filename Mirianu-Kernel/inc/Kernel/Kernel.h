#pragma once
#include <efi_mirianu.h>
#include <Mirianu-Compiler.h>
#include <Kernel/Drivers/Drivers.h>
#include <Kernel/Memory/EFIMemory.h>
#include <Kernel/List.h>
#include <Kernel/Platform/x86_64/ACPI/ACPI.h>

class Kernel
{
public:
	Kernel(BasicRenderer* pRenderer, BLOCKINFO* bi);
	~Kernel();
	
	bool Init();
	void Run();
	void Close();
private:
	BLOCKINFO* blockinfo;
	DriverManager* drvManager;
	BasicRenderer* renderer;
	GDT* gdt;
	IDT* idt;
	ACPI* acpi;
};

