#include "stdafx.h"
#include "BootSetup.h"
#include "Global.h"
#include "MainGUI.h"
#include <shellapi.h>

int BootSetup::GetSystemFirmwareType()
{
	gr7::ModifyPrivilege(SE_SYSTEM_ENVIRONMENT_NAME, TRUE, GetCurrentProcess());

	if (GetFirmwareEnvironmentVariableW(_T(""), _T("{00000000-0000-0000-0000-000000000000}"), NULL, 0) == 0) {
		if (GetLastError() == ERROR_INVALID_FUNCTION) {
			return 1; // BIOS
		}
		else if (GetLastError() == ERROR_NOACCESS) {
			return 2; // UEFI
		}
	}
	return 0; // ERROR
}

void BootSetup::SetupSystemBoot()
{
	wchar_t WindowsFolder[MAX_PATH];

	wcsncpy_s(WindowsFolder, ImageInstallObjects.destDrive, sizeof(WindowsFolder));
#pragma warning(suppress : 4129)
	wcsncat_s(WindowsFolder, L"\Windows", sizeof(WindowsFolder));

	SHELLEXECUTEINFO ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = L"open";
	ShExecInfo.lpFile = L"bcdboot";
	ShExecInfo.lpParameters = WindowsFolder;
	ShExecInfo.lpDirectory = ImageInstallObjects.installSources;

#ifdef _DEBUG
	ShExecInfo.nShow = SW_SHOW;
#else
	ShExecInfo.nShow = SW_HIDE;
#endif

	ShExecInfo.hInstApp = NULL;
	ShellExecuteExW(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	CloseHandle(ShExecInfo.hProcess);
}