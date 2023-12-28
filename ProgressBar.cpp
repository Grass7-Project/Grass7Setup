#include "stdafx.h"
#include "ProgressBar.h"
#include "GUI.h"
#include "Global.h"
#include <Uxtheme.h>

void ProgressBar::createProgressBar()
{
	int hoz, ver;

	GUI::GetDesktopResolution(hoz, ver);

	GlobalObjects.hProgressCtrlInstalling = ::CreateWindowExW(
		0,
		PROGRESS_CLASS,
		L"",
		WS_CHILD | WS_VISIBLE | PBS_SMOOTH | WS_BORDER,
		210,
		ver - 55,
		hoz - 210,
		11,
		GlobalObjects.hWndMainWindow,
		(HMENU)ID_SMOOTHPROGRESSCTRL,
		GlobalObjects.hInst,
		NULL);

	GlobalObjects.hProgressCtrlCollectingInfo = ::CreateWindowExW(
		0,
		PROGRESS_CLASS,
		L"",
		WS_CHILD | WS_VISIBLE | PBS_SMOOTH | WS_BORDER,
		0,
		ver - 55,
		208,
		11,
		GlobalObjects.hWndMainWindow,
		(HMENU)ID_SMOOTHPROGRESSCTRL,
		GlobalObjects.hInst,
		NULL);

	::SendMessageW(GlobalObjects.hProgressCtrlCollectingInfo, PBM_SETPOS, (WPARAM)(INT)0, 0);
	SetWindowTheme(GlobalObjects.hProgressCtrlCollectingInfo, _T(""), _T(""));
	::SendMessageW(GlobalObjects.hProgressCtrlCollectingInfo, PBM_SETBARCOLOR, 0, 0x0033cc99);
	::SendMessageW(GlobalObjects.hProgressCtrlCollectingInfo, PBM_SETBKCOLOR, 0, 0x00b1937a);

	::SendMessageW(GlobalObjects.hProgressCtrlInstalling, PBM_SETPOS, (WPARAM)(INT)0, 0);
	SetWindowTheme(GlobalObjects.hProgressCtrlInstalling, _T(""), _T(""));
	::SendMessageW(GlobalObjects.hProgressCtrlInstalling, PBM_SETBARCOLOR, 0, 0x0033cc99);
	::SendMessageW(GlobalObjects.hProgressCtrlInstalling, PBM_SETBKCOLOR, 0, 0x00b1937a);

	GlobalObjects.CollectingInfoPercentage = 0;
	GlobalObjects.InstallingPercentage = 0;
	::SendMessageW(GlobalObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);

	ProgressBar::paintTextBelowProgressBar(GlobalObjects.hWndMainWindow, 37, ver - 40, gr7::LoadStringToW(GetModuleHandleW(NULL), IDS_PROGBAR_TEXT1), 18);
	ProgressBar::paintTextBelowProgressBar(GlobalObjects.hWndMainWindow, 228, ver - 40, gr7::LoadStringToW(GetModuleHandleW(NULL), IDS_PROGBAR_TEXT2), 18);
	ProgressBar::paintTextBelowProgressBar(GlobalObjects.hWndMainWindow, 11, ver - 40, L"1", 34);
	ProgressBar::paintTextBelowProgressBar(GlobalObjects.hWndMainWindow, 201, ver - 40, L"2", 34);
}

void ProgressBar::updateProgressBar()
{
	::SendMessageW(GlobalObjects.hProgressCtrlCollectingInfo, PBM_SETPOS, (WPARAM)(INT)GlobalObjects.CollectingInfoPercentage, 0);
	::SendMessageW(GlobalObjects.hProgressCtrlInstalling, PBM_SETPOS, (WPARAM)(INT)GlobalObjects.InstallingPercentage, 0);
	::UpdateWindow(GlobalObjects.hWndMainWindow);
}

void ProgressBar::createProgressText()
{
	// Init the percentage values
	GlobalObjects.CopyingFilesPercentage = 0;
	GlobalObjects.ExpandingFilesPercentage = 0;
	GlobalObjects.InstallingFeaturesPercentage = 0;
	GlobalObjects.InstallingUpdatesPercentage = 0;

	// Create the text
	updateProgressText(63, 123, GlobalObjects.CopyingFilesPercentage, gr7::LoadStringToW(GlobalObjects.hInst, IDS_COPYING_FILES));
	updateProgressText(63, 143, GlobalObjects.ExpandingFilesPercentage, gr7::LoadStringToW(GlobalObjects.hInst, IDS_EXPANDING_FILES));
	updateProgressText(63, 163, GlobalObjects.InstallingFeaturesPercentage, gr7::LoadStringToW(GlobalObjects.hInst, IDS_INSTALLING_FEATURES));
	updateProgressText(63, 183, GlobalObjects.InstallingUpdatesPercentage, gr7::LoadStringToW(GlobalObjects.hInst, IDS_INSTALLING_UPDATES));
}

void ProgressBar::updateProgressText(int x, int y, int &ProgressPercantage, wchar_t *Text)
{
	TCHAR percentage[5] = { 0 };
	TCHAR ProgressText[30] = { 0 };

	swprintf_s(percentage, L"%d", ProgressPercantage);
	wcsncat_s(percentage, L"%", 5);

	wcsncat_s(ProgressText, Text, 24);
	wcsncat_s(ProgressText, percentage, 256);

	HDC hdc = ::GetDC(GlobalObjects.hWndSetupWindow);
	HFONT hFont, hTmp;
	hFont = CreateFontW(18, 0, 0, 0, FW_LIGHT, 0, 0, 0, 0, 0, 0, 2, 0, L"Segoe UI");
	hTmp = (HFONT)SelectObject(hdc, hFont);
	size_t size = wcslen(ProgressText);
	int convertsize = static_cast<int>(size);
	TextOutW(hdc, x, y, ProgressText, convertsize);
	ReleaseDC(GlobalObjects.hWndSetupWindow, hdc);

	::UpdateWindow(GlobalObjects.hWndSetupWindow);
}

void ProgressBar::paintTextBelowProgressBar(HWND hWnd, int x, int y, wchar_t *Text, int FontSize)
{
	TCHAR ProgressText[30] = { 0 };
	wcsncat_s(ProgressText, Text, wcslen(Text));

	HDC hdc = ::GetDC(hWnd);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));
	HFONT hFont, hTmp;
	hFont = CreateFontW(FontSize, 0, 0, 0, FW_LIGHT, 0, 0, 0, 0, 0, 0, 2, 0, L"Segoe UI");
	hTmp = (HFONT)SelectObject(hdc, hFont);
	size_t size = wcslen(ProgressText);
	int convertsize = static_cast<int>(size);
	TextOutW(hdc, x, y, ProgressText, convertsize);
	ReleaseDC(hWnd, hdc);

	::UpdateWindow(hWnd);
}