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
	wcsncpy_s(diskpartCMD, L"/C diskpart /s ", sizeof(diskpartCMD));
	wcsncat_s(diskpartCMD, ImageInstallObjects.installSources, sizeof(diskpartCMD));
	if (FirmwareType == 1) {
		wcsncat_s(diskpartCMD, L"\\BIOS.txt", sizeof(diskpartCMD));
	}
	if (FirmwareType == 2) {
		wcsncat_s(diskpartCMD, L"\\UEFI.txt", sizeof(diskpartCMD));
	}

	SHELLEXECUTEINFO ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = L"open";
	ShExecInfo.lpFile = L"cmd";
	ShExecInfo.lpParameters = diskpartCMD;
	ShExecInfo.lpDirectory = ImageInstallObjects.installSources;
	ShExecInfo.nShow = SW_SHOW;

	ShExecInfo.hInstApp = NULL;
	ShellExecuteExW(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	CloseHandle(ShExecInfo.hProcess);
}

void PartitionCode::ManualPartitioning()
{
	ShellExecuteW(NULL, NULL, L"cmd", L"/C diskpart", NULL, SW_SHOW);
}
