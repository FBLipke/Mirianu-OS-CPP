#include <Mirianu-Compiler.h>
#include <types.h>
#include <List.h>

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
#define	PIC_EOI		0x20

class PIC
{
public:
	PIC();
	PIC(uint8_t addr);
	void Send(uint8_t data);

	~PIC();

private:
	uint8_t __addr = 0x20;
	uint8_t __data = 0x21;
	uint8_t __cmd = 0x20;
};

class PICS
{
public:
	PICS();
	~PICS();

	void Init();
	void Send(uint8_t irq);
private:
	List<PIC> PICs;
};

