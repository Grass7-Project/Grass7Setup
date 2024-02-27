#include "stdafx.h"
#include "SetupGUI.h"
#include "DialogGUI.h"
#include "ProgressGUI.h"
#include "InstallCode.h"
#include "ButtonGUI.h"
#include "RestartCode.h"

void SetupGUI::Init(HWND &hWnd, HWND &hWndParent, HINSTANCE &hInst)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = SetupGUI::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = MainObjects.hInst;
	wcex.hIcon = LoadIconW(MainObjects.hInst, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"SetupWindow";
	wcex.hIconSm = LoadIconW(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	hWnd = CreateWindowW(
		L"SetupWindow",
		AppResStringsObjects.AppTitleText.c_str(),
		WS_CHILD | WS_VISIBLE | WS_SYSMENU | DS_FIXEDSYS,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		622,
		428,
		hWndParent,
		NULL,
		hInst,
		NULL);

	if (!hWnd) {
		ErrorHandler::InvokeErrorHandler(1, 0, L"Failed to create Setup Window", AppResStringsObjects.AppTitleText);
	}
	else {
		RECT rc;

		GetWindowRect(hWnd, &rc);

		int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
		int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

		// Set position of the setup window
		SetWindowPos(hWnd, 0, xPos - 3, yPos + 14 - 33, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		SetWindowLongW(hWnd, GWL_STYLE, GetWindowLongW(hWnd, GWL_STYLE)&~WS_SIZEBOX);
	}
}

// Setup Window Window Procedure
LRESULT CALLBACK SetupGUI::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_CREATE:
	{
		ButtonGUI::InitButtonBitmaps();
	}
	break;
	case WM_CLOSE:
	{
		if (MainObjects.doNotClose) {
			return 0;
		}
		::DestroyWindow(MainObjects.hWndDialogWindow);
		::DestroyWindow(hWnd);
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
		PAINTSTRUCT     ps;
		HDC             hdc;
		{
			hdc = BeginPaint(hWnd, &ps);
			Grass7API::Paint::PaintTransparentBitmap(hdc, 0, 382, BitmapObjects.hBottomPanel, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
			EndPaint(hWnd, &ps);
		}
		break;

		// This code updates the dialog to show a new page
	case SETUPWND_UPDATE_DIALOG:
	{
		DialogGUI::Update();
	}
	break;

	// Installation finished
	case SETUPWND_INSTALL_FINISH:
	{
		MainObjects.doNotClose = 0;
		MainObjects.Page = 6;
		EnableMenuItem(GetSystemMenu(MainObjects.hWndSetupWindow, 1), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		::SendMessageW(MainObjects.hWndSetupWindow, SETUPWND_UPDATE_DIALOG, (WPARAM)(INT)0, 0);
	}
	break;

	case SETUPWND_RESTARTING_TIMER:
		ProgressGUI::updateProgressBar(MainObjects.hWndSetupWindow, ProgressBarObjects.hProgressCtrlRestarting, ProgressBarObjects.RestartingPercentage);
		break;

	case WM_CTLCOLORDLG:
	{
		SetBkColor((HDC)wParam, RGB(0, 0, 0));
		return (INT_PTR)GetStockObject(NULL_BRUSH);
	}

	case WM_CTLCOLORSTATIC:
	{
		SetBkColor((HDC)wParam, RGB(255, 255, 255));
		return (INT_PTR)GetStockObject(NULL_BRUSH);
	}

	case WM_DESTROY:
		::DestroyWindow(MainObjects.hWndMainWindow);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}