#include "stdafx.h"
#include "Grass7 Setup.h"
#include "GUI.h"
#include "Global.h"
#include "ProgressBar.h"
#include <iostream>

GlobalMain MainObjects;
GlobalButtons ButtonObjects;
GlobalRichEditControl RichEditControlObjects;
GlobalProgressBar ProgressBarObjects;
GlobalProgressTextPercentage ProgressTextPercentageObjects;
GlobalAppResStrings AppResStringsObjects;
GlobalImageInstall ImageInstallObjects;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	VersionInfo.dwOSVersionInfoSize	= sizeof(OSVERSIONINFO);

    #pragma warning(suppress : 4996)
	if (GetVersionExW(&VersionInfo) != 0) {
		if (VersionInfo.dwMajorVersion < WINDOWS_XP_MAJORVERSION) {
			MessageBoxW(NULL,
				L"Setup needs at least Windows XP or above to run properly.",
				szTitle,MB_ICONERROR | MB_OK);

			return 0;
		}
	}

#ifdef _DEBUG
	
#else
	DWORD dwValue;
	DWORD dwSize = sizeof(dwValue);
	RegGetValueW(HKEY_LOCAL_MACHINE, L"SYSTEM\\Setup", L"SystemSetupInProgress", RRF_RT_DWORD, NULL, (LPBYTE)&dwValue, &dwSize);

	if (dwValue == 0) {
		MessageBoxW(NULL,
			L"Setup is required to be run in WinPE",
			szTitle, MB_ICONERROR | MB_OK);
		return 0;
	}
#endif

	MainObjects.hInst = hInstance;

	GUI::LoadStrings();

	WCHAR installSourcesp[MAX_PATH];
	GetModuleFileNameW(NULL, installSourcesp, MAX_PATH);
	PathRemoveFileSpecW(installSourcesp);
	ImageInstallObjects.installSources = installSourcesp;

	LoadStringW(MainObjects.hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	GUI::RegisterClasses();

	if (!GUI::InitInstance()) {
		return FALSE;
	}

	hAccelTable = LoadAcceleratorsW(MainObjects.hInst, L"MainWindow");

	while (GetMessageW(&msg, NULL, 0, 0)) {
		if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return (int) msg.wParam;
}