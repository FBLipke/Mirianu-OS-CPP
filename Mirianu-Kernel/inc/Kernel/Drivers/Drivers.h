#pragma once
#include <efi_mirianu.h>
#include <Kernel/Drivers/Video/BasicRenderer.h>
#include <List.h>

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
	const char* name = "";
};

class DriverManager
{
public:
	DriverManager();
	~DriverManager();

	void Start();
	void Init();
	void Stop();
	void Close();
	void Add(const CHAR8* key, Driver* drv);
	void Remove(const CHAR8* key);
private:
	List<Driver*> _drivers;
};

