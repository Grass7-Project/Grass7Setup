#include "stdafx.h"
#include "ButtonGUI.h"
#include "MainInit.h"
#include "MainGUI.h"
#include "PartitionCode.h"

ButtonGUI BtnGUI;

// Initialize Back Button
void ButtonGUI::InitBackBtn()
{
	RECT rc;
	GetWindowRect(MainObjects.hWndSetupWindow, &rc);

	ButtonObjects.BackButtonDisabled = FALSE;

	int height;
	int width;
	Grass7API::Monitor::GetDesktopResolution(width, height);

	BitBlt(BtnGUI.hdcWndScreenshot, 0, 0, width, height, BtnGUI.hdcMainWnd, 0, 0, SRCCOPY);

	ButtonObjects.hBackBtn = CreateWindowExW(NULL,L"BUTTON", NULL, BS_OWNERDRAW | WS_CHILD | WS_VISIBLE, rc.left - 1, rc.top - 36, 31, 31, MainObjects.hWndMainWindow, (HMENU)ID_BACKBTN, MainObjects.hInst, 0);

	SetWindowSubclass(ButtonObjects.hBackBtn, &ButtonGUI::BackButtonProc, ID_BACKBTN, 0);
}

// Initialize Close Button
void ButtonGUI::InitCloseBtn()
{
	RECT rc;
	GetWindowRect(MainObjects.hWndSetupWindow, &rc);

	ButtonObjects.CloseButtonDisabled = FALSE;

	ButtonObjects.hCloseBtn = CreateWindowExW(NULL, L"BUTTON", NULL, BS_OWNERDRAW | WS_CHILD | WS_VISIBLE, rc.right - 40, rc.top - 42, 44, 16, MainObjects.hWndMainWindow, (HMENU)ID_CLOSEBTN, MainObjects.hInst, 0);

	SetWindowSubclass(ButtonObjects.hCloseBtn, &ButtonGUI::CloseButtonProc, ID_CLOSEBTN, 0);
}

// Initialize Normal Button
void ButtonGUI::InitNormalBtn()
{
	ButtonObjects.InstallButtonText = FALSE; // Set the install button text to not appear, currently
	ButtonObjects.NormalButtonDisabled = FALSE;
	ButtonObjects.NormalButtonState = 1;

	ButtonObjects.hNormalBtn = CreateWindowExW(NULL,L"BUTTON", NULL, BS_OWNERDRAW | WS_CHILD | WS_VISIBLE, 529, 394, 75, 23, MainObjects.hWndSetupWindow, (HMENU)ID_NORMALBTN, MainObjects.hInst, 0);
	SetWindowSubclass(ButtonObjects.hNormalBtn, &ButtonGUI::NormalButtonProc, ID_NORMALBTN, 0);
}

// Initialize Automatic Partitioning Button
void ButtonGUI::InitAutoPartitionButton()
{
	ButtonObjects.AutoPartButtonDisabled = FALSE;

	ButtonObjects.hAutoPartitionBtn = CreateWindowExW(NULL, L"BUTTON", NULL, BS_OWNERDRAW | WS_CHILD | WS_VISIBLE, 18, 338, 140, 26, MainObjects.hWndSetupWindow, (HMENU)ID_AUTOMATICPARTBTN, MainObjects.hInst, 0);
	SetWindowSubclass(ButtonObjects.hAutoPartitionBtn, &ButtonGUI::AutoPartButtonProc, ID_AUTOMATICPARTBTN, 0);
}
// Initialize Manual Partitioning Button
void ButtonGUI::InitManualPartitionButton()
{
	ButtonObjects.ManualPartButtonDisabled = FALSE;

	ButtonObjects.hManualPartitionBtn = CreateWindowExW(NULL, L"BUTTON", NULL, BS_OWNERDRAW | WS_CHILD | WS_VISIBLE, 175, 338, 123, 26, MainObjects.hWndSetupWindow, (HMENU)ID_MANUALPARTBTN, MainObjects.hInst, 0);
	SetWindowSubclass(ButtonObjects.hManualPartitionBtn, &ButtonGUI::ManualPartButtonProc, ID_MANUALPARTBTN, 0);
}

// Initialize Button Bitmaps for use
void ButtonGUI::InitButtonBitmaps()
{
	int height;
	int width;
	Grass7API::Monitor::GetDesktopResolution(width, height);
	BtnGUI.hdcMainWnd = ::GetDC(MainObjects.hWndMainWindow);
	BtnGUI.hdcWndScreenshot = ::CreateCompatibleDC(BtnGUI.hdcMainWnd);
	BtnGUI.hbmpWndScreenshot = ::CreateCompatibleBitmap(BtnGUI.hdcMainWnd, width, height);
	SelectObject(BtnGUI.hdcWndScreenshot, BtnGUI.hbmpWndScreenshot);

	BtnGUI.hBackBtnTmpImg = BitmapObjects.hBackBtnImg1;
	BtnGUI.hCloseBtnTmpImg = BitmapObjects.hCloseBtnImg1;
	BtnGUI.hNormalBtnTmpImg = BitmapObjects.hNormalBtnImg1;
	BtnGUI.hAutoPartBtnTmpImg = BitmapObjects.hAutoPartBtnImg1;
	BtnGUI.hManualPartBtnTmpImg = BitmapObjects.hManualPartBtnImg1;
}

void ButtonGUI::Paint(HWND &hWnd, HBITMAP &hButtonImg, int xBmpPos = 0, int yBmpPos = 0, int drawText = FALSE, LPCWSTR text = L"", int customTextXY = FALSE, int textX = 0, int textY = 0)
{
	InvalidateRect(hWnd, 0, TRUE);
	BITMAP          bitmap01;
	PAINTSTRUCT		ps;
	HDC				hdc;
	RECT			rc;
	GetWindowRect(hWnd, &rc);
	GetObjectW(hButtonImg, sizeof(bitmap01), &bitmap01);

	hdc = BeginPaint(hWnd, &ps);

	BitBlt(hdc, xBmpPos, yBmpPos, bitmap01.bmWidth, bitmap01.bmHeight, BtnGUI.hdcWndScreenshot, rc.left + xBmpPos, rc.top + yBmpPos, SRCCOPY);

	Grass7API::Paint::PaintTransparentBitmap(hdc, xBmpPos, yBmpPos, hButtonImg, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });

	if (drawText == TRUE) {
		if (customTextXY == 0) {
			Grass7API::Paint::PaintText(hdc, (rc.right - rc.left) / 2 - 12, (rc.bottom - rc.top) / 2 - 7, L"Segoe UI", RGB(0, 0, 0), text, 9, 1, TRANSPARENT, FW_LIGHT);
		}
		if (customTextXY == 1) {
			Grass7API::Paint::PaintText(hdc, textX, textY, L"Segoe UI", RGB(0, 0, 0), text, 9, 1, TRANSPARENT, FW_LIGHT);
		}
	}

	EndPaint(hWnd, &ps);
}

void ButtonGUI::ChangeBitmapState(HWND &hWnd, BOOL &ButtonDisabled, HBITMAP &hButtonTmpImg, HBITMAP &hButtonImg, BOOL setState = FALSE)
{
	ButtonDisabled = setState;
	hButtonTmpImg = hButtonImg;
	::RedrawWindow(hWnd, NULL, NULL, RDW_INTERNALPAINT);
}

// Back Button Procedure
LRESULT CALLBACK ButtonGUI::BackButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
		case WM_PAINT:
		{
			ButtonGUI::Paint(hWnd, BtnGUI.hBackBtnTmpImg);
		}
		break;
		case WM_LBUTTONDOWN:
		{
			if (!ButtonObjects.BackButtonDisabled) {
				ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.BackButtonDisabled, BtnGUI.hBackBtnTmpImg, BitmapObjects.hBackBtnImg2);
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			if (!ButtonObjects.BackButtonDisabled) {
				ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.BackButtonDisabled, BtnGUI.hBackBtnTmpImg, BitmapObjects.hBackBtnImg4);

				MainObjects.Page = MainObjects.Page - 1;
				ProgressBarObjects.CollectingInfoPercentage = ProgressBarObjects.CollectingInfoPercentage - 1;
				::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_COLLECT_INFO_PROG_BAR, (WPARAM)(INT)0, 0);
				::SendMessageW(MainObjects.hWndSetupWindow, SETUPWND_UPDATE_DIALOG, (WPARAM)(INT)0, 0);
			}
		}
		break;
		case WM_MOUSEMOVE:
		{
			if (!BtnGUI.BackButtonTracking) {
				TRACKMOUSEEVENT Tme;
				Tme.cbSize = sizeof(Tme);
				Tme.hwndTrack = hWnd;
				Tme.dwFlags = TME_LEAVE | TME_HOVER;
				Tme.dwHoverTime = 1;
				if(_TrackMouseEvent(&Tme))
					BtnGUI.BackButtonTracking = TRUE;
			}
			break;
		}
		case WM_MOUSEHOVER:
		{
			BtnGUI.BackButtonHover = TRUE;
			if (!ButtonObjects.BackButtonDisabled) {
				ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.BackButtonDisabled, BtnGUI.hBackBtnTmpImg, BitmapObjects.hBackBtnImg4);
			}
		}
		break;
		case WM_MOUSELEAVE:
		{
			BtnGUI.BackButtonTracking = BtnGUI.BackButtonHover = FALSE;
			if (!ButtonObjects.BackButtonDisabled) {
				ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.BackButtonDisabled, BtnGUI.hBackBtnTmpImg, BitmapObjects.hBackBtnImg1);
			}
		}
		break;
		case BTN_DISABLE:
		{
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.BackButtonDisabled, BtnGUI.hBackBtnTmpImg, BitmapObjects.hBackBtnImg3, TRUE);
		}
		break;
		case BTN_ENABLE:
		{
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.BackButtonDisabled, BtnGUI.hBackBtnTmpImg, BitmapObjects.hBackBtnImg1, FALSE);
		}
		break;
		case WM_NCDESTROY:
			RemoveWindowSubclass(ButtonObjects.hBackBtn, &BackButtonProc, 1);
			break;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

// Close Button Procedure
LRESULT CALLBACK ButtonGUI::CloseButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		ButtonGUI::Paint(hWnd, BtnGUI.hCloseBtnTmpImg, -7, -9);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (!ButtonObjects.CloseButtonDisabled) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.CloseButtonDisabled, BtnGUI.hCloseBtnTmpImg, BitmapObjects.hCloseBtnImg2);
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		if (!ButtonObjects.CloseButtonDisabled) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.CloseButtonDisabled, BtnGUI.hCloseBtnTmpImg, BitmapObjects.hCloseBtnImg1);

			std::thread ExitCode(MainInit::Exit);
			ExitCode.detach();
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (!BtnGUI.CloseButtonTracking) {
			TRACKMOUSEEVENT Tme;
			Tme.cbSize = sizeof(Tme);
			Tme.hwndTrack = hWnd;
			Tme.dwFlags = TME_LEAVE | TME_HOVER;
			Tme.dwHoverTime = 1;
			if (_TrackMouseEvent(&Tme))
				BtnGUI.CloseButtonTracking = TRUE;
		}
		break;
	}
	case WM_MOUSEHOVER:
	{
		BtnGUI.CloseButtonHover = TRUE;
		if (!ButtonObjects.CloseButtonDisabled) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.CloseButtonDisabled, BtnGUI.hCloseBtnTmpImg, BitmapObjects.hCloseBtnImg4);
		}
	}
	break;
	case WM_MOUSELEAVE:
	{
		BtnGUI.CloseButtonTracking = BtnGUI.CloseButtonHover = FALSE;
		if (!ButtonObjects.CloseButtonDisabled) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.CloseButtonDisabled, BtnGUI.hCloseBtnTmpImg, BitmapObjects.hCloseBtnImg1);
		}
	}
	break;
	case BTN_DISABLE:
	{
		ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.CloseButtonDisabled, BtnGUI.hCloseBtnTmpImg, BitmapObjects.hCloseBtnImg3, TRUE);
	}
	break;
	case BTN_ENABLE:
	{
		ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.CloseButtonDisabled, BtnGUI.hCloseBtnTmpImg, BitmapObjects.hCloseBtnImg1, FALSE);
	}
	break;
	case WM_NCDESTROY:
		RemoveWindowSubclass(ButtonObjects.hCloseBtn, &CloseButtonProc, 1);
		break;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

// Normal Button Procedure
LRESULT CALLBACK ButtonGUI::NormalButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		if (ButtonObjects.InstallButtonText == FALSE) {
			ButtonGUI::Paint(hWnd, BtnGUI.hNormalBtnTmpImg, 0, 0, TRUE, AppResStringsObjects.NextButtonText.c_str());
		}
		if (ButtonObjects.InstallButtonText == TRUE) {
			ButtonGUI::Paint(hWnd, BtnGUI.hNormalBtnTmpImg, 0, 0, TRUE, AppResStringsObjects.InstallButtonText.c_str());
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (ButtonObjects.NormalButtonState != 3) {
			ButtonObjects.NormalButtonState = 2;
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg2);
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		if (ButtonObjects.NormalButtonState != 3) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg1);

			MainObjects.Page = MainObjects.Page + 1;
			ProgressBarObjects.CollectingInfoPercentage = ProgressBarObjects.CollectingInfoPercentage + 1;
			::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_COLLECT_INFO_PROG_BAR, (WPARAM)(INT)0, 0);
			::SendMessageW(MainObjects.hWndSetupWindow, SETUPWND_UPDATE_DIALOG, (WPARAM)(INT)0, 0);
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (!BtnGUI.NormalButtonTracking) {
			TRACKMOUSEEVENT Tme;
			Tme.cbSize = sizeof(Tme);
			Tme.hwndTrack = hWnd;
			Tme.dwFlags = TME_LEAVE | TME_HOVER;
			Tme.dwHoverTime = 1;
			if (_TrackMouseEvent(&Tme))
				BtnGUI.NormalButtonTracking = TRUE;
		}
		break;
	}
	case WM_MOUSEHOVER:
	{
		BtnGUI.NormalButtonHover = TRUE;
		if (ButtonObjects.NormalButtonState != 3) {
			ButtonObjects.NormalButtonState = 4;
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg4);
		}
	}
	break;
	case WM_MOUSELEAVE:
	{
		BtnGUI.NormalButtonTracking = BtnGUI.NormalButtonHover = FALSE;
		if (ButtonObjects.NormalButtonState != 3) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg1);
		}
	}
	break;
	case BTN_DISABLE:
	{
		ButtonObjects.NormalButtonState = 3;
		ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg3, TRUE);
	}
	break;
	case BTN_ENABLE:
	{
		ButtonObjects.NormalButtonState = 1;
		ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg1, FALSE);
	}
	break;
	case BTN_UPDATE:
	{
		if (MainObjects.Page == 4) {
			ButtonObjects.InstallButtonText = TRUE;
		} else {
			ButtonObjects.InstallButtonText = FALSE;
		}
		if (ButtonObjects.NormalButtonState == 1) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg1, FALSE);
		}
		if (ButtonObjects.NormalButtonState == 2) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg4, FALSE);
		}
		if (ButtonObjects.NormalButtonState == 3) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg3, TRUE);
		}
		if (ButtonObjects.NormalButtonState == 4) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.hNormalBtnTmpImg, BitmapObjects.hNormalBtnImg4);
		}
	}
	break;
	case WM_NCDESTROY:
		RemoveWindowSubclass(ButtonObjects.hNormalBtn, &NormalButtonProc, 1);
		break;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

// Automatic Partitioning Button Procedure
LRESULT CALLBACK ButtonGUI::AutoPartButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		ButtonGUI::Paint(hWnd, BtnGUI.hAutoPartBtnTmpImg, 0, 0, TRUE, AppResStringsObjects.AutoPartButtonText.c_str(), TRUE, 9, 5);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (ButtonObjects.AutoPartButtonState != 3) {
			ButtonObjects.AutoPartButtonState = 2;
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.AutoPartButtonDisabled, BtnGUI.hAutoPartBtnTmpImg, BitmapObjects.hAutoPartBtnImg2);
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		if (ButtonObjects.AutoPartButtonState != 3) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.AutoPartButtonDisabled, BtnGUI.hAutoPartBtnTmpImg, BitmapObjects.hAutoPartBtnImg1);

			std::thread AutoPartThread(PartitionCode::AutomaticPartitioning);
			AutoPartThread.detach();
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (!BtnGUI.AutoPartButtonTracking) {
			TRACKMOUSEEVENT Tme;
			Tme.cbSize = sizeof(Tme);
			Tme.hwndTrack = hWnd;
			Tme.dwFlags = TME_LEAVE | TME_HOVER;
			Tme.dwHoverTime = 1;
			if (_TrackMouseEvent(&Tme))
				BtnGUI.AutoPartButtonTracking = TRUE;
		}
		break;
	}
	case WM_MOUSEHOVER:
	{
		BtnGUI.AutoPartButtonHover = TRUE;
		if (ButtonObjects.AutoPartButtonState != 3) {
			ButtonObjects.AutoPartButtonState = 4;
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.AutoPartButtonDisabled, BtnGUI.hAutoPartBtnTmpImg, BitmapObjects.hAutoPartBtnImg4);
		}
	}
	break;
	case WM_MOUSELEAVE:
	{
		BtnGUI.AutoPartButtonTracking = BtnGUI.AutoPartButtonHover = FALSE;
		if (ButtonObjects.AutoPartButtonState != 3) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.AutoPartButtonDisabled, BtnGUI.hAutoPartBtnTmpImg, BitmapObjects.hAutoPartBtnImg1);
		}
	}
	break;
	case BTN_DISABLE:
	{
		ButtonObjects.AutoPartButtonState = 3;
		ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.AutoPartButtonDisabled, BtnGUI.hAutoPartBtnTmpImg, BitmapObjects.hAutoPartBtnImg3, TRUE);
	}
	break;
	case BTN_ENABLE:
	{
		ButtonObjects.AutoPartButtonState = 1;
		ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.AutoPartButtonDisabled, BtnGUI.hAutoPartBtnTmpImg, BitmapObjects.hAutoPartBtnImg1, FALSE);
	}
	break;
	case BTN_UPDATE:
	{
		if (ButtonObjects.AutoPartButtonState == 1) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.AutoPartButtonDisabled, BtnGUI.hAutoPartBtnTmpImg, BitmapObjects.hAutoPartBtnImg1, FALSE);
		}
		if (ButtonObjects.AutoPartButtonState == 2) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.AutoPartButtonDisabled, BtnGUI.hAutoPartBtnTmpImg, BitmapObjects.hAutoPartBtnImg4, FALSE);
		}
		if (ButtonObjects.AutoPartButtonState == 3) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.AutoPartButtonDisabled, BtnGUI.hAutoPartBtnTmpImg, BitmapObjects.hAutoPartBtnImg3, TRUE);
		}
		if (ButtonObjects.AutoPartButtonState == 4) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.AutoPartButtonDisabled, BtnGUI.hAutoPartBtnTmpImg, BitmapObjects.hAutoPartBtnImg4);
		}
	}
	break;
	case WM_NCDESTROY:
		RemoveWindowSubclass(ButtonObjects.hAutoPartitionBtn, &AutoPartButtonProc, 1);
		break;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

// Manual Partitioning Button Procedure
LRESULT CALLBACK ButtonGUI::ManualPartButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		ButtonGUI::Paint(hWnd, BtnGUI.hManualPartBtnTmpImg, 0, 0, TRUE, AppResStringsObjects.ManualPartButtonText.c_str(), TRUE, 9, 5);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (ButtonObjects.ManualPartButtonState != 3) {
			ButtonObjects.ManualPartButtonState = 2;
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.ManualPartButtonDisabled, BtnGUI.hManualPartBtnTmpImg, BitmapObjects.hManualPartBtnImg2);
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		if (ButtonObjects.ManualPartButtonState != 3) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.ManualPartButtonDisabled, BtnGUI.hManualPartBtnTmpImg, BitmapObjects.hManualPartBtnImg1);

			std::thread ManualPartThread(PartitionCode::ManualPartitioning);
			ManualPartThread.detach();
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (!BtnGUI.ManualPartButtonTracking) {
			TRACKMOUSEEVENT Tme;
			Tme.cbSize = sizeof(Tme);
			Tme.hwndTrack = hWnd;
			Tme.dwFlags = TME_LEAVE | TME_HOVER;
			Tme.dwHoverTime = 1;
			if (_TrackMouseEvent(&Tme))
				BtnGUI.ManualPartButtonTracking = TRUE;
		}
		break;
	}
	case WM_MOUSEHOVER:
	{
		BtnGUI.ManualPartButtonHover = TRUE;
		if (ButtonObjects.ManualPartButtonState != 3) {
			ButtonObjects.ManualPartButtonState = 4;
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.ManualPartButtonDisabled, BtnGUI.hManualPartBtnTmpImg, BitmapObjects.hManualPartBtnImg4);
		}
	}
	break;
	case WM_MOUSELEAVE:
	{
		BtnGUI.ManualPartButtonTracking = BtnGUI.ManualPartButtonHover = FALSE;
		if (ButtonObjects.ManualPartButtonState != 3) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.ManualPartButtonDisabled, BtnGUI.hManualPartBtnTmpImg, BitmapObjects.hManualPartBtnImg1);
		}
	}
	break;
	case BTN_DISABLE:
	{
		ButtonObjects.ManualPartButtonState = 3;
		ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.ManualPartButtonDisabled, BtnGUI.hManualPartBtnTmpImg, BitmapObjects.hManualPartBtnImg3, TRUE);
	}
	break;
	case BTN_ENABLE:
	{
		ButtonObjects.ManualPartButtonState = 1;
		ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.ManualPartButtonDisabled, BtnGUI.hManualPartBtnTmpImg, BitmapObjects.hManualPartBtnImg1, FALSE);
	}
	break;
	case BTN_UPDATE:
	{
		if (ButtonObjects.ManualPartButtonState == 1) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.ManualPartButtonDisabled, BtnGUI.hManualPartBtnTmpImg, BitmapObjects.hManualPartBtnImg1, FALSE);
		}
		if (ButtonObjects.ManualPartButtonState == 2) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.ManualPartButtonDisabled, BtnGUI.hManualPartBtnTmpImg, BitmapObjects.hManualPartBtnImg4, FALSE);
		}
		if (ButtonObjects.ManualPartButtonState == 3) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.ManualPartButtonDisabled, BtnGUI.hManualPartBtnTmpImg, BitmapObjects.hManualPartBtnImg3, TRUE);
		}
		if (ButtonObjects.ManualPartButtonState == 4) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.ManualPartButtonDisabled, BtnGUI.hManualPartBtnTmpImg, BitmapObjects.hManualPartBtnImg4);
		}
	}
	break;
	case WM_NCDESTROY:
		RemoveWindowSubclass(ButtonObjects.hManualPartitionBtn, &ManualPartButtonProc, 1);
		break;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}