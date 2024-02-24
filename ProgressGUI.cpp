#include "stdafx.h"
#include "ProgressGUI.h"
#include <Uxtheme.h>

void ProgressGUI::createProgressBar()
{
	int hoz, ver;
	gr7::GetDesktopResolution(hoz, ver);

	ProgressGUI::createProgressBarNew(MainObjects.hWndMainWindow, MainObjects.hInst, ProgressBarObjects.hProgressCtrlCollectingInfo, 0, ver - 66, 208, 11, ProgressBarObjects.CollectingInfoPercentage, RGB(153, 204, 51), RGB(122, 147, 177), MAINWND_UPDATE_COLLECT_INFO_PROG_BAR);
	if (!ProgressBarObjects.hProgressCtrlCollectingInfo) {
		ErrorHandler::InvokeErrorHandler(1, 0, L"Failed to create Collecting Information progress bar", AppResStringsObjects.AppTitleText);
	}
	ProgressGUI::createProgressBarNew(MainObjects.hWndMainWindow, MainObjects.hInst, ProgressBarObjects.hProgressCtrlInstalling, 210, ver - 66, hoz - 210, 11, ProgressBarObjects.InstallingPercentage, RGB(153, 204, 51), RGB(122, 147, 177), MAINWND_UPDATE_INSTALLING_PROG_BAR);
	if (!ProgressBarObjects.hProgressCtrlInstalling) {
		ErrorHandler::InvokeErrorHandler(1, 0, L"Failed to create Installing progress bar", AppResStringsObjects.AppTitleText);
	}
	ProgressGUI::createProgressBarNew(MainObjects.hWndSetupWindow, MainObjects.hInst, ProgressBarObjects.hProgressCtrlRestarting, 42, 62, 525, 11, ProgressBarObjects.RestartingPercentage, RGB(153, 204, 51), RGB(122, 147, 177), SETUPWND_RESTARTING_TIMER);
	if (!ProgressBarObjects.hProgressCtrlRestarting) {
		ErrorHandler::InvokeErrorHandler(1, 0, L"Failed to create Restarting progress bar", AppResStringsObjects.AppTitleText);
	}
	else {
		::ShowWindow(ProgressBarObjects.hProgressCtrlRestarting, FALSE);
	}
}

void ProgressGUI::createProgressBarNew(HWND &hWndParent, HINSTANCE &hInst, HWND &hWndProgressBar, int xPos, int yPos, int Width, int Height, int &Percentage, COLORREF BarColor, COLORREF BackgroundColor, int MessageID)
{
	hWndProgressBar = ::CreateWindowExW(
		0,
		PROGRESS_CLASS,
		L"",
		WS_CHILD | WS_VISIBLE | PBS_SMOOTH | WS_BORDER,
		xPos,
		yPos,
		Width,
		Height,
		hWndParent,
		(HMENU)ID_SMOOTHPROGRESSCTRL,
		hInst,
		NULL);

	::SendMessageW(hWndProgressBar, PBM_SETPOS, (WPARAM)(INT)0, 0);
	SetWindowTheme(hWndProgressBar, L"", L"");
	::SendMessageW(hWndProgressBar, PBM_SETBARCOLOR, 0, BarColor);
	::SendMessageW(hWndProgressBar, PBM_SETBKCOLOR, 0, BackgroundColor);

	Percentage = 0;
	::SendMessageW(MainObjects.hWndMainWindow, MessageID, (WPARAM)(INT)0, 0);
}

void ProgressGUI::updateProgressBar(HWND &hParentWnd, HWND &hProgressBarWnd, int &ProgressBarPercentage)
{
	::SendMessageW(hProgressBarWnd, PBM_SETPOS, (WPARAM)(INT)ProgressBarPercentage, 0);
	::UpdateWindow(hParentWnd);
}

void ProgressGUI::createProgressText(HDC &hdc, int &percent1, int &percent2, int &percent3, int &percent4, std::wstring &Text1, std::wstring &Text2, std::wstring &Text3, std::wstring &Text4)
{
	// Init the percentage values
	percent1 = 0;
	percent2 = 0;
	percent3 = 0;
	percent4 = 0;

	// Create the text
	ProgressGUI::updateProgressText(hdc, 63, 123, percent1, Text1, RGB(128, 128, 128), FALSE, L"", FW_LIGHT);
	ProgressGUI::updateProgressText(hdc, 63, 143, percent2, Text2, RGB(128, 128, 128), FALSE, L"", FW_LIGHT);
	ProgressGUI::updateProgressText(hdc, 63, 163, percent3, Text3, RGB(128, 128, 128), FALSE, L"", FW_LIGHT);
	ProgressGUI::updateProgressText(hdc, 63, 183, percent4, Text4, RGB(128, 128, 128), FALSE, L"", FW_LIGHT);
}

void ProgressGUI::updateProgressTexthWnd(HWND &hWnd, int x, int y, int &ProgressPercantage, std::wstring &Text, COLORREF txtColor, int AddPercentage, std::wstring ExtraText, int cWeight)
{
	HDC hdc = GetDC(hWnd);
	ProgressGUI::updateProgressText(hdc, x, y, ProgressPercantage, Text, txtColor, AddPercentage, ExtraText, cWeight);
	ReleaseDC(hWnd, hdc);
}

void ProgressGUI::updateProgressText(HDC &hdc, int x, int y, int &ProgressPercantage, std::wstring &Text, COLORREF txtColor, int AddPercentage, std::wstring ExtraText, int cWeight)
{
	std::wstring ProgressText = Text;
	std::wstring percentage(MAX_PATH, 0);

	if (AddPercentage == 1) {
		percentage.resize((size_t)swprintf_s(&percentage[0], percentage.size(), L"%d", ProgressPercantage));

		ProgressText.append(L"(");
		ProgressText.append(percentage);
		ProgressText.append(L"%)");
		ProgressText.append(ExtraText);
		ProgressText.append(L"            ");
	}
	else {
		ProgressText.append(ExtraText);
		ProgressText.append(L"                       ");
	}

	gr7::PaintText(hdc, x, y, L"Segoe UI", txtColor, ProgressText.c_str(), 9, 1, OPAQUE, cWeight);
}

void ProgressGUI::WaitThread(BOOL &WaitThreadRunning, BOOL &WaitThreadExit, int &Percentage, std::wstring &Text, int xPos, int yPos, std::wstring &Dots)
{
	WaitThreadRunning = TRUE;
	while (WaitThreadExit == FALSE) {
		Sleep(1000);
		Dots = L" .";
		ProgressGUI::updateProgressTexthWnd(MainObjects.hWndSetupWindow, xPos, yPos, Percentage, Text, RGB(0, 0, 0), TRUE, Dots, FW_BOLD);
		Sleep(1000);
		Dots = L" ..";
		ProgressGUI::updateProgressTexthWnd(MainObjects.hWndSetupWindow, xPos, yPos, Percentage, Text, RGB(0, 0, 0), TRUE, Dots, FW_BOLD);
		Sleep(1000);
		Dots = L" ...";
		ProgressGUI::updateProgressTexthWnd(MainObjects.hWndSetupWindow, xPos, yPos, Percentage, Text, RGB(0, 0, 0), TRUE, Dots, FW_BOLD);
	}
	WaitThreadRunning = FALSE;
}