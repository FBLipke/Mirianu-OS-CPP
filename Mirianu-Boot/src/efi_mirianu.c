#include <efi_mirianu.h>

// Constructor prototypes
typedef void(__cdecl* _PVFV)(void);
typedef int(__cdecl* _PIFV)(void);


// Linker puts constructors between these sections, and we use them to locate constructor pointers.
#pragma section(".CRT$XIA",long,read)
#pragma section(".CRT$XIZ",long,read)
#pragma section(".CRT$XCA",long,read)
#pragma section(".CRT$XCZ",long,read)

// Put .CRT data into .rdata section
#pragma comment(linker, "/merge:.CRT=.rdata")

// Pointers surrounding constructors
__declspec(allocate(".CRT$XIA")) _PIFV __xi_a[] = { 0 };
__declspec(allocate(".CRT$XIZ")) _PIFV __xi_z[] = { 0 };
__declspec(allocate(".CRT$XCA")) _PVFV __xc_a[] = { 0 };
__declspec(allocate(".CRT$XCZ")) _PVFV __xc_z[] = { 0 };

extern __declspec(allocate(".CRT$XIA")) _PIFV __xi_a[];
extern __declspec(allocate(".CRT$XIZ")) _PIFV __xi_z[];    // C initializers
extern __declspec(allocate(".CRT$XCA")) _PVFV __xc_a[];
extern __declspec(allocate(".CRT$XCZ")) _PVFV __xc_z[];    // C++ initializers

static int _initterm_e(_PIFV* pfbegin, _PIFV* pfend)
{
	int ret = 0;

	while (pfbegin < pfend && ret == 0)
	{
		if (*pfbegin != 0)
			ret = (**pfbegin)();

		++pfbegin;
	}

	return ret;
}


// Call C++ constructors
static void _initterm(_PVFV* pfbegin, _PVFV* pfend)
{
	while (pfbegin < pfend)
	{
		// if current table entry is non-NULL, call thru it.
		if (*pfbegin != 0)
			(**pfbegin)();
		++pfbegin;
	}
}

int CallConstructors()
{
	// Do C initialization
	int initret = _initterm_e(__xi_a, __xi_z);
	if (initret != 0) {
		return 0;
	}

	// Do C++ initialization
	_initterm(__xc_a, __xc_z);
	return 1;
}

EFI_BOOT_SERVICES* Get_BootService(EFI_SYSTEM_TABLE* systemTable)
{
	return systemTable->BootServices;
}


EFI_STATUS efi_get_RSDP_Table_New()
{
	const EFI_GUID RS;
}


/// <summary>
/// Returns the Address of the RSDP.
/// </summary>
/// <param name="blockinfo"></param>
/// <param name="SystemTable"></param>
/// <returns></returns>
EFI_STATUS efi_Get_RSDP_Table(BLOCKINFO* blockinfo, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_CONFIGURATION_TABLE* configTable = SystemTable->ConfigurationTable;
	RSDP* rsdp = NULL;
	EFI_GUID Acpi2TableGuid = ACPI_20_TABLE_GUID;

	int rsdp_found = 0;

	for (UINTN index = 0; index < SystemTable->NumberOfTableEntries; index++)
	{
		if (CompareGuid(&configTable[index].VendorGuid, &Acpi2TableGuid))
		{
			if (__strcmp("RSD PTR ", configTable->VendorTable, 8) == 1)
			{
				rsdp = (RSDP*)configTable->VendorTable;
				if (rsdp->Revision != 0)
					rsdp_found = 1;
			}
		}

		configTable++;
	}

	if (rsdp_found == 1)
	{
		blockinfo->RSDPTable = rsdp;
		return EFI_SUCCESS;
	}
	else
	{
		return EFI_ABORTED;
	}
}

void ClearScreen()
{
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL color = { 43, 45, 47,0 };
	gop->Blt(gop, &color, EfiBltVideoFill, 0, 0, 0, 0, gop->Mode->Info->HorizontalResolution, gop->Mode->Info->VerticalResolution, 0);
}

void __LoadFile(BLOCKINFO* bi, UINTN mapKey, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	UINT64 FileStartPosition = 0;

	EFI_FILE* _file = LoadFile(NULL, Path, ImageHandle, SystemTable);

	if (_file == NULL) {
		Print(L"[E] Could not load kernel!\n");
		return;
	}

	_file->GetPosition(_file, &FileStartPosition);
	UINTN FileInfoSize = 0;
	_file->GetInfo(_file, &gEfiFileInfoGuid, &FileInfoSize, NULL);

	EFI_FILE_INFO* FileInfo = NULL;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo);
	_file->GetInfo(_file, &gEfiFileInfoGuid, &FileInfoSize, FileInfo);

	UINTN size = sizeof(IMAGE_DOS_HEADER);
	IMAGE_DOS_HEADER DOSheader;

	_file->Read(_file, &size, &DOSheader);
	EFI_PHYSICAL_ADDRESS Header_memory = 0x400000;
	if (DOSheader.e_magic == IMAGE_DOS_SIGNATURE) // MZ
	{
		_file->SetPosition(_file, (UINT64)DOSheader.e_lfanew);
		size = sizeof(IMAGE_NT_HEADERS64);
		IMAGE_NT_HEADERS64 PEHeader;
		_file->Read(_file, &size, &PEHeader);
		if (PEHeader.Signature == IMAGE_NT_SIGNATURE)
		{
			if (PEHeader.FileHeader.Machine == IMAGE_FILE_MACHINE_X64 && PEHeader.OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
			{
				if (PEHeader.OptionalHeader.Subsystem != IMAGE_SUBSYSTEM_EFI_APPLICATION)
				{
					UINT64 i; // Iterator
					UINT64 virt_size = 0; // Size of all the data sections combined, which we need to know in order to allocate the right number of pages
					IMAGE_SECTION_HEADER section_headers_table[32]; // This table is an array of section headers
					size = IMAGE_SIZEOF_SECTION_HEADER * (UINT64)PEHeader.FileHeader.NumberOfSections; // Size of section header table in file... Hm...

					_file->Read(_file, &size, &section_headers_table[0]);

					for (i = 0; i < (UINT64)PEHeader.FileHeader.NumberOfSections; i++) // Go through each section of the "sections" section to get the address boundary of the last section
					{
						IMAGE_SECTION_HEADER* specific_section_header = &section_headers_table[i];
						virt_size = (virt_size > (UINT64)(specific_section_header->VirtualAddress + specific_section_header->Misc.VirtualSize) ? virt_size : (UINT64)(specific_section_header->VirtualAddress + specific_section_header->Misc.VirtualSize));
					}

					UINTN Header_size = (UINTN)PEHeader.OptionalHeader.SizeOfHeaders;
					UINT64 pages = (virt_size + EFI_PAGE_MASK) >> EFI_PAGE_SHIFT;
					EFI_PHYSICAL_ADDRESS AllocatedMemory = PEHeader.OptionalHeader.ImageBase;
					SystemTable->BootServices->AllocatePages(AllocateAnyPages, EfiLoaderData, pages, &AllocatedMemory);

					_file->SetPosition(_file, 0);
					_file->Read(_file, &Header_size, (EFI_PHYSICAL_ADDRESS*)AllocatedMemory);

					for (i = 0; i < (UINT64)PEHeader.FileHeader.NumberOfSections; i++) // Load sections into memory
					{
						IMAGE_SECTION_HEADER* specific_section_header = &section_headers_table[i];
						UINTN RawDataSize = (UINTN)specific_section_header->SizeOfRawData;
						EFI_PHYSICAL_ADDRESS SectionAddress = AllocatedMemory + (UINT64)specific_section_header->VirtualAddress;


						_file->SetPosition(_file, (UINT64)specific_section_header->PointerToRawData);
						if (RawDataSize != 0) // Apparently some UEFI implementations can't deal with reading 0 byte sections
						{

							_file->Read(_file, &RawDataSize, (EFI_PHYSICAL_ADDRESS*)SectionAddress); // (void*)SectionAddress
						}
					}

					if (AllocatedMemory != PEHeader.OptionalHeader.ImageBase && PEHeader.OptionalHeader.NumberOfRvaAndSizes > IMAGE_DIRECTORY_ENTRY_BASERELOC) // Need to perform relocations
					{
						IMAGE_BASE_RELOCATION* Relocation_Directory_Base = (IMAGE_BASE_RELOCATION*)(AllocatedMemory + (UINT64)PEHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
						IMAGE_BASE_RELOCATION* RelocTableEnd = (IMAGE_BASE_RELOCATION*)(AllocatedMemory + (UINT64)PEHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size + (UINT64)PEHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

						// Determine by how much we are off the executable's intended ImageBase
						UINT64 delta = 0;
						if (AllocatedMemory > PEHeader.OptionalHeader.ImageBase)
						{
							delta = AllocatedMemory - PEHeader.OptionalHeader.ImageBase; // Determine by how much we are off the executable's intended ImageBase
						}
						else
						{
							delta = PEHeader.OptionalHeader.ImageBase - AllocatedMemory;
						}
						UINT64 NumRelocationsPerChunk = 0;

						for (; (Relocation_Directory_Base->SizeOfBlock) && (Relocation_Directory_Base < RelocTableEnd);)
						{
							EFI_PHYSICAL_ADDRESS page = AllocatedMemory + (UINT64)Relocation_Directory_Base->VirtualAddress; //This virtual address is page-specific, and needs to be offset by Header_memory
							UINT16* DataToFix = (UINT16*)((UINT8*)Relocation_Directory_Base + IMAGE_SIZEOF_BASE_RELOCATION); // The base relocation size is 8 bytes (64 bits)
							NumRelocationsPerChunk = (Relocation_Directory_Base->SizeOfBlock - IMAGE_SIZEOF_BASE_RELOCATION) / sizeof(UINT16);

							for (i = 0; i < NumRelocationsPerChunk; i++)
							{
								if (DataToFix[i] >> EFI_PAGE_SHIFT == 0)
								{
									// Nothing, this is a padding area
								}
								else if (DataToFix[i] >> EFI_PAGE_SHIFT == 10) // 64-bit offset relocation only, check uppper 4 bits of each DataToFix entry
								{
									if (AllocatedMemory > PEHeader.OptionalHeader.ImageBase)
									{
										*((UINT64*)((UINT8*)page + (DataToFix[i] & EFI_PAGE_MASK))) += delta;
									}
									else
									{
										*((UINT64*)((UINT8*)page + (DataToFix[i] & EFI_PAGE_MASK))) -= delta;
									}
								}
								else
								{
									Print(L"[E] Something happened whilst relocating. i: %llu, Relocation_Directory_Base: 0x%llx \r\n", i, Relocation_Directory_Base);
								}
							}

							Relocation_Directory_Base = (IMAGE_BASE_RELOCATION*)((UINT8*)Relocation_Directory_Base + Relocation_Directory_Base->SizeOfBlock);

						}
					}

					Header_memory = AllocatedMemory + (UINT64)PEHeader.OptionalHeader.AddressOfEntryPoint;
				}
			}
		}
	}


	EntryPointFunction EntryPointPlaceholder = (EntryPointFunction)(Header_memory);

	Get_BootService(SystemTable)->SetWatchdogTimer(0, 0, 0, NULL);
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
	Get_BootService(SystemTable)->ExitBootServices(ImageHandle, mapKey);
	CallConstructors();
	Print(L"%d\n", EntryPointPlaceholder(bi));

}

EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_FILE* LoadedFile;

	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage = NULL;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImage);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem = NULL;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&FileSystem);

	if (Directory == NULL) {
		FileSystem->OpenVolume(FileSystem, &Directory);
	}

	EFI_STATUS s = Directory->Open(Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if (s != EFI_SUCCESS) {
		return NULL;
	}

	return LoadedFile;

}

EFI_STATUS Get_MemoryMap(BLOCKINFO* bi, EFI_SYSTEM_TABLE* SystemTable, UINTN* mapKey)
{
	SystemTable->BootServices->GetMemoryMap(&bi->MMapSize, bi->MMap, mapKey, &bi->MMapDescriptorSize, &bi->MMapDescriptorVersion);
	SystemTable->BootServices->AllocatePool(EfiLoaderData, bi->MMapSize, (void**)&bi->MMap);
	SystemTable->BootServices->GetMemoryMap(&bi->MMapSize, bi->MMap, mapKey, &bi->MMapDescriptorSize, &bi->MMapDescriptorVersion);

	if (bi->MMap == NULL) { return EFI_ABORTED; }

	return EFI_SUCCESS;
}

PSF1_FONT* LoadPSF1Font(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	EFI_FILE* font = LoadFile(Directory, Path, ImageHandle, SystemTable);
	if (font == NULL) return NULL;

	PSF1_HEADER* fontHeader = NULL;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_HEADER), (void**)&fontHeader);
	UINTN size = sizeof(PSF1_HEADER);
	font->Read(font, &size, fontHeader);

	if (fontHeader->magic[0] != PSF1_MAGIC0 || fontHeader->magic[1] != PSF1_MAGIC1) {
		return NULL;
	}

	UINTN glyphBufferSize = fontHeader->charsize * 256;
	if (fontHeader->mode == 1) { //512 glyph mode
		glyphBufferSize = fontHeader->charsize * 512;
	}

	void* glyphBuffer;
	{
		font->SetPosition(font, sizeof(PSF1_HEADER));
		SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&glyphBuffer);
		font->Read(font, &glyphBufferSize, glyphBuffer);
	}

	PSF1_FONT* finishedFont = NULL;
	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_FONT), (void**)&finishedFont);
	finishedFont->psf1_Header = fontHeader;
	finishedFont->glyphBuffer = glyphBuffer;
	return finishedFont;

}

uint32_t __strcmp(const char* a, const char* b, size_t length)
{
	for (size_t i = 0; i < length; i++) {

		if (a[i] != b[i])
			return 0;
	}

	return 1;
}
