#include "stdafx.h"
#include "PartitionCode.h"
#include "MainGUI.h"
#include "BootSetup.h"
#include <shellapi.h>

void PartitionCode::AutomaticPartitioning()
{
	int FirmwareType = BootSetup::GetSystemFirmwareType();

	std::wstring diskpartCMD;
	diskpartCMD.append(L"/C diskpart /s ");
	diskpartCMD.append(ImageInstallObjects.installSources);

	if (FirmwareType == 1) {
		diskpartCMD.append(L"\\BIOS.txt");
	}
	if (FirmwareType == 2) {
		diskpartCMD.append(L"\\UEFI.txt");
	}

	SHELLEXECUTEINFO ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = L"open";
	ShExecInfo.lpFile = L"cmd";
	ShExecInfo.lpParameters = diskpartCMD.c_str();
	ShExecInfo.lpDirectory = ImageInstallObjects.installSources.c_str();
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
