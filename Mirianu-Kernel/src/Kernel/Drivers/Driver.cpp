#include <Kernel/Drivers/Drivers.h>

Driver::Driver()
{
}

Driver::~Driver()
{
}

void Driver::Init()
{
}

void Driver::Start()
{
}

void Driver::Stop()
{
}

void Driver::Close()
{
}

DriverManager::DriverManager()
{
}

DriverManager::~DriverManager()
{
}

void DriverManager::Start()
{
	for (int i = _drivers.size(); i > 0; i--)
		_drivers[i]->Start();
}

void DriverManager::Add(const CHAR8* key, Driver* drv)
{
	_drivers.push_back(key, drv);
}

void DriverManager::Remove(const CHAR8* key)
{
	_drivers.Remove(key);
}