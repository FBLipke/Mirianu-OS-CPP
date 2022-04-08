#include <Kernel/Kernel.h>

Kernel::Kernel(MemoryManager* memory, BasicRenderer* pRenderer, BLOCKINFO* bi)
{
	blockinfo = bi;
	renderer = pRenderer;
	MemManager = memory;
}

bool Kernel::Init()
{
	renderer->Init();
	gdt.Init();
	idt.Init();

	MemManager->Init(1000,4096);
	renderer->Print("[D] RUN OK! (MEMORY) should be applied successfully)!\n");
	return true;
}

void Kernel::Run()
{
	renderer->Print("[D] RUN OK!\n");
}

void Kernel::Close()
{
	renderer->Close();
}

Kernel::~Kernel()
{
}