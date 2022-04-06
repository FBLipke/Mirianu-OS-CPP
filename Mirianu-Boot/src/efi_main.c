#include <efi_mirianu.h>

extern int _fltused = 0;

void* FindTable(SDTHeader* sdtHeader, char* signature) {

	int entries = (sdtHeader->Length - sizeof(SDTHeader)) / 8;

	for (int t = 0; t < entries; t++) {
		SDTHeader* newSDTHeader = (SDTHeader*)*(uint64_t*)((uint64_t)sdtHeader + sizeof(SDTHeader) + (t * 8));
		for (int i = 0; i < 4; i++)
		{
			if (newSDTHeader->Signature[i] != signature[i])
				break;

			if (t == 3)
				return newSDTHeader;
		}
	}

	Print(L"[E] Could not find SDT Table!\n");

	return NULL;
}


EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
#ifdef _GNU_EFI
	InitializeLib(ImageHandle, SystemTable);
#endif
	UINTN mapKey = 0;
	EFI_STATUS status;

	Get_BootService(SystemTable)->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID**)&gop);

	bi.frameBuffer->BaseAddress = (void*)gop->Mode->FrameBufferBase;
	bi.frameBuffer->BufferSize = gop->Mode->FrameBufferSize;
	bi.frameBuffer->Width = gop->Mode->Info->HorizontalResolution;
	bi.frameBuffer->Height = gop->Mode->Info->VerticalResolution;
	bi.frameBuffer->PixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;

	bi.frameBuffer->psf1font = LoadPSF1Font(NULL, L"zap-light16.psf", ImageHandle, SystemTable);
	bi.frameBuffer->color = 0xffffffff;

	bi.RSDPTable = NULL;
	bi.MCFGHeader = NULL;

	if (bi.frameBuffer->psf1font == NULL) {
		Print(L"[E] Font is not valid or was not found\n");
		return EFI_ABORTED;
	}

	status = Get_MemoryMap(&bi, SystemTable, &mapKey);
	if (EFI_ERROR(status) == EFI_ABORTED)
	{
		Print(L"[E] Failed to get MemoryMap...\n");
		return EFI_ABORTED;
	}

	status = efi_Get_RSDP_Table(&bi, SystemTable);

	if (bi.RSDPTable == NULL)
	{
		Print(L"[E] Failed to get RSDP...\n");
		return EFI_ABORTED;
	}

	if (bi.RSDPTable->Revision == 2)
	{
		Print(L"[I] Got ACPI 2.0 Table\n");
		bi.MCFGHeader = (MCFGHeader*)FindTable((SDTHeader*)bi.RSDPTable->XSDT, (char*)"MCFG");
	}

	__LoadFile(&bi, mapKey, L"Mirianu-Kernel.bin", ImageHandle, SystemTable);

	Print(L"[BOOTX64] Kernel Closed!\n");

	while (1) { ___Halt(); }

	return EFI_SUCCESS;
}
