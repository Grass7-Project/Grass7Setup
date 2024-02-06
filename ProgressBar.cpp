#include "stdafx.h"
#include "ProgressBar.h"
#include "GUI.h"
#include "Global.h"
#include <Uxtheme.h>

void ProgressBar::createProgressBar()
{
	int hoz, ver;

	GUI::GetDesktopResolution(hoz, ver);

	ProgressBarObjects.hProgressCtrlInstalling = ::CreateWindowExW(
		0,
		PROGRESS_CLASS,
		L"",
		WS_CHILD | WS_VISIBLE | PBS_SMOOTH | WS_BORDER,
		210,
		ver - 66,
		hoz - 210,
		11,
		MainObjects.hWndMainWindow,
		(HMENU)ID_SMOOTHPROGRESSCTRL,
		MainObjects.hInst,
		NULL);

	ProgressBarObjects.hProgressCtrlCollectingInfo = ::CreateWindowExW(
		0,
		PROGRESS_CLASS,
		L"",
		WS_CHILD | WS_VISIBLE | PBS_SMOOTH | WS_BORDER,
		0,
		ver - 66,
		208,
		11,
		MainObjects.hWndMainWindow,
		(HMENU)ID_SMOOTHPROGRESSCTRL,
		MainObjects.hInst,
		NULL);

	::SendMessageW(ProgressBarObjects.hProgressCtrlCollectingInfo, PBM_SETPOS, (WPARAM)(INT)0, 0);
	SetWindowTheme(ProgressBarObjects.hProgressCtrlCollectingInfo, _T(""), _T(""));
	::SendMessageW(ProgressBarObjects.hProgressCtrlCollectingInfo, PBM_SETBARCOLOR, 0, 0x0033cc99);
	::SendMessageW(ProgressBarObjects.hProgressCtrlCollectingInfo, PBM_SETBKCOLOR, 0, 0x00b1937a);

	::SendMessageW(ProgressBarObjects.hProgressCtrlInstalling, PBM_SETPOS, (WPARAM)(INT)0, 0);
	SetWindowTheme(ProgressBarObjects.hProgressCtrlInstalling, _T(""), _T(""));
	::SendMessageW(ProgressBarObjects.hProgressCtrlInstalling, PBM_SETBARCOLOR, 0, 0x0033cc99);
	::SendMessageW(ProgressBarObjects.hProgressCtrlInstalling, PBM_SETBKCOLOR, 0, 0x00b1937a);

	ProgressBarObjects.CollectingInfoPercentage = 0;
	ProgressBarObjects.InstallingPercentage = 0;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);

	ProgressBar::paintTextBelowProgressBar(MainObjects.hWndMainWindow, 37, ver - 42, AppResStringsObjects.ProgressBarText1, 9);
	ProgressBar::paintTextBelowProgressBar(MainObjects.hWndMainWindow, 228, ver - 42, AppResStringsObjects.ProgressBarText2, 9);
	ProgressBar::paintTextBelowProgressBar(MainObjects.hWndMainWindow, 9, ver - 47, L"1", 25);
	ProgressBar::paintTextBelowProgressBar(MainObjects.hWndMainWindow, 200, ver - 47, L"2", 25);
}

void ProgressBar::updateProgressBar()
{
	::SendMessageW(ProgressBarObjects.hProgressCtrlCollectingInfo, PBM_SETPOS, (WPARAM)(INT)ProgressBarObjects.CollectingInfoPercentage, 0);
	::SendMessageW(ProgressBarObjects.hProgressCtrlInstalling, PBM_SETPOS, (WPARAM)(INT)ProgressBarObjects.InstallingPercentage, 0);
	::UpdateWindow(MainObjects.hWndMainWindow);
}

void ProgressBar::createProgressText()
{
	// Init the percentage values
	ProgressTextPercentageObjects.CopyingFilesPercentage = 0;
	ProgressTextPercentageObjects.ExpandingFilesPercentage = 0;
	ProgressTextPercentageObjects.InstallingFeaturesPercentage = 0;
	ProgressTextPercentageObjects.InstallingUpdatesPercentage = 0;

	// Create the text
	ProgressBar::updateProgressText(63, 123, ProgressTextPercentageObjects.CopyingFilesPercentage, AppResStringsObjects.CopyingFilesText);
	ProgressBar::updateProgressText(63, 143, ProgressTextPercentageObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText);
	ProgressBar::updateProgressText(63, 163, ProgressTextPercentageObjects.InstallingFeaturesPercentage, AppResStringsObjects.InstallingFeaturesText);
	ProgressBar::updateProgressText(63, 183, ProgressTextPercentageObjects.InstallingUpdatesPercentage, AppResStringsObjects.InstallingUpdatesText);
}

void ProgressBar::updateProgressText(int x, int y, int &ProgressPercantage, wchar_t *Text)
{
	TCHAR percentage[5] = { 0 };
	TCHAR ProgressText[30] = { 0 };

	swprintf_s(percentage, L"%d", ProgressPercantage);
	wcsncat_s(percentage, L"%", 5);

	wcsncat_s(ProgressText, Text, 24);
	wcsncat_s(ProgressText, percentage, 256);

	HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
	HFONT hFont, hTmp;
	hFont = CreateFontW(18, 0, 0, 0, FW_LIGHT, 0, 0, 0, 0, 0, 0, 2, 0, L"Segoe UI");
	hTmp = (HFONT)SelectObject(hdc, hFont);
	size_t size = wcslen(ProgressText);
	int convertsize = static_cast<int>(size);
	TextOutW(hdc, x, y, ProgressText, convertsize);
	ReleaseDC(MainObjects.hWndSetupWindow, hdc);

	::UpdateWindow(MainObjects.hWndSetupWindow);
}

void ProgressBar::paintTextBelowProgressBar(HWND hWnd, int x, int y, wchar_t *Text, int FontSize)
{
	TCHAR ProgressText[30] = { 0 };
	wcsncat_s(ProgressText, Text, wcslen(Text));

	HDC hdc = ::GetDC(hWnd);

	gr7::PaintText(hdc, x, y, L"Segoe UI", RGB(255, 255, 255), Text, FontSize, 1);

	::UpdateWindow(hWnd);
}