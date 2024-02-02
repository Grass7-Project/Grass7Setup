#include "stdafx.h"
#include "PartitionCode.h"
#include "Global.h"
#include "GUI.h"
#include "BootSetup.h"
#include <shellapi.h>

void PartitionCode::AutomaticPartitioning()
{
	int FirmwareType = BootSetup::GetSystemFirmwareType();
	wchar_t diskpartCMD[MAX_PATH];
	wcsncpy_s(diskpartCMD, L"diskpart /s ", sizeof(diskpartCMD));
	wcsncat_s(diskpartCMD, ImageInstallObjects.installSources, sizeof(diskpartCMD));
	if (FirmwareType == 1) {
		wcsncat_s(diskpartCMD, L"\\BIOS.txt", sizeof(diskpartCMD));
	}
	if (FirmwareType == 2) {
		wcsncat_s(diskpartCMD, L"\\UEFI.txt", sizeof(diskpartCMD));
	}
	ShellExecuteW(NULL, NULL, L"cmd", diskpartCMD, NULL, SW_SHOW);
}

void PartitionCode::ManualPartitioning()
{
	ShellExecuteW(NULL, NULL, L"cmd", L"diskpart", NULL, SW_SHOW);
}
