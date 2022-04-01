#pragma once
#include <efi_mirianu.h>
#include <Kernel/Drivers/Video/BasicRenderer.h>
#include <Kernel/List.h>

#ifdef _ARCH_X86_64
#include <Kernel/Drivers/x86_64/x86_64.h>
#endif

class Driver
{
public:
	Driver();
	~Driver();

	void Init();
	void Start();
	void Stop();
	void Close();
private:
	const char* name;
};

class DriverManager
{
public:
	DriverManager();
	~DriverManager();

	void Start();

	void Add(const CHAR8* key, Driver* drv);
	void Remove(const CHAR8* key);
private:
	List<Driver*> _drivers;
};

