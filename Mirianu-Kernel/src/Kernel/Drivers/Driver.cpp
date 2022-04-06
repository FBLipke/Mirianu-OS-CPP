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

void DriverManager::Init()
{
	for (int i = _drivers.size(); i > 0; i--)
		_drivers[i]->Init();
}

void DriverManager::Stop()
{
	for (int i = _drivers.size(); i > 0; i--)
		_drivers[i]->Stop();
}

void DriverManager::Close()
{
	for (int i = _drivers.size(); i > 0; i--)
		_drivers[i]->Close();
}


void DriverManager::Add(const CHAR8* key, Driver* drv)
{
	_drivers.push_back(key, drv);
}

void DriverManager::Remove(const CHAR8* key)
{
	_drivers.Remove(key);
}
