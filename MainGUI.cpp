#include "stdafx.h"
#include "MainGUI.h"
#include "ProgressGUI.h"
#include "InstallCode.h"
#include "ButtonGUI.h"
#include "PartitionCode.h"
#include "ResourceLoader.h"
#include "RestartCode.h"
#include <Uxtheme.h>
#include <gdiplus.h>

MainGUI MainGUIObj;

// Main GUI Code
BOOL MainGUI::InitInstance()
{
	// Call the function to register the classes for the windows
	MainGUI::RegisterClasses();

	// Call the function to load the bitmaps
	ResourceLoader::LoadBitmaps();

	// Note: the main window HAS to be a dialog, it is adjusted to look like a window before being shown
	// This is because of some code being broken and not working correctly if it was a real window like the setup window
	// I have no idea why, but since this is based on the old setup project before i built upon this, its gonna stay like that.

	// Create Main Window
	MainObjects.hWndMainWindow = CreateDialogW(
		MainObjects.hInst,
		MAKEINTRESOURCE(IDD_PARENTPAGE),
		0,
		(DLGPROC)MainGUI::WndProc);

	if (!MainObjects.hWndMainWindow) {
		ErrorHandler::InvokeErrorHandler(1, 0, L"Failed to create Main Window", AppResStringsObjects.AppTitleText);
	}

	EnableMenuItem(GetSystemMenu(MainObjects.hWndMainWindow, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	SetWindowLongW(MainObjects.hWndMainWindow, GWL_STYLE, GetWindowLongW(MainObjects.hWndMainWindow, GWL_STYLE) & ~WS_MINIMIZEBOX);

	// Change appearence of the Main Window
	DWORD dwStyle = GetWindowLongW(MainObjects.hWndMainWindow, GWL_STYLE);
	DWORD dwRemove = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	DWORD dwNewStyle = dwStyle & ~dwRemove;
	HDC hdcMonitor = GetWindowDC(NULL);
	SetWindowLongW(MainObjects.hWndMainWindow, GWL_STYLE, dwNewStyle);

	// Set position of the main window
	SetWindowPos(MainObjects.hWndMainWindow, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	SetWindowPos(MainObjects.hWndMainWindow, NULL, 0, 0, GetDeviceCaps(hdcMonitor, HORZRES), GetDeviceCaps(hdcMonitor, VERTRES), SWP_FRAMECHANGED);

	ReleaseDC(NULL, hdcMonitor);

	ButtonObjects.InstallButtonText = FALSE; // Set the install button text to not appear, currently

	// Create Setup Window
	MainObjects.hWndSetupWindow = CreateWindowW(
		L"SetupWindow",
		AppResStringsObjects.AppTitleText.c_str(),
		WS_CHILD | WS_VISIBLE | WS_SYSMENU | DS_FIXEDSYS, CW_USEDEFAULT,
		CW_USEDEFAULT,
		622,
		428,
		MainObjects.hWndMainWindow,
		NULL,
		MainObjects.hInst,
		NULL);

	if (!MainObjects.hWndSetupWindow) {
		ErrorHandler::InvokeErrorHandler(1, 0, L"Failed to create Setup Window", AppResStringsObjects.AppTitleText);
	}

	RECT rc;

	GetWindowRect(MainObjects.hWndSetupWindow, &rc);

	int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

	// Set position of the rich edit control
	RichEditControlObjects.RichEditCtrlX = xPos - 3;
	RichEditControlObjects.RichEditCtrlY = yPos + 14;

	// Set position of the setup window
	SetWindowPos(MainObjects.hWndSetupWindow, 0, xPos - 3, yPos + 14 - 33, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	SetWindowLongW(MainObjects.hWndSetupWindow, GWL_STYLE, GetWindowLongW(MainObjects.hWndSetupWindow, GWL_STYLE)&~WS_SIZEBOX);

	::ShowWindow(MainObjects.hWndSetupWindow, TRUE); // Show the Setup Window
	::UpdateWindow(MainObjects.hWndSetupWindow); // Update the Setup Window to trigger a redraw

	::ShowWindow(MainObjects.hWndMainWindow, TRUE); // Show the Main Window
	::UpdateWindow(MainObjects.hWndMainWindow); // Update the Main Window to trigger a redraw

	// We init the custom button code
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

// Window Classes are registered over here
ATOM MainGUI::RegisterClasses()
{
	// Main Window
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

	// Setup Window
	WNDCLASSEX wcex1;

	wcex1.cbSize = sizeof(wcex1);
	wcex1.style = CS_HREDRAW | CS_VREDRAW;
	wcex1.lpfnWndProc = MainGUI::WndProcSetupWnd;
	wcex1.cbClsExtra = 0;
	wcex1.cbWndExtra = 0;
	wcex1.hInstance = MainObjects.hInst;
	wcex1.hIcon = LoadIconW(MainObjects.hInst, MAKEINTRESOURCE(IDI_ICON));
	wcex1.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wcex1.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex1.lpszMenuName = NULL;
	wcex1.lpszClassName = L"SetupWindow";
	wcex1.hIconSm = LoadIconW(wcex1.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	// Dialog Window
	WNDCLASSEX wcex2;

	wcex2.cbSize = sizeof(wcex2);
	wcex2.style = CS_HREDRAW | CS_VREDRAW;
	wcex2.lpfnWndProc = MainGUI::WndProcDialogWnd;
	wcex2.cbClsExtra = 0;
	wcex2.cbWndExtra = 0;
	wcex2.hInstance = MainObjects.hInst;
	wcex2.hIcon = LoadIconW(MainObjects.hInst, MAKEINTRESOURCE(IDI_ICON));
	wcex2.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wcex2.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex2.lpszMenuName = NULL;
	wcex2.lpszClassName = L"DialogWindow";
	wcex2.hIconSm = LoadIconW(wcex2.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex), RegisterClassExW(&wcex1), RegisterClassExW(&wcex2);
}

void MainGUI::MainWindowPaintCode(HWND &hWnd)
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
	HGDIOBJ oldBkgBitmap = (HBITMAP)SelectObject(hdcBkgMem, BitmapObjects.hBackground);
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

void MainGUI::Exit()
{
	if (MainGUIObj.doNotClose) {
		MessageBoxW(NULL, AppResStringsObjects.SetupExitDuringSetup.c_str(), AppResStringsObjects.AppTitleText.c_str(), MB_ICONERROR | MB_OK);
	}

	if (!MainGUIObj.doNotClose) {
		const int result = MessageBoxW(NULL, AppResStringsObjects.SetupExit.c_str(), AppResStringsObjects.AppTitleText.c_str(), MB_ICONERROR | MB_YESNO);
		switch (result)
		{
		case IDYES:
			SendMessageW(MainObjects.hWndMainWindow, WM_CLOSE, (WPARAM)(INT)0, 0);
			break;
		}
	}
}

void MainGUI::DialogUpdateCode()
{
	::ShowWindow(MainObjects.hWndDialogWindow, 0);

	// Welcome Page
	if (MainObjects.Page == 1) {
		::SendMessageW(ButtonObjects.hAutoPartitionBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
		::SendMessageW(ButtonObjects.hManualPartitionBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
		::ShowWindow(ButtonObjects.hAutoPartitionBtn, FALSE);
		::ShowWindow(ButtonObjects.hManualPartitionBtn, FALSE);

		MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_WELCOMEPAGE), MainObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);
		::SendMessageW(ButtonObjects.hBackBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
		if (ButtonObjects.NormalButtonState == 3) {
			::SendMessageW(ButtonObjects.hNormalBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
		}

		if (RichEditControlObjects.hWndRichEditCtrl != NULL)
		{
			DestroyWindow(RichEditControlObjects.hWndRichEditCtrl);
		}
	}

	// License Page
	if (MainObjects.Page == 2) {
		if (ButtonObjects.BackButtonDisabled == FALSE) {
			::SendMessageW(ButtonObjects.hBackBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
		}
		MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_LICENSEPAGE), MainObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);

		::SendMessageW(ButtonObjects.hNormalBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);

		// Show license
		std::wstring file = ImageInstallObjects.installSources;
		file.append(L"\\license.rtf");

		RichEditControlObjects.hWndRichEditCtrl = gr7::CreateRichEdit(MainObjects.hWndDialogWindow, 42, 62, 543, 272, MainObjects.hInst);
		gr7::FillRichEditFromFile(RichEditControlObjects.hWndRichEditCtrl, file.c_str(), SF_RTF);
		::SendMessageW(RichEditControlObjects.hWndRichEditCtrl, EM_SETREADONLY, TRUE, 0);
	}

	// Changelog Page
	if (MainObjects.Page == 3) {
		::ShowWindow(ButtonObjects.hAutoPartitionBtn, FALSE);
		::ShowWindow(ButtonObjects.hManualPartitionBtn, FALSE);

		if (ButtonObjects.NormalButtonState == 3) {
			::SendMessageW(ButtonObjects.hNormalBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
		}

		if (RichEditControlObjects.hWndRichEditCtrl != NULL)
		{
			DestroyWindow(RichEditControlObjects.hWndRichEditCtrl);
		}
		if (ButtonObjects.BackButtonDisabled == TRUE) {
			::SendMessageW(ButtonObjects.hBackBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
		}

		MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_CHANGELOGPAGE), MainObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);

		std::wstring file = ImageInstallObjects.installSources;
		file.append(L"\\changelog.rtf");

		RichEditControlObjects.hWndRichEditCtrl = gr7::CreateRichEdit(MainObjects.hWndDialogWindow, 42, 62, 543, 272, MainObjects.hInst);
		gr7::FillRichEditFromFile(RichEditControlObjects.hWndRichEditCtrl, file.c_str(), SF_RTF);
		::SendMessageW(RichEditControlObjects.hWndRichEditCtrl, EM_SETREADONLY, TRUE, 0);
	}

	// Partition Page
	if (MainObjects.Page == 4) {
		if (ImageInstallObjects.NoDeploy) {
			::SendMessageW(ButtonObjects.hNormalBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
			::SendMessageW(ButtonObjects.hAutoPartitionBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
			::SendMessageW(ButtonObjects.hManualPartitionBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
		}
		else {
			::SendMessageW(ButtonObjects.hNormalBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
		}

		MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_PARTITIONPAGE), MainObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);

		::ShowWindow(ButtonObjects.hAutoPartitionBtn, TRUE);
		::ShowWindow(ButtonObjects.hManualPartitionBtn, TRUE);
	}

	// Installing Page
	if (MainObjects.Page == 5) {
		::ShowWindow(ButtonObjects.hAutoPartitionBtn, FALSE);
		::ShowWindow(ButtonObjects.hManualPartitionBtn, FALSE);
		::ShowWindow(ButtonObjects.hNormalBtn, FALSE);
		::ShowWindow(ButtonObjects.hBackBtn, FALSE);

		MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_INSTALLINGPAGE), MainObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);
	}

	// Restarting Page
	if (MainObjects.Page == 6) {
		::ShowWindow(ButtonObjects.hCloseBtn, FALSE);
		::ShowWindow(ProgressBarObjects.hProgressCtrlRestarting, TRUE);
		MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_RESTARTINGPAGE), MainObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);
	}

	if (!MainObjects.hWndDialogWindow) {
		ErrorHandler::InvokeErrorHandler(1, 0, L"Failed to create Dialog Window", AppResStringsObjects.AppTitleText);
	}

	// We show the window and update it to see our dialog page
	::ShowWindow(MainObjects.hWndDialogWindow, TRUE);
	::UpdateWindow(MainObjects.hWndDialogWindow);

	::SendMessageW(ButtonObjects.hNormalBtn, BTN_UPDATE, (WPARAM)(INT)0, 0);
}

void MainGUI::DialogPaintCode(HDC &hdc)
{
	// Text painting options
	int xPos = 43;
	int yPos = 22;
	COLORREF color = RGB(0, 105, 51);
	int nSize = 12;

	// Welcome Page
	if (MainObjects.Page == 1) {
		// Draw Logo Text
		gr7::PaintTransparentBitmap(hdc, 0, (428 / 2) - 72, BitmapObjects.hBanner, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	}

	// License Page
	if (MainObjects.Page == 2) {
		// Draw Dialog Title Text
		gr7::PaintText(hdc, xPos, yPos, L"Segoe UI", RGB(0, 105, 51), AppResStringsObjects.EulaTitleText.c_str(), nSize, 1, TRANSPARENT, FW_LIGHT);
	}

	// Changelog Page
	if (MainObjects.Page == 3) {
		// Draw Dialog Title Text
		gr7::PaintText(hdc, xPos, yPos, L"Segoe UI", RGB(0, 105, 51), AppResStringsObjects.ChangelogTitleText.c_str(), nSize, 1, TRANSPARENT, FW_LIGHT);
	}

	// Partition Page
	if (MainObjects.Page == 4) {
		// Draw Dialog Title Text
		gr7::PaintText(hdc, xPos, yPos, L"Segoe UI", RGB(0, 105, 51), AppResStringsObjects.PartitionTitleText.c_str(), nSize, 1, TRANSPARENT, FW_LIGHT);
		int nHeightFont = -MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	}

	// Installing Page
	if (MainObjects.Page == 5) {
		// Draw Dialog Title Text
		gr7::PaintText(hdc, xPos, yPos, L"Segoe UI", RGB(0, 105, 51), AppResStringsObjects.InstallingTitleText.c_str(), nSize, 1, TRANSPARENT, FW_LIGHT);
		ProgressBarObjects.CollectingInfoPercentage = 100;
		ProgressBarObjects.CollectingInfoPercentage = ProgressBarObjects.CollectingInfoPercentage + 1;
		::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_COLLECT_INFO_PROG_BAR, (WPARAM)(INT)0, 0);
		MainGUIObj.doNotClose = 1;
		EnableMenuItem(GetSystemMenu(MainObjects.hWndSetupWindow, 0), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		ProgressGUI::createProgressText(hdc);
		::UpdateWindow(MainObjects.hWndSetupWindow);
		Install::InstallMain();
	}

	// Restarting Page
	if (MainObjects.Page == 6) {
		// Draw Dialog Title Text
		gr7::PaintText(hdc, xPos, yPos, L"Segoe UI", RGB(0, 105, 51), AppResStringsObjects.RestartingTitleText.c_str(), nSize, 1, TRANSPARENT, FW_LIGHT);
		Restart::InitiateRestart();
	}
}

// Main Window Window Procedure
LRESULT CALLBACK MainGUI::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
		case WM_CLOSE:
		{
			if (MainGUIObj.doNotClose) {
				return 0;
			}
			DestroyWindow(MainObjects.hWndDialogWindow);
			DestroyWindow(MainObjects.hWndSetupWindow);
			DestroyWindow(MainObjects.hWndMainWindow);
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
				MainGUI::MainWindowPaintCode(hWnd);
			}
			break;

		// Calls the function to create the progress bar
		case MAINWND_CREATE_PROG_BAR:
			ProgressGUI::createProgressBar();
			break;

		// Calls the function to update the Collecting Information Progress Bar
		case MAINWND_UPDATE_COLLECT_INFO_PROG_BAR:
			ProgressGUI::updateProgressBar(MainObjects.hWndMainWindow, ProgressBarObjects.hProgressCtrlCollectingInfo, ProgressBarObjects.CollectingInfoPercentage);
			break;

		// Calls the function to update the Installing Progress Bar
		case MAINWND_UPDATE_INSTALLING_PROG_BAR:
			ProgressGUI::updateProgressBar(MainObjects.hWndMainWindow, ProgressBarObjects.hProgressCtrlInstalling, ProgressBarObjects.InstallingPercentage);
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

// Setup Window Window Procedure
LRESULT CALLBACK MainGUI::WndProcSetupWnd(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
				if (MainGUIObj.doNotClose) {
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
				gr7::PaintTransparentBitmap(hdc, 0, 382, BitmapObjects.hBottomPanel, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
				EndPaint(hWnd, &ps);
			}
			break;

		// This code updates the dialog to show a new page
		case SETUPWND_UPDATE_DIALOG:
			{
				MainGUI::DialogUpdateCode();
			}
			break;

		// Installation finished
		case SETUPWND_INSTALL_FINISH:
		{
			MainGUIObj.doNotClose = 0;
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

// Dialog Window Window Procedure
LRESULT CALLBACK MainGUI::WndProcDialogWnd(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
		case WM_COMMAND:
			wmId = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			switch (wmId)
			{
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;

			case ID_ACCEPT_LICENSE:
				if (IsDlgButtonChecked(hWnd, ID_ACCEPT_LICENSE) == BST_CHECKED) {
					::SendMessageW(ButtonObjects.hNormalBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
				}
				if (IsDlgButtonChecked(hWnd, ID_ACCEPT_LICENSE) == BST_UNCHECKED) {
					::SendMessageW(ButtonObjects.hNormalBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
				}
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
				gr7::PaintTransparentBitmap(hdc, 0, 382, BitmapObjects.hBottomPanel, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
				MainGUI::DialogPaintCode(hdc);
				EndPaint(hWnd, &ps);
			}
			break;

		// Set background color of the dialog
		case WM_CTLCOLORDLG:
		{
			SetBkColor((HDC)wParam, RGB(255, 255, 255));
			return (INT_PTR)GetStockObject(NULL_BRUSH);
		}

		// Set background color of the text and controls
		case WM_CTLCOLORSTATIC:
		{
			SetBkColor((HDC)wParam, RGB(255, 255, 255));
			return (INT_PTR)GetStockObject(WHITE_BRUSH);
		}

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}