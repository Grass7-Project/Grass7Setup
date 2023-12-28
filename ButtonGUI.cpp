#include "stdafx.h"
#include "ButtonGUI.h"
#include "Global.h"
#include "GUI.h"

ButtonGUI BtnGUI;

// Initialize Back Button
void ButtonGUI::InitBackBtn()
{
	RECT rc;
	GetWindowRect(GlobalObjects.hWndSetupWindow, &rc);

	GlobalObjects.BackButtonDisabled = FALSE;
	BtnGUI.drawBackButton = TRUE;

	int height;
	int width;
	GUI::GetDesktopResolution(width, height);

	BitBlt(BtnGUI.hdcWndScreenshot, 0, 0, width, height, BtnGUI.hdcMainWnd, 0, 0, SRCCOPY);

	GlobalObjects.hBackBtn = CreateWindowExW(NULL,L"BUTTON", NULL, BS_OWNERDRAW | WS_CHILD | WS_VISIBLE, rc.left - 1, rc.top - 35, 31, 31, GlobalObjects.hWndMainWindow, (HMENU)ID_BACKBTN, GlobalObjects.hInst, 0);

	SetWindowSubclass(GlobalObjects.hBackBtn, &ButtonGUI::BackButtonProc, ID_BACKBTN, 0);
}

// Initialize Close Button
void ButtonGUI::InitCloseBtn()
{
	RECT rc;
	GetWindowRect(GlobalObjects.hWndSetupWindow, &rc);

	GlobalObjects.CloseButtonDisabled = FALSE;
	BtnGUI.drawCloseButton = TRUE;

	GlobalObjects.hCloseBtn = CreateWindowExW(NULL, L"BUTTON", NULL, BS_OWNERDRAW | WS_CHILD | WS_VISIBLE, rc.right - 40, rc.top - 42, 44, 16, GlobalObjects.hWndMainWindow, (HMENU)ID_CLOSEBTN, GlobalObjects.hInst, 0);

	SetWindowSubclass(GlobalObjects.hCloseBtn, &ButtonGUI::CloseButtonProc, ID_CLOSEBTN, 0);
}

// Initialize Normal Button
void ButtonGUI::InitNormalBtn()
{
	GlobalObjects.NormalButtonDisabled = FALSE;
	BtnGUI.drawNormalButton = TRUE;

	GlobalObjects.hNormalBtn = CreateWindowExW(NULL,L"BUTTON", NULL, BS_OWNERDRAW | WS_CHILD | WS_VISIBLE, 529, 394, 75, 23, GlobalObjects.hWndSetupWindow, (HMENU)ID_NORMALBTN, GlobalObjects.hInst, 0);
	SetWindowSubclass(GlobalObjects.hNormalBtn, &ButtonGUI::NormalButtonProc, ID_NORMALBTN, 0);
}

// Initialize Button Bitmaps
void ButtonGUI::InitButtonBitmaps()
{
	// Normal Button States
	BtnGUI.hBackBtnImg1 = static_cast<HBITMAP>(LoadImageW(GlobalObjects.hInst, MAKEINTRESOURCE(IDB_BACK_BTN1_BMP), IMAGE_BITMAP, 0, 0, 0));
	BtnGUI.hCloseBtnImg1 = static_cast<HBITMAP>(LoadImageW(GlobalObjects.hInst, MAKEINTRESOURCE(IDB_CLOSE_BTN1_BMP), IMAGE_BITMAP, 0, 0, 0));
	BtnGUI.hNormalBtnImg1 = static_cast<HBITMAP>(LoadImageW(GlobalObjects.hInst, MAKEINTRESOURCE(IDB_NORMAL_BTN1_BMP), IMAGE_BITMAP, 0, 0, 0));

	// Pressed Button States
	BtnGUI.hBackBtnImg2 = static_cast<HBITMAP>(LoadImageW(GlobalObjects.hInst, MAKEINTRESOURCE(IDB_BACK_BTN2_BMP), IMAGE_BITMAP, 0, 0, 0));
	BtnGUI.hCloseBtnImg2 = static_cast<HBITMAP>(LoadImageW(GlobalObjects.hInst, MAKEINTRESOURCE(IDB_CLOSE_BTN2_BMP), IMAGE_BITMAP, 0, 0, 0));
	BtnGUI.hNormalBtnImg2 = static_cast<HBITMAP>(LoadImageW(GlobalObjects.hInst, MAKEINTRESOURCE(IDB_NORMAL_BTN2_BMP), IMAGE_BITMAP, 0, 0, 0));

	// Disabled Button States
	BtnGUI.hBackBtnImg3 = static_cast<HBITMAP>(LoadImageW(GlobalObjects.hInst, MAKEINTRESOURCE(IDB_BACK_BTN3_BMP), IMAGE_BITMAP, 0, 0, 0));
	BtnGUI.hCloseBtnImg3 = static_cast<HBITMAP>(LoadImageW(GlobalObjects.hInst, MAKEINTRESOURCE(IDB_CLOSE_BTN3_BMP), IMAGE_BITMAP, 0, 0, 0));
	BtnGUI.hNormalBtnImg3 = static_cast<HBITMAP>(LoadImageW(GlobalObjects.hInst, MAKEINTRESOURCE(IDB_NORMAL_BTN3_BMP), IMAGE_BITMAP, 0, 0, 0));

	// Hover Button States
	BtnGUI.hBackBtnImg4 = static_cast<HBITMAP>(LoadImageW(GlobalObjects.hInst, MAKEINTRESOURCE(IDB_BACK_BTN4_BMP), IMAGE_BITMAP, 0, 0, 0));
	BtnGUI.hCloseBtnImg4 = static_cast<HBITMAP>(LoadImageW(GlobalObjects.hInst, MAKEINTRESOURCE(IDB_CLOSE_BTN4_BMP), IMAGE_BITMAP, 0, 0, 0));
	BtnGUI.hNormalBtnImg4 = static_cast<HBITMAP>(LoadImageW(GlobalObjects.hInst, MAKEINTRESOURCE(IDB_NORMAL_BTN4_BMP), IMAGE_BITMAP, 0, 0, 0));

	int height;
	int width;
	GUI::GetDesktopResolution(width, height);
	BtnGUI.hdcMainWnd = ::GetDC(GlobalObjects.hWndMainWindow);
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

		HGDIOBJ oldBitmap01 = SelectObject(hdcMem01, hButtonImg);

		BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		AlphaBlend(hdc, xBmpPos, yBmpPos, bitmap01.bmWidth, bitmap01.bmHeight, hdcMem01, 0, 0, bitmap01.bmWidth, bitmap01.bmHeight, bf);

		SelectObject(hdcMem01, oldBitmap01);
		DeleteDC(hdcMem01);
		EndPaint(hWnd, &ps);
		if (drawText == TRUE) {
			HDC hdc = ::GetDC(hWnd);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(0, 0, 0));
			HFONT hFont, hTmp;
			int nHeight = -MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
			hFont = CreateFontW(nHeight, 0, 0, 0, FW_LIGHT, 0, 0, 0, 0, 0, 0, 2, 0, L"Segoe UI");
			hTmp = (HFONT)SelectObject(hdc, hFont);
			size_t size = wcslen(text);
			int convertsize = static_cast<int>(size);
			TextOutW(hdc, (rc.right - rc.left)/2 - 12, (rc.bottom - rc.top) / 2 - 7, text, convertsize);
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
			if (!GlobalObjects.BackButtonDisabled) {
				ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.BackButtonDisabled, BtnGUI.drawBackButton, BtnGUI.hBackBtnTmpImg, BtnGUI.hBackBtnImg2);
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			if (!GlobalObjects.BackButtonDisabled) {
				ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.BackButtonDisabled, BtnGUI.drawBackButton, BtnGUI.hBackBtnTmpImg, BtnGUI.hBackBtnImg4);

				GlobalObjects.Page = GlobalObjects.Page - 1;
				GlobalObjects.CollectingInfoPercentage = GlobalObjects.CollectingInfoPercentage - 1;
				::SendMessageW(GlobalObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
				::SendMessageW(GlobalObjects.hWndSetupWindow, SETUPWND_UPDATE_DIALOG, (WPARAM)(INT)0, 0);
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
			if (!GlobalObjects.BackButtonDisabled) {
				ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.BackButtonDisabled, BtnGUI.drawBackButton, BtnGUI.hBackBtnTmpImg, BtnGUI.hBackBtnImg4);
			}
		}
		break;
		case WM_MOUSELEAVE:
		{
			BtnGUI.bButtonTracking = BtnGUI.bButtonHover = FALSE;
			if (!GlobalObjects.BackButtonDisabled) {
				ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.BackButtonDisabled, BtnGUI.drawBackButton, BtnGUI.hBackBtnTmpImg, BtnGUI.hBackBtnImg1);
			}
		}
		break;
		case BTN_DISABLE:
		{
			ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.BackButtonDisabled, BtnGUI.drawBackButton, BtnGUI.hBackBtnTmpImg, BtnGUI.hBackBtnImg3, TRUE);
		}
		break;
		case BTN_ENABLE:
		{
			ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.BackButtonDisabled, BtnGUI.drawBackButton, BtnGUI.hBackBtnTmpImg, BtnGUI.hBackBtnImg1, FALSE);
		}
		break;
		case WM_NCDESTROY:
			RemoveWindowSubclass(GlobalObjects.hBackBtn, &BackButtonProc, 1);
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
		if (!GlobalObjects.CloseButtonDisabled) {
			ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.CloseButtonDisabled, BtnGUI.drawCloseButton, BtnGUI.hCloseBtnTmpImg, BtnGUI.hCloseBtnImg2);
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		if (!GlobalObjects.CloseButtonDisabled) {
			ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.CloseButtonDisabled, BtnGUI.drawCloseButton, BtnGUI.hCloseBtnTmpImg, BtnGUI.hCloseBtnImg1);

			SendMessageW(GlobalObjects.hWndMainWindow, WM_CLOSE, (WPARAM)(INT)0, 0);
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
		if (!GlobalObjects.CloseButtonDisabled) {
			ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.CloseButtonDisabled, BtnGUI.drawCloseButton, BtnGUI.hCloseBtnTmpImg, BtnGUI.hCloseBtnImg4);
		}
	}
	break;
	case WM_MOUSELEAVE:
	{
		BtnGUI.cButtonTracking = BtnGUI.cButtonHover = FALSE;
		if (!GlobalObjects.CloseButtonDisabled) {
			ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.CloseButtonDisabled, BtnGUI.drawCloseButton, BtnGUI.hCloseBtnTmpImg, BtnGUI.hCloseBtnImg1);
		}
	}
	break;
	case BTN_DISABLE:
	{
		ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.CloseButtonDisabled, BtnGUI.drawCloseButton, BtnGUI.hCloseBtnTmpImg, BtnGUI.hCloseBtnImg3, TRUE);
	}
	break;
	case BTN_ENABLE:
	{
		ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.CloseButtonDisabled, BtnGUI.drawCloseButton, BtnGUI.hCloseBtnTmpImg, BtnGUI.hCloseBtnImg1, FALSE);
	}
	break;
	case WM_NCDESTROY:
		RemoveWindowSubclass(GlobalObjects.hCloseBtn, &CloseButtonProc, 1);
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
		if (GlobalObjects.InstallButtonText == FALSE) {
			ButtonGUI::Paint(hWnd, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, 0, 0, TRUE, gr7::LoadStringToW(GetModuleHandleW(NULL), IDS_NEXTBTN));
		}
		if (GlobalObjects.InstallButtonText == TRUE) {
			ButtonGUI::Paint(hWnd, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, 0, 0, TRUE, gr7::LoadStringToW(GetModuleHandleW(NULL), IDS_INSTALLBTN));
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		if (GlobalObjects.NormalButtonState != 3) {
			GlobalObjects.NormalButtonState = 2;
			ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg2);
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		if (GlobalObjects.NormalButtonState != 3) {
			ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg1);

			GlobalObjects.Page = GlobalObjects.Page + 1;
			GlobalObjects.CollectingInfoPercentage = GlobalObjects.CollectingInfoPercentage + 1;
			::SendMessageW(GlobalObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
			::SendMessageW(GlobalObjects.hWndSetupWindow, SETUPWND_UPDATE_DIALOG, (WPARAM)(INT)0, 0);
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
		if (GlobalObjects.NormalButtonState != 3) {
			GlobalObjects.NormalButtonState = 4;
			ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg4);
		}
	}
	break;
	case WM_MOUSELEAVE:
	{
		BtnGUI.nButtonTracking = BtnGUI.nButtonHover = FALSE;
		if (GlobalObjects.NormalButtonState != 3) {
			ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg1);
		}
	}
	break;
	case BTN_DISABLE:
	{
		GlobalObjects.NormalButtonState = 3;
		ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg3, TRUE);
	}
	break;
	case BTN_ENABLE:
	{
		GlobalObjects.NormalButtonState = 1;
		ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg1, FALSE);
	}
	break;
	case BTN_UPDATE:
	{
		if (GlobalObjects.Page == 5) {
			GlobalObjects.InstallButtonText = TRUE;
		} else {
			GlobalObjects.InstallButtonText = FALSE;
		}
		if (GlobalObjects.NormalButtonState == 1) {
			ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg1, FALSE);
		}
		if (GlobalObjects.NormalButtonState == 2) {
			ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg4, FALSE);
		}
		if (GlobalObjects.NormalButtonState == 3) {
			ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg3, TRUE);
		}
		if (GlobalObjects.NormalButtonState == 4) {
			ButtonGUI::ChangeBitmapState(hWnd, GlobalObjects.NormalButtonDisabled, BtnGUI.drawNormalButton, BtnGUI.hNormalBtnTmpImg, BtnGUI.hNormalBtnImg4);
		}
	}
	break;
	case WM_NCDESTROY:
		RemoveWindowSubclass(GlobalObjects.hNormalBtn, &NormalButtonProc, 1);
		break;
	}
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}