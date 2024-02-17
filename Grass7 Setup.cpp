#include "stdafx.h"
#include "Grass7 Setup.h"
#include "MainGUI.h"
#include "ResourceLoader.h"
#include <iostream>

GlobalMain MainObjects;
GlobalButtons ButtonObjects;
GlobalRichEditControl RichEditControlObjects;
GlobalProgressBar ProgressBarObjects;
GlobalProgressTextPercentage ProgressTextPercentageObjects;
GlobalAppResStrings AppResStringsObjects;
GlobalImageInstall ImageInstallObjects;
GlobalBitmaps BitmapObjects;

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

	MainObjects.hInst = hInstance;
	MainObjects.hProcess = GetCurrentProcess();
	ResourceLoader::LoadStrings();

    #pragma warning(suppress : 4996)
	if (GetVersionExW(&VersionInfo) != 0) {
		if (VersionInfo.dwMajorVersion < WINDOWS_XP_MAJORVERSION) {
			ErrorHandler::InvokeErrorHandler(1, 0, L"Setup needs at least Windows XP or above to run properly.", AppResStringsObjects.AppTitleText);
		}
	}

#ifdef _DEBUG
#else
	DWORD dwValue;
	DWORD dwSize = sizeof(dwValue);
	RegGetValueW(HKEY_LOCAL_MACHINE, L"SYSTEM\\Setup", L"SystemSetupInProgress", RRF_RT_DWORD, NULL, (LPBYTE)&dwValue, &dwSize);

	if (dwValue == 0) {
		ErrorHandler::InvokeErrorHandler(1, 0, L"Setup is required to be run in WinPE.", AppResStringsObjects.AppTitleText);
	}
#endif

	std::wstring installSourcesp1(MAX_PATH, 0);

	// Basic application strings
	installSourcesp1.resize((size_t)GetModuleFileNameW(NULL, &installSourcesp1[0], (int)installSourcesp1.size()));
	PathRemoveFileSpecW(&installSourcesp1[0]);

	ImageInstallObjects.installSources = &installSourcesp1[0];

	std::wstring srcImageESD = ImageInstallObjects.installSources;
	std::wstring srcImageWIM = ImageInstallObjects.installSources;

	srcImageESD.append(L"\\install.esd");
	srcImageWIM.append(L"\\install.wim");

	if (gr7::fileExistsW(const_cast<wchar_t*>(srcImageESD.c_str())) == TRUE) {
		ImageInstallObjects.ImagePath = srcImageESD;
	}
	if (gr7::fileExistsW(const_cast<wchar_t*>(srcImageWIM.c_str())) == TRUE) {
		ImageInstallObjects.ImagePath = srcImageWIM;
	}
	if (gr7::fileExistsW(const_cast<wchar_t*>(srcImageESD.c_str())) + gr7::fileExistsW(const_cast<wchar_t*>(srcImageWIM.c_str())) == FALSE) {
		ErrorHandler::InvokeErrorHandler(1, 0, L"Setup did not detect any installation image file.", AppResStringsObjects.AppTitleText);
	}

	ImageInstallObjects.ImageIndex = 1;

#ifdef _DEBUG
	ImageInstallObjects.destDrive = L"C:\\Users\\Genki\\Desktop\\gr7\\Deploy";
#else
	ImageInstallObjects.destDrive = L"W:\\";
#endif

	if (!MainGUI::InitInstance()) {
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