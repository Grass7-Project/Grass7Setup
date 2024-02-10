#include "stdafx.h"
#include "Grass7 Setup.h"
#include "MainGUI.h"
#include "Global.h"
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
	ResourceLoader::LoadStrings();

    #pragma warning(suppress : 4996)
	if (GetVersionExW(&VersionInfo) != 0) {
		if (VersionInfo.dwMajorVersion < WINDOWS_XP_MAJORVERSION) {
			MessageBoxW(NULL,
				AppResStringsObjects.CompatibilityErrorText.c_str(),
				AppResStringsObjects.AppTitleText.c_str(),MB_ICONERROR | MB_OK);

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
			AppResStringsObjects.RunInWinPERequiredErrorText,
			AppResStringsObjects.AppTitleText, MB_ICONERROR | MB_OK);
		return 0;
	}
#endif

	WCHAR installSourcesp[MAX_PATH];
	GetModuleFileNameW(NULL, installSourcesp, MAX_PATH);
	PathRemoveFileSpecW(installSourcesp);
	ImageInstallObjects.installSources = installSourcesp;

	std::wstring srcImageESD = ImageInstallObjects.installSources;
	std::wstring srcImageWIM = ImageInstallObjects.installSources;

	srcImageESD.append(L"\\install.esd");
	srcImageWIM.append(L"\\install.wim");

	if (gr7::fileExists(const_cast<wchar_t*>(srcImageESD.c_str())) == TRUE) {
		ImageInstallObjects.ImagePath = srcImageESD;
	}
	if (gr7::fileExists(const_cast<wchar_t*>(srcImageWIM.c_str())) == TRUE) {
		ImageInstallObjects.ImagePath = srcImageWIM;
	}
	if (gr7::fileExists(const_cast<wchar_t*>(srcImageESD.c_str())) + gr7::fileExists(const_cast<wchar_t*>(srcImageWIM.c_str())) == FALSE) {
		MessageBoxW(NULL,
			AppResStringsObjects.NoInstallImageFoundErrorText.c_str(),
			AppResStringsObjects.AppTitleText.c_str(), MB_ICONERROR | MB_OK);
		return 0;
	}

	ImageInstallObjects.ImageIndex = 1;

#ifdef _DEBUG
	ImageInstallObjects.destDrive = L"C:\\Users\\Genki\\Desktop\\gr7\\Deploy";
#else
	ImageInstallObjects.destDrive = L"W:\\";
#endif

	MainGUI::RegisterClasses();

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