#include <Kernel/Arch/x86_64/PIC/pic.h>

PIC::PIC() {}
PIC::~PIC() {}

PIC::PIC(uint8_t addr)
{
	__addr = addr;
	__data = __addr + 1;
	__cmd = __addr;
}

void PIC::Send(uint8_t data)
{
	outb(__cmd, data);
}


PICS::PICS()
{
}

void PICS::Init()
{
	PICs.push_back("M", PIC(PIC1));
	PICs.push_back("S", PIC(PIC2));
}

void PICS::Send(uint8_t irq)
{
	if (irq >= 8) // Also send to slave PIC...
		PICs.GetAt(1).Send(PIC_EOI);

	PICs.GetAt(0).Send(PIC_EOI);
}

PICS::~PICS()
{
}
