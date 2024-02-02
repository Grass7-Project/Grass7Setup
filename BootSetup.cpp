#include "stdafx.h"
#include "BootSetup.h"
#include "Global.h"
#include "GUI.h"
#include <shellapi.h>

int BootSetup::GetSystemFirmwareType()
{
	gr7::ModifyPrivilege(SE_SYSTEM_ENVIRONMENT_NAME, TRUE, GetCurrentProcess());

	if (GetFirmwareEnvironmentVariable(_T(""), _T("{00000000-0000-0000-0000-000000000000}"), NULL, 0) == 0) {
		if (GetLastError() == ERROR_INVALID_FUNCTION) {
			return 1; // BIOS
		}
		else if (GetLastError() == ERROR_NOACCESS) {
			return 2; // UEFI
		}
	}
	return 0;
}

void BootSetup::SetupSystemBoot()
{
	int FirmwareType = BootSetup::GetSystemFirmwareType();
	wchar_t SetBootCMD[MAX_PATH];
	wcsncpy_s(SetBootCMD, ImageInstallObjects.installSources, sizeof(SetBootCMD));
	wcsncat_s(SetBootCMD, L"\\BootFiles.bat", sizeof(SetBootCMD));
	if (FirmwareType == 1) {
		wcsncat_s(SetBootCMD, L" BIOS ", sizeof(SetBootCMD));
	}
	if (FirmwareType == 2) {
		wcsncat_s(SetBootCMD, L" UEFI ", sizeof(SetBootCMD));
	}
	wcsncat_s(SetBootCMD, ImageInstallObjects.destDrive, sizeof(SetBootCMD));
	ShellExecuteW(NULL, NULL, L"cmd", SetBootCMD, NULL, SW_SHOW);
}