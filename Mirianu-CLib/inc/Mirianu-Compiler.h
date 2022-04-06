
#ifndef MIRIANU_COMPILER_H
#define MIRIANU_COMPILER_H
#pragma once
#include <types.h>
#define EXTERN						extern "C"

#ifdef _MSC_VER
#include <intrin.h>
#define PACK( __Declaration__ )		__pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#ifdef _WIN64

/// <summary>
/// Halts the microprocessor until an enabled interrupt, a nonmaskable interrupt (NMI), or a reset occurs.
/// </summary>
static void ___Halt() { __halt(); }

/// <summary>
/// Enable interrupts.
/// </summary>
static void ___Sti() { _enable(); }

/// <summary>
/// Disable interrupts.
/// </summary>
static void ___Cli() { _disable(); }

/// <summary>
/// Loads the global descriptor table (GDT) with the value in the specified memory location.
/// </summary>
/// <param name="ptr"></param>
static void ___Lgdt(void* ptr) { _lgdt(ptr); }

/// <summary>
/// Stores the global descriptor table (GDT) with the value in the specified memory location.
/// </summary>
/// <param name="ptr"></param>
static void ___Sgdt(void* ptr) { _sgdt(ptr); }

/// <summary>
/// Loads the interrupt descriptor table register (IDTR) with the value in the specified memory location.
/// </summary>
/// <param name="ptr"></param>
static void ___Lidt(void* ptr) { __lidt(ptr); }

/// <summary>
/// Stores the value of the interrupt descriptor table register (IDTR) in the specified memory location.
/// </summary>
/// <param name="ptr"></param>
static void ___Sidt(void* ptr) { __sidt(ptr); }

/// <summary>
/// Generates the in instruction, returning a single byte read from the port specified by Port.
/// </summary>
/// <param name="port"></param>
/// <returns></returns>
static uint8_t ___inByte(uint16_t port) { __inbyte(port); }

/// <summary>
/// Generates the out instruction, which sends 1 byte specified by Data out the I/O port specified by Port.
/// </summary>
/// <param name="port"></param>
/// <param name="value"></param>
static void ___outByte(uint16_t port, uint8_t value) { __outbyte(port, value); }

/// <summary>
/// Generates the cpuid instruction that is available on x86 and x64.
/// This instruction queries the processor for information about supported features and the CPU type.
/// </summary>
/// <param name="data"></param>
/// <param name="funcId"></param>
static void ___CpuId(int32_t* data, int32_t funcId) { __cpuid(data, funcId); }

/// <summary>
/// Generates the cpuid instruction that is available on x86 and x64.
/// This instruction queries the processor for information about supported features and the CPU type.
/// </summary>
/// <param name="data"></param>
/// <param name="funcId"></param>
/// <param name="SfuncId"></param>
static void ___CpuIdEx(int32_t* data, int32_t funcId, int32_t SfuncId) { __cpuidex(data, funcId, SfuncId); }

/// <summary>
/// Writes the value Data to the CR0 register.
/// </summary>
/// <param name="value"></param>
static void ___WriteCr0(uint64_t value) { __writecr0(value); }

/// <summary>
/// Writes the value Data to the CR2 register.
/// </summary>
/// <param name="value"></param>
static void ___WriteCr2(uint64_t value) { __writecr2(value); }

/// <summary>
/// Writes the value Data to the CR3 register.
/// </summary>
/// <param name="value"></param>
static void ___WriteCr3(uint64_t value) { __writecr3(value); }

/// <summary>
/// Writes the value Data to the CR4 register.
/// </summary>
/// <param name="value"></param>
static void ___WriteCr4(uint64_t value) { __writecr4(value); }

/// <summary>
/// Writes the value Data to the CR8 register.
/// </summary>
/// <param name="value"></param>
static void ___WriteCr8(uint64_t value) { __writecr8(value); }

/// <summary>
/// Reads the CR0 register and returns its value.
/// </summary>
/// <returns></returns>
static uint64_t ___ReadCr0() { return __readcr0(); }

/// <summary>
/// Reads the CR2 register and returns its value.
/// </summary>
/// <returns></returns>
static uint64_t ___ReadCr2() { return __readcr2(); }

/// <summary>
/// Reads the CR3 register and returns its value.
/// </summary>
/// <returns></returns>
static uint64_t ___ReadCr3() { return __readcr3(); }

/// <summary>
/// Reads the CR4 register and returns its value.
/// </summary>
/// <returns></returns>
static uint64_t ___ReadCr4() { return __readcr4(); }

/// <summary>
/// Reads the CR8 register and returns its value.
/// </summary>
/// <returns></returns>
static uint64_t ___ReadCr8() { return __readcr8(); }
#endif // _WIN64
#else
#define _USE_ASM_LoadGDT
#define _USE_ASM_LoadIDT

PACK(__Declaration__) __Declaration__ __attribute__((packed))
#endif // _MSC_VER
#endif // !MIRIANU_COMPILER_H
