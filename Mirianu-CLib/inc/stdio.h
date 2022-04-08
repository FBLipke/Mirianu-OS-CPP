#pragma once
#include <Mirianu-Compiler.h>

static void ___memset(void* s, int c, unsigned int len)
{
	unsigned char* p = (unsigned char*)s;

	while (len--) {
		*p++ = (unsigned char)c;
	}
}
