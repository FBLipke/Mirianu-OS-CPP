#include <Kernel/Kernel.h>

Kernel::Kernel(BasicRenderer* pRenderer, BLOCKINFO* bi)
{
	blockinfo = bi;
	renderer = pRenderer;
}

bool Kernel::Init()
{
	renderer->Init();

	return true;
}

void Kernel::Run()
{
}

void Kernel::Close()
{
	renderer->Close();
}

Kernel::~Kernel()
{
}