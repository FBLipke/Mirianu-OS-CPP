#pragma once
#include <efi_mirianu.h>
#include <Kernel/Drivers/Video/BasicRenderer.h>

class ACPI
{
public:
	ACPI(BLOCKINFO* pBlockinfo, BasicRenderer* pRenderer);
	~ACPI();

	bool Init();
	MCFGHeader* Get_MCFGTable();
	address_t* Find_Table(SDTHeader* sdt_header, const char* signature);
private:
	BasicRenderer* renderer;
	BLOCKINFO* blockinfo;
};
