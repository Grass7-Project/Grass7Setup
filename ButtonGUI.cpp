#include "stdafx.h"
#include "ButtonGUI.h"
#include "Global.h"
#include "GUI.h"

ButtonGUI BtnGUI;

// Initialize Back Button
void ButtonGUI::InitBackBtn()
{
	RECT rc;
	GetWindowRect(MainObjects.hWndSetupWindow, &rc);

	ButtonObjects.BackButtonDisabled = FALSE;
	BtnGUI.drawBackButton = TRUE;

	int height;
	int width;
	GUI::GetDesktopResolution(width, height);

	BitBlt(BtnGUI.hdcWndScreenshot, 0, 0, width, height, BtnGUI.hdcMainWnd, 0, 0, SRCCOPY);

	ButtonObjects.hBackBtn = CreateWindowExW(NULL,L"BUTTON", NULL, BS_OWNERDRAW | WS_CHILD | WS_VISIBLE, rc.left - 1, rc.top - 35, 31, 31, MainObjects.hWndMainWindow, (HMENU)ID_BACKBTN, MainObjects.hInst, 0);

	SetWindowSubclass(ButtonObjects.hBackBtn, &ButtonGUI::BackButtonProc, ID_BACKBTN, 0);
}

// Initialize Close Button
void ButtonGUI::InitCloseBtn()
{
	RECT rc;
	GetWindowRect(MainObjects.hWndSetupWindow, &rc);

	ButtonObjects.CloseButtonDisabled = FALSE;
	BtnGUI.drawCloseButton = TRUE;

	ButtonObjects.hCloseBtn = CreateWindowExW(NULL, L"BUTTON", NULL, BS_OWNERDRAW | WS_CHILD | WS_VISIBLE, rc.right - 40, rc.top - 42, 44, 16, MainObjects.hWndMainWindow, (HMENU)ID_CLOSEBTN, MainObjects.hInst, 0);

	SetWindowSubclass(ButtonObjects.hCloseBtn, &ButtonGUI::CloseButtonProc, ID_CLOSEBTN, 0);
}

// Initialize Normal Button
void ButtonGUI::InitNormalBtn()
{
	ButtonObjects.NormalButtonDisabled = FALSE;
	BtnGUI.drawNormalButton = TRUE;

	ButtonObjects.hNormalBtn = CreateWindowExW(NULL,L"BUTTON", NULL, BS_OWNERDRAW | WS_CHILD | WS_VISIBLE, 529, 394, 75, 23, MainObjects.hWndSetupWindow, (HMENU)ID_NORMALBTN, MainObjects.hInst, 0);
	SetWindowSubclass(ButtonObjects.hNormalBtn, &ButtonGUI::NormalButtonProc, ID_NORMALBTN, 0);
}

// Initialize Button Bitmaps
void ButtonGUI::InitButtonBitmaps()
{
	// Normal Button States
	BtnGUI.hBackBtnImg1 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_BACK_BTN1_BMP), IMAGE_BITMAP, 0, 0, 0));
	BtnGUI.hCloseBtnImg1 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_CLOSE_BTN1_BMP), IMAGE_BITMAP, 0, 0, 0));
	BtnGUI.hNormalBtnImg1 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_NORMAL_BTN1_BMP), IMAGE_BITMAP, 0, 0, 0));

	// Pressed Button States
	BtnGUI.hBackBtnImg2 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_BACK_BTN2_BMP), IMAGE_BITMAP, 0, 0, 0));
	BtnGUI.hCloseBtnImg2 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_CLOSE_BTN2_BMP), IMAGE_BITMAP, 0, 0, 0));
	BtnGUI.hNormalBtnImg2 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_NORMAL_BTN2_BMP), IMAGE_BITMAP, 0, 0, 0));

	// Disabled Button States
	BtnGUI.hBackBtnImg3 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_BACK_BTN3_BMP), IMAGE_BITMAP, 0, 0, 0));
	BtnGUI.hCloseBtnImg3 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_CLOSE_BTN3_BMP), IMAGE_BITMAP, 0, 0, 0));
	BtnGUI.hNormalBtnImg3 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_NORMAL_BTN3_BMP), IMAGE_BITMAP, 0, 0, 0));

	// Hover Button States
	BtnGUI.hBackBtnImg4 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_BACK_BTN4_BMP), IMAGE_BITMAP, 0, 0, 0));
	BtnGUI.hCloseBtnImg4 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_CLOSE_BTN4_BMP), IMAGE_BITMAP, 0, 0, 0));
	BtnGUI.hNormalBtnImg4 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_NORMAL_BTN4_BMP), IMAGE_BITMAP, 0, 0, 0));

	int height;
	int width;
	GUI::GetDesktopResolution(width, height);
	BtnGUI.hdcMainWnd = ::GetDC(MainObjects.hWndMainWindow);
	BtnGUI.hdcWndScreenshot = ::CreateCompatibleDC(BtnGUI.hdcMainWnd);
	BtnGUI.hbmpWndScreenshot = ::CreateCompatibleBitmap(BtnGUI.hdcMainWnd, width, height);
	SelectObject(BtnGUI.hdcWndScreenshot, BtnGUI.hbmpWndScreenshot);

	BtnGUI.hBackBtnTmpImg = BtnGUI.hBackBtnImg1;
	BtnGUI.hCloseBtnTmpImg = BtnGUI.hCloseBtnImg1;
	BtnGUI.hNormalBtnTmpImg = BtnGUI.hNormalBtnImg1;
}

void ButtonGUI::Paint(HWND &hWnd, int &drawButton, HBITMAP &hButtonImg, int xBmpPos = 0, int yBmpPos = 0, int drawText = FALSE, wchar_t *text = L"")
{
	if (drawButton == TRUE) {
		InvalidateRect(hWnd, 0, TRUE);
		BITMAP          bitmap01;
		PAINTSTRUCT		ps;
		RECT			rc;
		GetWindowRect(hWnd, &rc);
		GetObjectW(hButtonImg, sizeof(bitmap01), &bitmap01);

		HDC hdc = BeginPaint(hWnd, &ps);

		BitBlt(hdc, xBmpPos, yBmpPos, bitmap01.bmWidth, bitmap01.bmHeight, BtnGUI.hdcWndScreenshot, rc.left + xBmpPos, rc.top + yBmpPos, SRCCOPY);

		HDC hdcMem01 = CreateCompatibleDC(hdc);
		gr7::PaintTransparentBitmap(hdc, xBmpPos, yBmpPos, hButtonImg, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
		DeleteDC(hdcMem01);

		EndPaint(hWnd, &ps);
		if (drawText == TRUE) {
			HDC hdc = ::GetDC(hWnd);
			gr7::PaintText(hdc, (rc.right - rc.left) / 2 - 12, (rc.bottom - rc.top) / 2 - 7, L"Segoe UI", RGB(0, 0, 0), text, 9, 1);
			ReleaseDC(hWnd, hdc);

			::UpdateWindow(hWnd);
		}
		drawButton = FALSE;
	}
}

void ButtonGUI::ChangeBitmapState(HWND &hWnd, int &ButtonDisabled, int &drawButton, HBITMAP &hButtonTmpImg, HBITMAP &hButtonImg, int setState = FALSE)
{
	ButtonDisabled = setState;
	hButtonTmpImg = NULL;
	hButtonTmpImg = hButtonImg;
	drawButton = TRUE;
	::RedrawWindow(hWnd, NULL, NULL, RDW_INTERNALPAINT);
}

// Back Button Procedure
LRESULT CALLBACK ButtonGUI::BackButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
		case WM_PAINT:
		{
			ButtonGUI::Paint(hWnd, BtnGUI.drawBackButton, BtnGUI.hBackBtnTmpImg);
		}
		break;
		case WM_LBUTTONDOWN:
		{
			if (!ButtonObjects.BackButtonDisabled) {
				ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.BackButtonDisabled, BtnGUI.drawBackButton, BtnGUI.hBackBtnTmpImg, BtnGUI.hBackBtnImg2);
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			if (!ButtonObjects.BackButtonDisabled) {
				ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.BackButtonDisabled, BtnGUI.drawBackButton, BtnGUI.hBackBtnTmpImg, BtnGUI.hBackBtnImg4);

				MainObjects.Page = MainObjects.Page - 1;
				ProgressBarObjects.CollectingInfoPercentage = ProgressBarObjects.CollectingInfoPercentage - 1;
				::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
				::SendMessageW(MainObjects.hWndSetupWindow, SETUPWND_UPDATE_DIALOG, (WPARAM)(INT)0, 0);
			}
		}
		break;
		case WM_MOUSEMOVE:
		{
			if (!BtnGUI.bButtonTracking) {
				TRACKMOUSEEVENT Tme;
				Tme.cbSize = sizeof(Tme);
				Tme.hwndTrack = hWnd;
				Tme.dwFlags = TME_LEAVE | TME_HOVER;
				Tme.dwHoverTime = 1;
				if(_TrackMouseEvent(&Tme))
					BtnGUI.bButtonTracking = TRUE;
			}
			break;
		}
		case WM_MOUSEHOVER:
		{
			BtnGUI.bButtonHover = TRUE;
			if (!ButtonObjects.BackButtonDisabled) {
				ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.BackButtonDisabled, BtnGUI.drawBackButton, BtnGUI.hBackBtnTmpImg, BtnGUI.hBackBtnImg4);
			}
		}
		break;
		case WM_MOUSELEAVE:
		{
			BtnGUI.bButtonTracking = BtnGUI.bButtonHover = FALSE;
			if (!ButtonObjects.BackButtonDisabled) {
				ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.BackButtonDisabled, BtnGUI.drawBackButton, BtnGUI.hBackBtnTmpImg, BtnGUI.hBackBtnImg1);
			}
		}
		break;
		case BTN_DISABLE:
		{
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.BackButtonDisabled, BtnGUI.drawBackButton, BtnGUI.hBackBtnTmpImg, BtnGUI.hBackBtnImg3, TRUE);
		}
		break;
		case BTN_ENABLE:
		{
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.BackButtonDisabled, BtnGUI.drawBackButton, BtnGUI.hBackBtnTmpImg, BtnGUI.hBackBtnImg1, FALSE);
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
		ButtonGUI::Paint(hWnd, BtnGUI.drawCloseButton, BtnGUI.hCloseBtnTmpImg, -7, -9);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (!ButtonObjects.CloseButtonDisabled) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.CloseButtonDisabled, BtnGUI.drawCloseButton, BtnGUI.hCloseBtnTmpImg, BtnGUI.hCloseBtnImg2);
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		if (!ButtonObjects.CloseButtonDisabled) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.CloseButtonDisabled, BtnGUI.drawCloseButton, BtnGUI.hCloseBtnTmpImg, BtnGUI.hCloseBtnImg1);

			SendMessageW(MainObjects.hWndMainWindow, WM_CLOSE, (WPARAM)(INT)0, 0);
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (!BtnGUI.cButtonTracking) {
			TRACKMOUSEEVENT Tme;
			Tme.cbSize = sizeof(Tme);
			Tme.hwndTrack = hWnd;
			Tme.dwFlags = TME_LEAVE | TME_HOVER;
			Tme.dwHoverTime = 1;
			if (_TrackMouseEvent(&Tme))
				BtnGUI.cButtonTracking = TRUE;
		}
		break;
	}
	case WM_MOUSEHOVER:
	{
		BtnGUI.cButtonHover = TRUE;
		if (!ButtonObjects.CloseButtonDisabled) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.CloseButtonDisabled, BtnGUI.drawCloseButton, BtnGUI.hCloseBtnTmpImg, BtnGUI.hCloseBtnImg4);
		}
	}
	break;
	case WM_MOUSELEAVE:
	{
		BtnGUI.cButtonTracking = BtnGUI.cButtonHover = FALSE;
		if (!ButtonObjects.CloseButtonDisabled) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.CloseButtonDisabled, BtnGUI.drawCloseButton, BtnGUI.hCloseBtnTmpImg, BtnGUI.hCloseBtnImg1);
		}
	}
	break;
	case BTN_DISABLE:
	{
		ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.CloseButtonDisabled, BtnGUI.drawCloseButton, BtnGUI.hCloseBtnTmpImg, BtnGUI.hCloseBtnImg3, TRUE);
	}
	break;
	case BTN_ENABLE:
	{
		ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.CloseButtonDisabled, BtnGUI.drawCloseButton, BtnGUI.hCloseBtnTmpImg, BtnGUI.hCloseBtnImg1, FALSE);
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
			ButtonGUI::Paint(hWnd, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, 0, 0, TRUE, AppResStringsObjects.NextButtonText);
		}
		if (ButtonObjects.InstallButtonText == TRUE) {
			ButtonGUI::Paint(hWnd, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, 0, 0, TRUE, AppResStringsObjects.InstallButtonText);
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (ButtonObjects.NormalButtonState != 3) {
			ButtonObjects.NormalButtonState = 2;
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg2);
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		if (ButtonObjects.NormalButtonState != 3) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg1);

			MainObjects.Page = MainObjects.Page + 1;
			ProgressBarObjects.CollectingInfoPercentage = ProgressBarObjects.CollectingInfoPercentage + 1;
			::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
			::SendMessageW(MainObjects.hWndSetupWindow, SETUPWND_UPDATE_DIALOG, (WPARAM)(INT)0, 0);
		}
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (!BtnGUI.nButtonTracking) {
			TRACKMOUSEEVENT Tme;
			Tme.cbSize = sizeof(Tme);
			Tme.hwndTrack = hWnd;
			Tme.dwFlags = TME_LEAVE | TME_HOVER;
			Tme.dwHoverTime = 1;
			if (_TrackMouseEvent(&Tme))
				BtnGUI.nButtonTracking = TRUE;
		}
		break;
	}
	case WM_MOUSEHOVER:
	{
		BtnGUI.nButtonHover = TRUE;
		if (ButtonObjects.NormalButtonState != 3) {
			ButtonObjects.NormalButtonState = 4;
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg4);
		}
	}
	break;
	case WM_MOUSELEAVE:
	{
		BtnGUI.nButtonTracking = BtnGUI.nButtonHover = FALSE;
		if (ButtonObjects.NormalButtonState != 3) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg1);
		}
	}
	break;
	case BTN_DISABLE:
	{
		ButtonObjects.NormalButtonState = 3;
		ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg3, TRUE);
	}
	break;
	case BTN_ENABLE:
	{
		ButtonObjects.NormalButtonState = 1;
		ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg1, FALSE);
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
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg1, FALSE);
		}
		if (ButtonObjects.NormalButtonState == 2) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg4, FALSE);
		}
		if (ButtonObjects.NormalButtonState == 3) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg3, TRUE);
		}
		if (ButtonObjects.NormalButtonState == 4) {
			ButtonGUI::ChangeBitmapState(hWnd, ButtonObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg4);
		}
	}
	break;
	case WM_NCDESTROY:
		RemoveWindowSubclass(ButtonObjects.hNormalBtn, &NormalButtonProc, 1);
		break;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}