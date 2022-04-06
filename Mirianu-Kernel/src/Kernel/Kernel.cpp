#include <Kernel/Kernel.h>

Kernel::Kernel(BasicRenderer* pRenderer, BLOCKINFO* bi)
{
	blockinfo = bi;
	renderer = pRenderer;
}

bool Kernel::Init()
{
	renderer->Init();

	renderer->Print("INIT\n");
	return true;
}

void Kernel::Run()
{
	renderer->Print("RUN\n");
}

void Kernel::Close()
{
	renderer->Print("CLOSE\n");
	renderer->Close();
}

Kernel::~Kernel()
{
}