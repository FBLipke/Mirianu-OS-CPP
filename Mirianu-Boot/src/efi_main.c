#include <efi_mirianu.h>

extern int _fltused = 0;


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

	if (EFI_ERROR(status) == EFI_ABORTED)
	{
		Print(L"[E] Failed to get RSDP...\n");
		return EFI_ABORTED;
	}

	Print(L"Loading Kernel...\n");
	
	__LoadFile(&bi, mapKey, L"Mirianu-Kernel.bin", ImageHandle, SystemTable);

	Print(L"[E] Kernel Closed!\n");

	while (1) { ___Halt(); }

	return EFI_SUCCESS;
}
