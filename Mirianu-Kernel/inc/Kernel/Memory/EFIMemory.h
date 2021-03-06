#pragma once
#include <efi_mirianu.h>
#include <Mirianu-Compiler.h>
#include <Kernel/Drivers/Video/BasicRenderer.h>

class EFIMemory
{
public:
    EFIMemory();
    ~EFIMemory();
    void Print(BLOCKINFO* bi, BasicRenderer* renderer);
private:
};

extern const char* EFI_MEMORY_TYPE_STRINGS[];

struct MemoryChunk
{
    MemoryChunk* next = 0;
    MemoryChunk* prev = 0;
    bool allocated = false;
    size_t size = 0;
};

class MemoryManager
{
protected:
    MemoryChunk* first = 0;
public:
    static MemoryManager* activeMemoryManager;

    MemoryManager();
    bool Init(size_t start, size_t size);
    ~MemoryManager();

    void* malloc(size_t size);
    void free(void* ptr);
};

void* operator new(size_t size);
void* operator new[](size_t size);

// placement new
void* operator new(size_t size, void* ptr);
void* operator new[](size_t size, void* ptr);

void operator delete(void* ptr);
void operator delete[](void* ptr);
