#include <Kernel/Kernel.h>

Kernel::Kernel(BasicRenderer* pRenderer, BLOCKINFO* bi)
{
	blockinfo = bi;
	renderer = pRenderer;
}

bool Kernel::Init()
{
	renderer->Init();
	gdt.Init();
	idt.Init();

	return true;
}

void Kernel::Run()
{
	renderer->Print("[D] RUN OK! (GDT and IDT should be applied successfully)!\n");
}

void Kernel::Close()
{
	renderer->Close();
}

Kernel::~Kernel()
{
}