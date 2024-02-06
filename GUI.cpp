#include "stdafx.h"
#include "GUI.h"
#include "Global.h"
#include "ProgressBar.h"
#include "InstallCode.h"
#include "ButtonGUI.h"
#include "PartitionCode.h"
#include <Uxtheme.h>
#include <gdiplus.h>

GUI GUIObj;

// Main GUI Code
BOOL GUI::InitInstance()
{
	MainObjects.hWndMainWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_PARENTPAGE), 0, (DLGPROC)GUI::WndProc);

	EnableMenuItem(GetSystemMenu(MainObjects.hWndMainWindow, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	SetWindowLongW(MainObjects.hWndMainWindow, GWL_STYLE, GetWindowLongW(MainObjects.hWndMainWindow, GWL_STYLE) & ~WS_MINIMIZEBOX);

	DWORD dwStyle = GetWindowLongW(MainObjects.hWndMainWindow, GWL_STYLE);
	DWORD dwRemove = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	DWORD dwNewStyle = dwStyle & ~dwRemove;
	HDC hDC = GetWindowDC(NULL);
	SetWindowLongW(MainObjects.hWndMainWindow, GWL_STYLE, dwNewStyle);
	SetWindowPos(MainObjects.hWndMainWindow, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	SetWindowPos(MainObjects.hWndMainWindow, NULL, 0, 0, GetDeviceCaps(hDC, HORZRES), GetDeviceCaps(hDC, VERTRES), SWP_FRAMECHANGED);

	if (!MainObjects.hWndMainWindow) {
		return 0;
	}

	ButtonObjects.InstallButtonText = FALSE;

	MainObjects.hWndSetupWindow = CreateWindowW(L"SetupWindow", AppResStringsObjects.AppTitleText, WS_CHILD | WS_VISIBLE | WS_SYSMENU | DS_FIXEDSYS, CW_USEDEFAULT, CW_USEDEFAULT, 622, 428, MainObjects.hWndMainWindow, NULL, MainObjects.hInst, NULL);

	RECT rc;

	GetWindowRect(MainObjects.hWndSetupWindow, &rc);

	int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

	RichEditControlObjects.RichEditCtrlX = xPos - 3;
	RichEditControlObjects.RichEditCtrlY = yPos + 14;

	SetWindowPos(MainObjects.hWndSetupWindow, 0, xPos - 3, yPos + 14 - 33, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	SetWindowLongW(MainObjects.hWndSetupWindow, GWL_STYLE, GetWindowLongW(MainObjects.hWndSetupWindow, GWL_STYLE)&~WS_SIZEBOX);

	// Show the Setup Window
	::ShowWindow(MainObjects.hWndSetupWindow, 1);
	::UpdateWindow(MainObjects.hWndSetupWindow);

	// Show the Main Window
	::ShowWindow(MainObjects.hWndMainWindow, 1);
	::UpdateWindow(MainObjects.hWndMainWindow);

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

// Load strings
void GUI::LoadStrings()
{
	// Basic application strings
	AppResStringsObjects.AppTitleText = gr7::LoadStringToW(MainObjects.hInst, IDS_APP_TITLE);
	AppResStringsObjects.TitleBarText = gr7::LoadStringToW(MainObjects.hInst, IDS_TITLEBAR);

	// Dialog title strings
	AppResStringsObjects.EulaTitleText = gr7::LoadStringToW(MainObjects.hInst, IDS_EULA_TITLE);
	AppResStringsObjects.ChangelogTitleText = gr7::LoadStringToW(MainObjects.hInst, IDS_CHANGELOG_TITLE);
	AppResStringsObjects.PartitionTitleText = gr7::LoadStringToW(MainObjects.hInst, IDS_PARTITION_TITLE);
	AppResStringsObjects.InstallingTitleText = gr7::LoadStringToW(MainObjects.hInst, IDS_INSTALLING_TITLE);
	AppResStringsObjects.RestartingTitleText = gr7::LoadStringToW(MainObjects.hInst, IDS_RESTARTING_TITLE);

	// Button strings
	AppResStringsObjects.NextButtonText = gr7::LoadStringToW(MainObjects.hInst, IDS_NEXTBTN);
	AppResStringsObjects.InstallButtonText = gr7::LoadStringToW(MainObjects.hInst, IDS_INSTALLBTN);
	AppResStringsObjects.AutoPartButtonText = gr7::LoadStringToW(MainObjects.hInst, IDS_AUTOPARTBTN);
	AppResStringsObjects.ManualPartButtonText = gr7::LoadStringToW(MainObjects.hInst, IDS_MANUALPARTBTN);

	// Progress Bar strings
	AppResStringsObjects.ProgressBarText1 = gr7::LoadStringToW(MainObjects.hInst, IDS_PROGBAR_TEXT1);
	AppResStringsObjects.ProgressBarText2 = gr7::LoadStringToW(MainObjects.hInst, IDS_PROGBAR_TEXT2);

	// Installation progress strings
	AppResStringsObjects.CollectingInfoText = gr7::LoadStringToW(MainObjects.hInst, IDS_COLLECTING_INFO);
	AppResStringsObjects.CopyingFilesText = gr7::LoadStringToW(MainObjects.hInst, IDS_COPYING_FILES);
	AppResStringsObjects.ExpandingFilesText = gr7::LoadStringToW(MainObjects.hInst, IDS_EXPANDING_FILES);
	AppResStringsObjects.InstallingFeaturesText = gr7::LoadStringToW(MainObjects.hInst, IDS_INSTALLING_FEATURES);
	AppResStringsObjects.InstallingUpdatesText = gr7::LoadStringToW(MainObjects.hInst, IDS_INSTALLING_UPDATES);

	// Error strings
	AppResStringsObjects.CompatibilityErrorText = gr7::LoadStringToW(MainObjects.hInst, IDS_COMPATIBILITY_ERR);
	AppResStringsObjects.RunInWinPERequiredErrorText = gr7::LoadStringToW(MainObjects.hInst, IDS_WINPE_REQUIRED_ERR);
	AppResStringsObjects.NoInstallImageFoundErrorText = gr7::LoadStringToW(MainObjects.hInst, IDS_NO_INSTALL_IMG_ERR);
	AppResStringsObjects.ApplyInstallImageErrorText = gr7::LoadStringToW(MainObjects.hInst, IDS_IMG_APPLY_ERR);
}

// Window Classes are registered over here
ATOM GUI::RegisterClasses()
{
	// Main Window
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = GUI::WndProc;
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
	wcex1.lpfnWndProc = GUI::WndProcSetupWnd;
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
	wcex2.lpfnWndProc = GUI::WndProcDialogWnd;
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

// Code to get the resolution of the screen
void GUI::GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;

	const HWND hDesktop = GetDesktopWindow();

	GetWindowRect(hDesktop, &desktop);

	horizontal = desktop.right;
	vertical = desktop.bottom;
}

void GUI::RestartSoon()
{
	Sleep(5000);
	SendMessageW(MainObjects.hWndMainWindow, WM_CLOSE, (WPARAM)(INT)0, 0);
}

void GUI::DialogPaintCode()
{
	// Welcome Page
	if (MainObjects.Page == 1) {
		// Draw Logo Text
		HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
		gr7::PaintTransparentBitmap(hdc, 0, (428 / 2) - 72, GUIObj.hBanner, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
		ReleaseDC(MainObjects.hWndSetupWindow, hdc);
	}
	// License Page
	if (MainObjects.Page == 2) {
		// Draw Dialog Title Text
		HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
		gr7::PaintText(hdc, 43, 22, L"Segoe UI", RGB(0, 105, 51), AppResStringsObjects.EulaTitleText, 12, 1);
		ReleaseDC(MainObjects.hWndSetupWindow, hdc);

		::UpdateWindow(MainObjects.hWndSetupWindow);
	}
	// Changelog Page
	if (MainObjects.Page == 3) {
		// Draw Dialog Title Text
		HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
		gr7::PaintText(hdc, 43, 22, L"Segoe UI", RGB(0, 105, 51), AppResStringsObjects.ChangelogTitleText, 12, 1);
		ReleaseDC(MainObjects.hWndSetupWindow, hdc);

		::UpdateWindow(MainObjects.hWndSetupWindow);
	}
	// Partition Page
	if (MainObjects.Page == 4) {
		// Draw Dialog Title Text
		HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
		gr7::PaintText(hdc, 43, 22, L"Segoe UI", RGB(0, 105, 51), AppResStringsObjects.PartitionTitleText, 12, 1);
		int nHeightFont = -MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		ReleaseDC(MainObjects.hWndSetupWindow, hdc);

		::UpdateWindow(MainObjects.hWndSetupWindow);

		/*HFONT hBtnFont = CreateFontW(nHeightFont, 0, 0, 0, FW_LIGHT, 0, 0, 0, 0, 0, 0, 2, 0, L"Segoe UI");

		ButtonObjects.hAutoPartitionBtn = CreateWindowW(L"BUTTON", AppResStringsObjects.AutoPartButtonText, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 18, 338, 140, 26, MainObjects.hWndDialogWindow, (HMENU)IDAUTOMATICPART, MainObjects.hInst, NULL);
		ButtonObjects.hManualPartitionBtn = CreateWindowW(L"BUTTON", AppResStringsObjects.ManualPartButtonText, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 338, 123, 26, MainObjects.hWndDialogWindow, (HMENU)IDMANUALPART, MainObjects.hInst, NULL);
		SendMessage(ButtonObjects.hAutoPartitionBtn, WM_SETFONT, (LPARAM)hBtnFont, TRUE);
		SendMessage(ButtonObjects.hManualPartitionBtn, WM_SETFONT, (LPARAM)hBtnFont, TRUE);
		SetWindowTheme(ButtonObjects.hAutoPartitionBtn, L"", L"");
		SetWindowTheme(ButtonObjects.hManualPartitionBtn, L"", L""); */
	}
	// Installing Page
	if (MainObjects.Page == 5) {
		// Draw Dialog Title Text
		HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
		gr7::PaintText(hdc, 43, 22, L"Segoe UI", RGB(0, 105, 51), AppResStringsObjects.InstallingTitleText, 12, 1);
		ReleaseDC(MainObjects.hWndSetupWindow, hdc);
		ProgressBar::createProgressText();

		::UpdateWindow(MainObjects.hWndSetupWindow);
	}
	// Restarting Page
	if (MainObjects.Page == 6) {
		// Draw Dialog Title Text
		HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
		gr7::PaintText(hdc, 43, 22, L"Segoe UI", RGB(0, 105, 51), AppResStringsObjects.RestartingTitleText, 12, 1);
		ReleaseDC(MainObjects.hWndSetupWindow, hdc);

		::UpdateWindow(MainObjects.hWndSetupWindow);
	}

	HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
	gr7::PaintTransparentBitmap(hdc, 0, 382, GUIObj.hBottomPanel, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(MainObjects.hWndSetupWindow, hdc);

	::SendMessageW(ButtonObjects.hNormalBtn, BTN_UPDATE, (WPARAM)(INT)0, 0);
}

// Main Window Window Procedure
LRESULT CALLBACK GUI::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
		case WM_INITDIALOG:
		{
			GUIObj.hBackground = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_BACKGROUND_BMP), IMAGE_BITMAP, 0, 0, 0));
			GUIObj.hFakeWindow = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_FAKEWND_BMP), IMAGE_BITMAP, 0, 0, 0));
			GUIObj.hBanner = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_BANNER_BMP), IMAGE_BITMAP, 0, 0, 0));
			GUIObj.hSmallLogo = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_SMALLLOGO_BMP), IMAGE_BITMAP, 0, 0, 0));
			GUIObj.hBottomPanel = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_BOTTOM_PANEL_BMP), IMAGE_BITMAP, 0, 0, 0));
		}
		break;
		case WM_CLOSE:
		{
			if (GUIObj.doNotClose == TRUE) {
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
			PAINTSTRUCT     ps;
			HDC             hdc;
			BITMAP          BkgBitmap;
			BITMAP          WndBitmap;
			int				horizontal;
			int				vertical;
			{
				GUI::GetDesktopResolution(horizontal, vertical);
				hdc = BeginPaint(hWnd, &ps);
				SetStretchBltMode(hdc, HALFTONE);

				// Draw Background Image
				HDC hdcBkgMem = CreateCompatibleDC(hdc);
				HGDIOBJ oldBkgBitmap = (HBITMAP)SelectObject(hdcBkgMem, GUIObj.hBackground);
				GetObjectW(GUIObj.hBackground, sizeof(BkgBitmap), &BkgBitmap);
				StretchBlt(hdc, 0, 0, horizontal, vertical, hdcBkgMem, 0, 0, BkgBitmap.bmWidth, BkgBitmap.bmHeight, SRCCOPY);
				SelectObject(hdcBkgMem, oldBkgBitmap);

				// Draw Fake Window
				HDC hdcWndMem = CreateCompatibleDC(hdc);
				GetObjectW(GUIObj.hFakeWindow, sizeof(WndBitmap), &WndBitmap);
				int xPos = (horizontal - WndBitmap.bmWidth) / 2;
				int yPos = (vertical - WndBitmap.bmHeight) / 2;
				gr7::PaintTransparentBitmap(hdc, xPos, yPos - 33, GUIObj.hFakeWindow, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
				
				// Draw Small Logo
				gr7::PaintTransparentBitmap(hdc, xPos + 56, yPos + 26 - 33, GUIObj.hSmallLogo, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
				
				// Draw Title Text
				gr7::PaintText(hdc, xPos + 56 + 23, yPos + 26 - 33, L"Segoe UI", RGB(0, 0, 0), AppResStringsObjects.TitleBarText, 9, 1);

				DeleteDC(hdcWndMem);
				DeleteDC(hdcBkgMem);
				EndPaint(hWnd, &ps);
			}
			break;

		// Calls the function to create the progress bar
		case MAINWND_CREATE_PROG_BAR:
			ProgressBar::createProgressBar();
			break;

		// Calls the function to update the progress bar
		case MAINWND_UPDATE_PROG_BAR:
			ProgressBar::updateProgressBar();
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
LRESULT CALLBACK GUI::WndProcSetupWnd(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
				if (GUIObj.doNotClose == TRUE) {
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
				EndPaint(hWnd, &ps);
			}
			break;

		// This code updates the dialog to show a new page
		case SETUPWND_UPDATE_DIALOG:
			{
				::ShowWindow(MainObjects.hWndDialogWindow, 0);

				// Welcome Page
				if (MainObjects.Page == 1) {
					::SendMessageW(ButtonObjects.hAutoPartitionBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
					::SendMessageW(ButtonObjects.hManualPartitionBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
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
					wchar_t file[MAX_PATH];
					wcsncpy_s(file, ImageInstallObjects.installSources, sizeof(file));
					wcsncat_s(file, L"\\license.rtf", sizeof(file));

					RichEditControlObjects.hWndRichEditCtrl = gr7::CreateRichEdit(MainObjects.hWndDialogWindow, 42, 62, 543, 272, MainObjects.hInst);
					gr7::FillRichEditFromFile(RichEditControlObjects.hWndRichEditCtrl, file, SF_RTF);

					memset(file, 0, sizeof(file));
				}

				// Changelog Page
				if (MainObjects.Page == 3) {
					::SendMessageW(ButtonObjects.hAutoPartitionBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
					::SendMessageW(ButtonObjects.hManualPartitionBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
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

					wchar_t file[MAX_PATH];
					wcsncpy_s(file, ImageInstallObjects.installSources, sizeof(file));
					wcsncat_s(file, L"\\changelog.rtf", sizeof(file));

					RichEditControlObjects.hWndRichEditCtrl = gr7::CreateRichEdit(MainObjects.hWndDialogWindow, 42, 62, 543, 272, MainObjects.hInst);
					gr7::FillRichEditFromFile(RichEditControlObjects.hWndRichEditCtrl, file, SF_RTF);

					memset(file, 0, sizeof(file));
				}

				// Partition Page
				if (MainObjects.Page == 4) {
					::SendMessageW(ButtonObjects.hNormalBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
					MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_PARTITIONPAGE), MainObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);

					::SendMessageW(ButtonObjects.hAutoPartitionBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
					::SendMessageW(ButtonObjects.hManualPartitionBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
					::ShowWindow(ButtonObjects.hAutoPartitionBtn, TRUE);
					::ShowWindow(ButtonObjects.hManualPartitionBtn, TRUE);
				}

				// Installing Page
				if (MainObjects.Page == 5) {
					::SendMessageW(ButtonObjects.hAutoPartitionBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
					::SendMessageW(ButtonObjects.hManualPartitionBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
					::ShowWindow(ButtonObjects.hAutoPartitionBtn, FALSE);
					::ShowWindow(ButtonObjects.hManualPartitionBtn, FALSE);

					::SendMessageW(ButtonObjects.hNormalBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
					::SendMessageW(ButtonObjects.hBackBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
					::SendMessageW(ButtonObjects.hCloseBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
					::ShowWindow(ButtonObjects.hNormalBtn, FALSE);

					MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_INSTALLINGPAGE), MainObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);
					ProgressBarObjects.CollectingInfoPercentage = 100;
					ProgressBar::updateProgressBar();

					ProgressBarObjects.CollectingInfoPercentage = ProgressBarObjects.CollectingInfoPercentage + 1;
					::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
					GUIObj.doNotClose = 1;
					EnableMenuItem(GetSystemMenu(MainObjects.hWndSetupWindow, 0), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
					Install::InstallMain();
				}

				// Restarting Page
				if (MainObjects.Page == 6) {
					::SendMessageW(ButtonObjects.hCloseBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
					MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_RESTARTINGPAGE), MainObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);
					std::thread Restart(GUI::RestartSoon);
					Restart.detach();
				}

				// We show the window and update it to see our dialog page
				::ShowWindow(MainObjects.hWndDialogWindow, 1);
				::UpdateWindow(MainObjects.hWndDialogWindow);

				//Activate custom paint code for dialogs
				GUI::DialogPaintCode();
			}
			break;

		// Installation finished
		case SETUPWND_INSTALL_FINISH:
		{
			GUIObj.doNotClose = 0;
			MainObjects.Page = 6;
			EnableMenuItem(GetSystemMenu(MainObjects.hWndSetupWindow, 1), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			::SendMessageW(MainObjects.hWndSetupWindow, SETUPWND_UPDATE_DIALOG, (WPARAM)(INT)0, 0);
		}
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
LRESULT CALLBACK GUI::WndProcDialogWnd(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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