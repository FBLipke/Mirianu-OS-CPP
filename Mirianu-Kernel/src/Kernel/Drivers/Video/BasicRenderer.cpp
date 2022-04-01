#include <Kernel/Drivers/Video/BasicRenderer.h>

BasicRenderer::BasicRenderer(Framebuffer* pFrambuffer)
{
	fb = pFrambuffer;
	CursorPosition = { 0,0 };
}

void BasicRenderer::Init()
{
	Print("[K] Console Output: EFI-Framebuffer\n");
}

void BasicRenderer::ClearScreen()
{
	for (unsigned long y = 0; y < fb->Height; y++)
	{
		CursorPosition.Y += PSF1_CHRHEIGHT;
		
		for (unsigned long x = 0; x < fb->Width; x++)
		{
			CursorPosition.X += PSF1_CHRWIDTH;
			Print(" ");
		}
	}
}

void BasicRenderer::Print(const char* str)
{
	char* chr = (char*)str;
	while (*chr != 0)
	{
		CursorPosition.X += PSF1_CHRWIDTH;

		if (CursorPosition.X + PSF1_CHRWIDTH > fb->Width || *chr == '\n')
		{
			CursorPosition.X = 0;
			CursorPosition.Y += PSF1_CHRHEIGHT;
		}

		if (*chr != '\n')
			putChar(*chr, CursorPosition.X, CursorPosition.Y);

		chr++;
	}
}

void BasicRenderer::putChar(char chr, uint32_t xOff, uint32_t yOff)
{
	uint32_t* pixPtr = (uint32_t*)fb->BaseAddress;
	char* fontPtr = (char*)fb->psf1font->glyphBuffer + (chr * fb->psf1font->psf1_Header->charsize);
	
	for (uint32_t y = yOff; y < yOff + PSF1_CHRHEIGHT; y++)
	{
		for (uint32_t x = xOff; x < xOff + PSF1_CHRWIDTH; x++)
			if ((*fontPtr & (0x80 >> (x - xOff))) > 0)
				*(uint32_t*)(pixPtr + x + (y * fb->PixelsPerScanLine)) = fb->color;

		fontPtr++;
	}
}

BasicRenderer::~BasicRenderer()
{
}
