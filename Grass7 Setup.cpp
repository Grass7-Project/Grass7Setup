#include "stdafx.h"
#include "Grass7 Setup.h"
#include "GUI.h"
#include "Global.h"
#include "ProgressBar.h"
#include <iostream>

Global GlobalObjects;

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

	GlobalObjects.hInst = hInstance;

	WCHAR installSourcesp[MAX_PATH];
	GetModuleFileNameW(NULL, installSourcesp, MAX_PATH);
	PathRemoveFileSpecW(installSourcesp);
	GlobalObjects.installSources = installSourcesp;

	LoadStringW(GlobalObjects.hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	GUI::RegisterClasses();

	if (!GUI::InitInstance()) {
		return FALSE;
	}

	hAccelTable = LoadAcceleratorsW(GlobalObjects.hInst, L"MainWindow");

	while (GetMessageW(&msg, NULL, 0, 0)) {
		if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return (int) msg.wParam;
}