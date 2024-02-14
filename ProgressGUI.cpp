#include "stdafx.h"
#include "ProgressGUI.h"
#include "MainGUI.h"
#include "Global.h"
#include <Uxtheme.h>

void ProgressGUI::createProgressBar()
{
	int hoz, ver;

	gr7::GetDesktopResolution(hoz, ver);

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
	SetWindowTheme(ProgressBarObjects.hProgressCtrlCollectingInfo, L"", L"");
	::SendMessageW(ProgressBarObjects.hProgressCtrlCollectingInfo, PBM_SETBARCOLOR, 0, RGB(153, 204, 51));
	::SendMessageW(ProgressBarObjects.hProgressCtrlCollectingInfo, PBM_SETBKCOLOR, 0, RGB(122, 147, 177));

	::SendMessageW(ProgressBarObjects.hProgressCtrlInstalling, PBM_SETPOS, (WPARAM)(INT)0, 0);
	SetWindowTheme(ProgressBarObjects.hProgressCtrlInstalling, L"", L"");
	::SendMessageW(ProgressBarObjects.hProgressCtrlInstalling, PBM_SETBARCOLOR, 0, RGB(153, 204, 51));
	::SendMessageW(ProgressBarObjects.hProgressCtrlInstalling, PBM_SETBKCOLOR, 0, RGB(122, 147, 177));

	ProgressBarObjects.CollectingInfoPercentage = 0;
	ProgressBarObjects.InstallingPercentage = 0;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);

	ProgressGUI::paintTextBelowProgressBar(MainObjects.hWndMainWindow, 37, ver - 42, AppResStringsObjects.ProgressBarText1, 9);
	ProgressGUI::paintTextBelowProgressBar(MainObjects.hWndMainWindow, 228, ver - 42, AppResStringsObjects.ProgressBarText2, 9);
	ProgressGUI::paintTextBelowProgressBar(MainObjects.hWndMainWindow, 9, ver - 47, L"1", 25);
	ProgressGUI::paintTextBelowProgressBar(MainObjects.hWndMainWindow, 200, ver - 47, L"2", 25);
}

void ProgressGUI::updateProgressBar()
{
	::SendMessageW(ProgressBarObjects.hProgressCtrlCollectingInfo, PBM_SETPOS, (WPARAM)(INT)ProgressBarObjects.CollectingInfoPercentage, 0);
	::SendMessageW(ProgressBarObjects.hProgressCtrlInstalling, PBM_SETPOS, (WPARAM)(INT)ProgressBarObjects.InstallingPercentage, 0);
	::UpdateWindow(MainObjects.hWndMainWindow);
}

void ProgressGUI::createProgressText()
{
	// Init the percentage values
	ProgressTextPercentageObjects.CopyingFilesPercentage = 0;
	ProgressTextPercentageObjects.ExpandingFilesPercentage = 0;
	ProgressTextPercentageObjects.InstallingFeaturesPercentage = 0;
	ProgressTextPercentageObjects.InstallingUpdatesPercentage = 0;

	// Create the text
	ProgressGUI::updateProgressText(63, 123, ProgressTextPercentageObjects.CopyingFilesPercentage, AppResStringsObjects.CopyingFilesText, RGB(128, 128, 128), FALSE, L"", FW_LIGHT);
	ProgressGUI::updateProgressText(63, 143, ProgressTextPercentageObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText, RGB(128, 128, 128), FALSE, L"", FW_LIGHT);
	ProgressGUI::updateProgressText(63, 163, ProgressTextPercentageObjects.InstallingFeaturesPercentage, AppResStringsObjects.InstallingFeaturesText, RGB(128, 128, 128), FALSE, L"", FW_LIGHT);
	ProgressGUI::updateProgressText(63, 183, ProgressTextPercentageObjects.InstallingUpdatesPercentage, AppResStringsObjects.InstallingUpdatesText, RGB(128, 128, 128), FALSE, L"", FW_LIGHT);
}

void ProgressGUI::updateProgressText(int x, int y, int &ProgressPercantage, std::wstring Text, COLORREF txtColor, int AddPercentage, std::wstring ExtraText, int cWeight)
{
	std::wstring ProgressText = Text;
	std::wstring percentage(MAX_PATH, 0);

	if (AddPercentage == 1) {
		percentage.resize((size_t)swprintf_s(&percentage[0], percentage.size(), L"%d", ProgressPercantage));
		percentage.append(L"%");

		ProgressText.append(L"(");
		ProgressText.append(percentage);
		ProgressText.append(L")");
		ProgressText.append(ExtraText);
		ProgressText.append(L"            ");
	}
	else {
		ProgressText.append(ExtraText);
		ProgressText.append(L"                       ");
	}

	HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
	PaintTextOptions PaintTextOpt;
	PaintTextOpt.xPos = x;
	PaintTextOpt.yPos = y;
	PaintTextOpt.font = L"Segoe UI";
	PaintTextOpt.color = txtColor;
	PaintTextOpt.text = ProgressText.c_str();
	PaintTextOpt.nSize = 9;
	PaintTextOpt.BkMode = OPAQUE;
	PaintTextOpt.cWeight = cWeight;

	gr7::PaintText(hdc, PaintTextOpt);

	ReleaseDC(MainObjects.hWndSetupWindow, hdc);

	::UpdateWindow(MainObjects.hWndSetupWindow);
}

void ProgressGUI::paintTextBelowProgressBar(HWND hWnd, int x, int y, std::wstring Text, int FontSize)
{
	HDC hdc = ::GetDC(hWnd);
	PaintTextOptions PaintTextOpt;
	PaintTextOpt.xPos = x;
	PaintTextOpt.yPos = y;
	PaintTextOpt.font = L"Segoe UI";
	PaintTextOpt.color = RGB(255, 255, 255);
	PaintTextOpt.text = Text.c_str();
	PaintTextOpt.nSize = FontSize;

	gr7::PaintText(hdc, PaintTextOpt);

	::UpdateWindow(hWnd);
}

void ProgressGUI::WaitThread()
{
	ImageInstallObjects.WaitThreadRunning = 1;
	while (ImageInstallObjects.WaitThreadGo == 0) {
		Sleep(1000);
		if (ImageInstallObjects.CopyingFiles == 1) {
			ProgressGUI::updateProgressText(63, 123, ProgressTextPercentageObjects.CopyingFilesPercentage, AppResStringsObjects.CopyingFilesText, RGB(0, 0, 0), TRUE, L" .", FW_BOLD);
			Sleep(1000);
			ProgressGUI::updateProgressText(63, 123, ProgressTextPercentageObjects.CopyingFilesPercentage, AppResStringsObjects.CopyingFilesText, RGB(0, 0, 0), TRUE, L" ..", FW_BOLD);
			Sleep(1000);
			ProgressGUI::updateProgressText(63, 123, ProgressTextPercentageObjects.CopyingFilesPercentage, AppResStringsObjects.CopyingFilesText, RGB(0, 0, 0), TRUE, L" ...", FW_BOLD);
		}
		if (ImageInstallObjects.ExpandingFiles == 1) {
			ProgressGUI::updateProgressText(63, 143, ProgressTextPercentageObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText, RGB(0, 0, 0), TRUE, L" .", FW_BOLD);
			Sleep(1000);
			ProgressGUI::updateProgressText(63, 143, ProgressTextPercentageObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText, RGB(0, 0, 0), TRUE, L" ..", FW_BOLD);
			Sleep(1000);
			ProgressGUI::updateProgressText(63, 143, ProgressTextPercentageObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText, RGB(0, 0, 0), TRUE, L" ...", FW_BOLD);
		}
		if (ImageInstallObjects.InstallingFeatures == 1) {
			ProgressGUI::updateProgressText(63, 163, ProgressTextPercentageObjects.InstallingFeaturesPercentage, AppResStringsObjects.InstallingFeaturesText, RGB(0, 0, 0), TRUE, L" .", FW_BOLD);
			Sleep(1000);
			ProgressGUI::updateProgressText(63, 163, ProgressTextPercentageObjects.InstallingFeaturesPercentage, AppResStringsObjects.InstallingFeaturesText, RGB(0, 0, 0), TRUE, L" ..", FW_BOLD);
			Sleep(1000);
			ProgressGUI::updateProgressText(63, 163, ProgressTextPercentageObjects.InstallingFeaturesPercentage, AppResStringsObjects.InstallingFeaturesText, RGB(0, 0, 0), TRUE, L" ...", FW_BOLD);
		}
		if (ImageInstallObjects.InstallingUpdates == 1) {
			ProgressGUI::updateProgressText(63, 183, ProgressTextPercentageObjects.InstallingUpdatesPercentage, AppResStringsObjects.InstallingUpdatesText, RGB(0, 0, 0), TRUE, L" .", FW_BOLD);
			Sleep(1000);
			ProgressGUI::updateProgressText(63, 183, ProgressTextPercentageObjects.InstallingUpdatesPercentage, AppResStringsObjects.InstallingUpdatesText, RGB(0, 0, 0), TRUE, L" ..", FW_BOLD);
			Sleep(1000);
			ProgressGUI::updateProgressText(63, 183, ProgressTextPercentageObjects.InstallingUpdatesPercentage, AppResStringsObjects.InstallingUpdatesText, RGB(0, 0, 0), TRUE, L" ...", FW_BOLD);
		}
	}
	ImageInstallObjects.WaitThreadRunning = 0;
}