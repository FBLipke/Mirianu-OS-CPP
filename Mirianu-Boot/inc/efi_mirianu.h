#ifndef EFI_MIRIANU_H
#define EFI_MIRIANU_H 1
#pragma once
#include <efi.h>
#include <efilib.h>

#ifndef fltused
#define fltused 1

#endif
static EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;

#include <Mirianu-Compiler.h>


typedef struct _IMAGE_DOS_HEADER {      // DOS .EXE header
    UINT16   e_magic;                     // Magic number
    UINT16   e_cblp;                      // Bytes on last page of file
    UINT16   e_cp;                        // Pages in file
    UINT16   e_crlc;                      // Relocations
    UINT16   e_cparhdr;                   // Size of header in paragraphs (1 paragraph = 16 bytes)
    UINT16   e_minalloc;                  // Minimum extra paragraphs needed
    UINT16   e_maxalloc;                  // Maximum extra paragraphs needed
    UINT16   e_ss;                        // Initial (relative) SS value
    UINT16   e_sp;                        // Initial SP value
    UINT16   e_csum;                      // Checksum
    UINT16   e_ip;                        // Initial IP value
    UINT16   e_cs;                        // Initial (relative) CS value
    UINT16   e_lfarlc;                    // File address of relocation table
    UINT16   e_ovno;                      // Overlay number
    UINT16   e_res[4];                    // Reserved words
    UINT16   e_oemid;                     // OEM identifier (for e_oeminfo)
    UINT16   e_oeminfo;                   // OEM information; e_oemid specific
    UINT16   e_res2[10];                  // Reserved words
    UINT32   e_lfanew;                    // File address of new exe header
} IMAGE_DOS_HEADER, * PIMAGE_DOS_HEADER;

#define IMAGE_NT_SIGNATURE                  0x00004550  // PE00
#define IMAGE_DOS_SIGNATURE                 0x5A4D      // MZ
#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16
#define IMAGE_NT_OPTIONAL_HDR64_MAGIC      0x20b
#define IMAGE_FILE_MACHINE_X64               0x8664  // x86_64

#define IMAGE_SIZEOF_SHORT_NAME              8
typedef struct _IMAGE_SECTION_HEADER {
    UINT8   Name[IMAGE_SIZEOF_SHORT_NAME];
    union {
        UINT32   PhysicalAddress;
        UINT32   VirtualSize;
    } Misc;
    UINT32   VirtualAddress;
    UINT32   SizeOfRawData;
    UINT32   PointerToRawData;
    UINT32   PointerToRelocations;
    UINT32   PointerToLinenumbers;
    UINT16   NumberOfRelocations;
    UINT16   NumberOfLinenumbers;
    UINT32   Characteristics;
} IMAGE_SECTION_HEADER, * PIMAGE_SECTION_HEADER;

typedef struct _IMAGE_DATA_DIRECTORY {
    UINT32   VirtualAddress;
    UINT32   Size;
} IMAGE_DATA_DIRECTORY, * PIMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_OPTIONAL_HEADER64 {
    UINT16  Magic; /* 0x20b */
    UINT8   MajorLinkerVersion;
    UINT8   MinorLinkerVersion;
    UINT32  SizeOfCode;
    UINT32  SizeOfInitializedData;
    UINT32  SizeOfUninitializedData;
    UINT32  AddressOfEntryPoint;
    UINT32  BaseOfCode;
    UINT64  ImageBase;
    UINT32  SectionAlignment;
    UINT32  FileAlignment;
    UINT16  MajorOperatingSystemVersion;
    UINT16  MinorOperatingSystemVersion;
    UINT16  MajorImageVersion;
    UINT16  MinorImageVersion;
    UINT16  MajorSubsystemVersion;
    UINT16  MinorSubsystemVersion;
    UINT32  Win32VersionValue;
    UINT32  SizeOfImage;
    UINT32  SizeOfHeaders;
    UINT32  CheckSum;
    UINT16  Subsystem;
    UINT16  DllCharacteristics;
    UINT64  SizeOfStackReserve;
    UINT64  SizeOfStackCommit;
    UINT64  SizeOfHeapReserve;
    UINT64  SizeOfHeapCommit;
    UINT32  LoaderFlags;
    UINT32  NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER64, * PIMAGE_OPTIONAL_HEADER64;

typedef struct _IMAGE_FILE_HEADER {
    UINT16   Machine;
    UINT16   NumberOfSections;
    UINT32   TimeDateStamp;
    UINT32   PointerToSymbolTable;
    UINT32   NumberOfSymbols;
    UINT16   SizeOfOptionalHeader;
    UINT16   Characteristics;
} IMAGE_FILE_HEADER, * PIMAGE_FILE_HEADER;

typedef struct _IMAGE_NT_HEADERS64 {
    UINT32 Signature;
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER64 OptionalHeader;
} IMAGE_NT_HEADERS64, * PIMAGE_NT_HEADERS64;

typedef struct _IMAGE_BASE_RELOCATION {
    UINT32   VirtualAddress;
    UINT32   SizeOfBlock;
    //  UINT16    TypeOffset[1];
} IMAGE_BASE_RELOCATION, * PIMAGE_BASE_RELOCATION;


#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04
#define PSF1_CHRHEIGHT 16
#define PSF1_CHRWIDTH 8

typedef PACK(struct PSF1_HEADER {
    unsigned char magic[2];
    unsigned char mode;
    unsigned char charsize;
}) PSF1_HEADER;

typedef PACK(struct PSF1_FONT {
    PSF1_HEADER* psf1_Header;
    void* glyphBuffer;
}) PSF1_FONT;

typedef PACK(struct Framebuffer {
    void* BaseAddress;
    UINTN BufferSize;
    unsigned int Width;
    unsigned int Height;
    unsigned int PixelsPerScanLine;
    PSF1_FONT* psf1font;
    unsigned int color;
}) Framebuffer;

typedef struct Point {
    unsigned int X;
    unsigned int Y;
} Point;

typedef PACK(struct SDTHeader {
    unsigned char Signature[4];
    uint32_t Length;
    uint8_t Revision;
    uint8_t Checksum;
    uint8_t OEMID[6];
    uint8_t OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision;
}) SDTHeader;

typedef PACK(struct MCFGHeader {
    SDTHeader Header;
    uint64_t Reserved;
}) MCFGHeader;

typedef PACK(struct RSDP {
    unsigned char Signature[8];
    uint8_t Checksum;
    uint8_t OEMId[6];
    uint8_t Revision;
    uint32_t RSDT;
    uint32_t Length;
    uint64_t XSDT;
    uint8_t ExtendedChecksum;
    uint8_t Reserved[3];
}) RSDP;

typedef PACK(struct BLOCKINFO
{
    Framebuffer*           frameBuffer;
    uint64_t               BaseAddress;
    uint64_t               BufferSize;
    uint32_t               ScreenWidth;
    uint32_t               ScreenHeight;
    uint32_t               PixelsPerScanLine;
    uint64_t               LoaderFileSize;
    EFI_MEMORY_DESCRIPTOR* MMap;
    uint64_t               MMapSize;
    uint32_t               MMapDescriptorVersion;
    uint64_t               MMapDescriptorSize;
    RSDP*                  RSDPTable;
    MCFGHeader*            MCFGHeader;
}) BLOCKINFO;

static BLOCKINFO bi;

#define IMAGE_SIZEOF_SECTION_HEADER          40
#define IMAGE_DIRECTORY_ENTRY_BASERELOC      5   // Base Relocation Table
#define IMAGE_SIZEOF_BASE_RELOCATION         8

uint32_t __strcmp(const char* a, const char* b, size_t length);

EFI_BOOT_SERVICES* Get_BootService(EFI_SYSTEM_TABLE* systemTable);
EFI_STATUS Get_MemoryMap(BLOCKINFO* bi, EFI_SYSTEM_TABLE* SystemTable, UINTN* mapKey);
void ClearScreen();
void __LoadFile(BLOCKINFO* bi, UINTN mapKey, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);
EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);
PSF1_FONT* LoadPSF1Font(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable);
EFI_STATUS efi_Get_RSDP_Table(BLOCKINFO* blockinfo, EFI_SYSTEM_TABLE* SystemTable);
typedef int (__attribute__((ms_abi))* EntryPointFunction)(BLOCKINFO*);

#endif
