#include "stdafx.h"
#include "MainInit.h"
#include "MainGUI.h"
#include "SetupGUI.h"
#include "DialogGUI.h"
#include "ButtonGUI.h"
#include "ResourceLoader.h"
#include <shellapi.h>
#include <versionhelpers.h>

int MainInit::Init(MSG &msg, HINSTANCE &hInstance, HINSTANCE &hPrevInstance, LPTSTR &lpCmdLine, int &nCmdShow)
{
	HACCEL hAccelTable;
	OSVERSIONINFO VersionInfo;

	VersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	MainObjects.hInst = hInstance;
	MainObjects.hProcess = GetCurrentProcess();
	ResourceLoader::LoadStrings();

	if (lpCmdLine != L"") {
		LPWSTR *szArglist;
		int nArgs;

		szArglist = CommandLineToArgvW(lpCmdLine, &nArgs);
		LocalFree(szArglist);

		// 3 Arguments avaliable
		if (nArgs > 3) {
			ErrorHandler::InvokeErrorHandler(1, 0, L"Too many command line arguments.", AppResStringsObjects.AppTitleText);
		}
	}

	if (lpCmdLine != NULL) {
		if (wcsstr(lpCmdLine, L"/nodeploy") != 0) {
			ImageInstallObjects.NoDeploy = TRUE;
		}
		if (wcsstr(lpCmdLine, L"/debug") != 0) {
			MainObjects.Debug = TRUE;
		}
		if (wcsstr(lpCmdLine, L"/disablewinpecheck") != 0) {
			MainObjects.NoWinPECheck = TRUE;
		}
	}

	if (!IsWindowsXPSP2OrGreater())
	{
		ErrorHandler::InvokeErrorHandler(1, 0, L"Setup needs at least Windows XP or above to run properly.", AppResStringsObjects.AppTitleText);
	}

	if (!MainObjects.NoWinPECheck) {
		DWORD dwValue;
		DWORD dwSize = sizeof(dwValue);
		RegGetValueW(HKEY_LOCAL_MACHINE, L"SYSTEM\\Setup", L"SystemSetupInProgress", RRF_RT_DWORD, NULL, (LPBYTE)&dwValue, &dwSize);

		if (dwValue == 0) {
			ErrorHandler::InvokeErrorHandler(1, 0, L"Setup is required to run in WinPE.", AppResStringsObjects.AppTitleText);
		}
	}

	MainObjects.dwmEnabled = 0;
	HMODULE hMod = LoadLibraryExW(L"dwmapi.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hMod)
	{
		HRESULT(WINAPI* pfnDwmIsCompositionEnabled)(BOOL *pfEnabled);

		(FARPROC&)pfnDwmIsCompositionEnabled = GetProcAddress(hMod, "DwmIsCompositionEnabled");

		if (pfnDwmIsCompositionEnabled)
		{
			pfnDwmIsCompositionEnabled(&MainObjects.dwmEnabled);
		}
		FreeLibrary(hMod);
	}

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

	if (MainObjects.Debug) {
		ImageInstallObjects.destDrive = L"C:\\Users\\Genki\\Desktop\\gr7\\Deploy";
	}
	else {
		ImageInstallObjects.destDrive = L"W:\\";
	}

	if (!MainInit::InitGUI()) {
		return FALSE;
	}

	hAccelTable = LoadAcceleratorsW(MainObjects.hInst, L"MainWindow");

	while (GetMessageW(&msg, NULL, 0, 0)) {
		if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return TRUE;
}

// GUI Init Code
BOOL MainInit::InitGUI()
{
	// Call the function to load the bitmaps
	ResourceLoader::LoadBitmaps();

	// Create Main Window
	MainGUI::Init(MainObjects.hWndMainWindow, MainObjects.hInst);

	// Create Setup Window
	SetupGUI::Init(MainObjects.hWndSetupWindow, MainObjects.hWndMainWindow, MainObjects.hInst);

	// Create Dialog Window
	DialogGUI::Init();

	::ShowWindow(MainObjects.hWndSetupWindow, TRUE); // Show the Setup Window
	::UpdateWindow(MainObjects.hWndSetupWindow); // Update the Setup Window to trigger a redraw

	::ShowWindow(MainObjects.hWndMainWindow, TRUE); // Show the Main Window
	::UpdateWindow(MainObjects.hWndMainWindow); // Update the Main Window to trigger a redraw

	// We call the functions to init the custom buttons
	ButtonGUI::InitBackBtn();
	ButtonGUI::InitCloseBtn();
	ButtonGUI::InitNormalBtn();
	ButtonGUI::InitAutoPartitionButton();
	ButtonGUI::InitManualPartitionButton();

	MainObjects.Page = 1;
	::SendMessageW(MainObjects.hWndSetupWindow, SETUPWND_UPDATE_DIALOG, (WPARAM)(INT)0, 0);

	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_CREATE_PROG_BAR, (WPARAM)(INT)0, 0);
	return 1;
}

void MainInit::Exit()
{
	if (MainObjects.doNotClose) {
		MessageBoxW(NULL, AppResStringsObjects.SetupExitDuringSetup.c_str(), AppResStringsObjects.AppTitleText.c_str(), MB_ICONERROR | MB_OK);
	}

	if (!MainObjects.doNotClose) {
		const int result = MessageBoxW(NULL, AppResStringsObjects.SetupExit.c_str(), AppResStringsObjects.AppTitleText.c_str(), MB_ICONERROR | MB_YESNO);
		switch (result)
		{
		case IDYES:
			SendMessageW(MainObjects.hWndMainWindow, WM_CLOSE, (WPARAM)(INT)0, 0);
			break;
		}
	}
}