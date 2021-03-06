#include <Kernel_Main.h>

int kmain(BLOCKINFO* bi)
{
	BasicRenderer renderer = BasicRenderer(bi->frameBuffer);
	MemoryManager memManagr;
	Kernel krnl = Kernel(&memManagr, &renderer, bi);
	
	if (!krnl.Init())
		return 101;

	krnl.Run();
	krnl.Close();

	while (1) {	___Halt(); }
	
	return 0;
}
