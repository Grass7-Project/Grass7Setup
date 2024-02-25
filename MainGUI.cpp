#include "stdafx.h"
#include "MainGUI.h"
#include "SetupGUI.h"
#include "DialogGUI.h"
#include "ProgressGUI.h"
#include "ButtonGUI.h"
#include "ResourceLoader.h"

MainGUI MainGUIObj;

void MainGUI::Init(HWND &hWnd, HINSTANCE &hInst)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainGUI::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = MainObjects.hInst;
	wcex.hIcon = LoadIconW(MainObjects.hInst, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"MainWindow";
	wcex.hIconSm = LoadIconW(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	hWnd = CreateWindowW(
		L"MainWindow",
		AppResStringsObjects.AppTitleText.c_str(),
		WS_SYSMENU | DS_FIXEDSYS,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInst,
		NULL);

	if (!hWnd) {
		ErrorHandler::InvokeErrorHandler(1, 0, L"Failed to create Main Window", AppResStringsObjects.AppTitleText);
	}
	else {
		EnableMenuItem(GetSystemMenu(hWnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

		SetWindowLongW(hWnd, GWL_STYLE, GetWindowLongW(hWnd, GWL_STYLE) & ~WS_MINIMIZEBOX);

		// Change appearence of the Main Window
		DWORD dwStyle = GetWindowLongW(hWnd, GWL_STYLE);
		DWORD dwRemove = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		DWORD dwNewStyle = dwStyle & ~dwRemove;
		HDC hdcMonitor = GetWindowDC(NULL);
		SetWindowLongW(hWnd, GWL_STYLE, dwNewStyle);

		// Set position of the main window
		SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
		SetWindowPos(hWnd, NULL, 0, 0, GetDeviceCaps(hdcMonitor, HORZRES), GetDeviceCaps(hdcMonitor, VERTRES), SWP_FRAMECHANGED);

		ReleaseDC(NULL, hdcMonitor);
	}
}

void MainGUI::Paint(HWND &hWnd)
{
	PAINTSTRUCT     ps;
	HDC             hdc;
	BITMAP          BkgBitmap;
	BITMAP          WndBitmap;
	int				horizontal;
	int				vertical;

	gr7::GetDesktopResolution(horizontal, vertical);
	hdc = BeginPaint(hWnd, &ps);
	SetStretchBltMode(hdc, HALFTONE);

	// Draw Background Image
	HDC hdcBkgMem = CreateCompatibleDC(hdc);
	HBITMAP oldBkgBitmap = (HBITMAP)SelectObject(hdcBkgMem, BitmapObjects.hBackground);
	GetObjectW(BitmapObjects.hBackground, sizeof(BkgBitmap), &BkgBitmap);
	StretchBlt(hdc, 0, 0, horizontal, vertical, hdcBkgMem, 0, 0, BkgBitmap.bmWidth, BkgBitmap.bmHeight, SRCCOPY);
	SelectObject(hdcBkgMem, oldBkgBitmap);

	DeleteDC(hdcBkgMem);
	DeleteObject(oldBkgBitmap);

	// Draw Fake Window
	GetObjectW(BitmapObjects.hFakeWindow, sizeof(WndBitmap), &WndBitmap);
	int xPos = (horizontal - WndBitmap.bmWidth) / 2;
	int yPos = (vertical - WndBitmap.bmHeight) / 2;
	gr7::PaintTransparentBitmap(hdc, xPos, yPos - 33, BitmapObjects.hFakeWindow, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });

	// Draw Small Logo
	gr7::PaintTransparentBitmap(hdc, xPos + 56, yPos + 26 - 33, BitmapObjects.hSmallLogo, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });

	// Draw Title Text
	gr7::PaintText(hdc, xPos + 56 + 23, yPos + 26 - 33, L"Segoe UI", RGB(0, 0, 0), AppResStringsObjects.TitleBarText.c_str(), 9, 1, TRANSPARENT, FW_LIGHT);

	// Paint bottom progress bar text
	int hoz, ver;
	gr7::GetDesktopResolution(hoz, ver);
	gr7::PaintText(hdc, 37, ver - 42, L"Segoe UI", RGB(255, 255, 255), AppResStringsObjects.ProgressBarText1.c_str(), 9, 1, TRANSPARENT, FW_LIGHT);
	gr7::PaintText(hdc, 228, ver - 42, L"Segoe UI", RGB(255, 255, 255), AppResStringsObjects.ProgressBarText2.c_str(), 9, 1, TRANSPARENT, FW_LIGHT);
	gr7::PaintText(hdc, 6, ver - 47, L"Segoe UI", RGB(255, 255, 255), L"1", 25, 1, TRANSPARENT, FW_LIGHT);
	gr7::PaintText(hdc, 200, ver - 47, L"Segoe UI", RGB(255, 255, 255), L"2", 25, 1, TRANSPARENT, FW_LIGHT);

#ifdef _DEBUG
	gr7::PaintText(hdc, 0, 0, L"Segoe UI", RGB(255, 255, 255), L"Debug Build", 9, 1, TRANSPARENT, FW_LIGHT);
#endif
	EndPaint(hWnd, &ps);
}

// Main Window Window Procedure
LRESULT CALLBACK MainGUI::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
		case WM_CLOSE:
		{
			if (MainObjects.doNotClose) {
				return 0;
			}
			DestroyWindow(MainObjects.hWndDialogWindow);
			DestroyWindow(MainObjects.hWndSetupWindow);
			DestroyWindow(hWnd);
		}
		break;
		case WM_COMMAND:
			wmId = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			switch (wmId)
			{
				case IDM_EXIT:
					::DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProcW(hWnd, message, wParam, lParam);
			}
			break;

		case WM_PAINT:
			{
				MainGUI::Paint(hWnd);
			}
			break;

		// Calls the function to create the progress bar
		case MAINWND_CREATE_PROG_BAR:
			ProgressGUI::createProgressBar();
			break;

		// Calls the function to update the Collecting Information Progress Bar
		case MAINWND_UPDATE_COLLECT_INFO_PROG_BAR:
			ProgressGUI::updateProgressBar(hWnd, ProgressBarObjects.hProgressCtrlCollectingInfo, ProgressBarObjects.CollectingInfoPercentage);
			break;

		// Calls the function to update the Installing Progress Bar
		case MAINWND_UPDATE_INSTALLING_PROG_BAR:
			ProgressGUI::updateProgressBar(hWnd, ProgressBarObjects.hProgressCtrlInstalling, ProgressBarObjects.InstallingPercentage);
			break;

		// Makes text color to be transparent on the main window
		case WM_CTLCOLORSTATIC:
			if ((HWND)lParam == GetDlgItem(hWnd, IDC_STATIC)) {
				SetBkMode((HDC)wParam, TRANSPARENT);
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				return (INT_PTR)GetStockObject(NULL_BRUSH);
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}