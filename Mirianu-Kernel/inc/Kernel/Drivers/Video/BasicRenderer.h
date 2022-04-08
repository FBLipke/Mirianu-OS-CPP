#pragma once
#include <efi_mirianu.h>

class BasicRenderer
{
public:
	BasicRenderer(Framebuffer* pFrambuffer);
	void Init();
	void Close();
	void ClearScreen();
	void Print(const char* str);
	~BasicRenderer();
	Point CursorPosition;
private:
	void putChar(char chr, uint32_t xOff, uint32_t yOff);
	Framebuffer* fb = 0;
};
