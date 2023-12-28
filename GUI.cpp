#include "stdafx.h"
#include "GUI.h"
#include "Global.h"
#include "ProgressBar.h"
#include "InstallGrass7.h"
#include "ButtonGUI.h"
#include <gdiplus.h>

GUI GUIObj;

// Main GUI Code
BOOL GUI::InitInstance()
{
	GlobalObjects.hWndMainWindow = CreateDialogW(GlobalObjects.hInst, MAKEINTRESOURCE(IDD_WELCOMEPAGE), 0, (DLGPROC)GUI::WndProc);

	EnableMenuItem(GetSystemMenu(GlobalObjects.hWndMainWindow, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	SetWindowLongW(GlobalObjects.hWndMainWindow, GWL_STYLE, GetWindowLongW(GlobalObjects.hWndMainWindow, GWL_STYLE) & ~WS_MINIMIZEBOX);

	DWORD dwStyle = GetWindowLongW(GlobalObjects.hWndMainWindow, GWL_STYLE);
	DWORD dwRemove = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	DWORD dwNewStyle = dwStyle & ~dwRemove;
	HDC hDC = GetWindowDC(NULL);
	SetWindowLongW(GlobalObjects.hWndMainWindow, GWL_STYLE, dwNewStyle);
	SetWindowPos(GlobalObjects.hWndMainWindow, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	SetWindowPos(GlobalObjects.hWndMainWindow, NULL, 0, 0, GetDeviceCaps(hDC, HORZRES), GetDeviceCaps(hDC, VERTRES), SWP_FRAMECHANGED);

	if (!GlobalObjects.hWndMainWindow) {
		return 0;
	}

	GlobalObjects.InstallButtonText = FALSE;

	GlobalObjects.hWndSetupWindow = CreateWindowW(L"SetupWindow", gr7::LoadStringToW(GetModuleHandleW(NULL), IDS_APP_TITLE), WS_CHILD | WS_VISIBLE | WS_SYSMENU | DS_FIXEDSYS, CW_USEDEFAULT, CW_USEDEFAULT, 622, 428, GlobalObjects.hWndMainWindow, NULL, GlobalObjects.hInst, NULL);

	RECT rc;

	GetWindowRect(GlobalObjects.hWndSetupWindow, &rc);

	int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

	GlobalObjects.RichEditCtrlX = xPos - 3;
	GlobalObjects.RichEditCtrlY = yPos + 14;

	SetWindowPos(GlobalObjects.hWndSetupWindow, 0, xPos - 3, yPos + 14, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	SetWindowLongW(GlobalObjects.hWndSetupWindow, GWL_STYLE, GetWindowLongW(GlobalObjects.hWndSetupWindow, GWL_STYLE)&~WS_SIZEBOX);

	// Show the Main Window, and the Setup Window
	::ShowWindow(GlobalObjects.hWndSetupWindow, 1);
	::UpdateWindow(GlobalObjects.hWndSetupWindow);

	::ShowWindow(GlobalObjects.hWndMainWindow, 1);
	::UpdateWindow(GlobalObjects.hWndMainWindow);

	ButtonGUI::InitBackBtn();
	ButtonGUI::InitCloseBtn();
	ButtonGUI::InitNormalBtn();

	GlobalObjects.Page = 1;
	::SendMessageW(GlobalObjects.hWndSetupWindow, SETUPWND_UPDATE_DIALOG, (WPARAM)(INT)0, 0);

	::SendMessageW(GlobalObjects.hWndMainWindow, MAINWND_CREATE_PROG_BAR, (WPARAM)(INT)0, 0);

	return 1;
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
	wcex.hInstance = GlobalObjects.hInst;
	wcex.hIcon = LoadIconW(GlobalObjects.hInst, MAKEINTRESOURCE(IDI_ICON));
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
	wcex1.hInstance = GlobalObjects.hInst;
	wcex1.hIcon = LoadIconW(GlobalObjects.hInst, MAKEINTRESOURCE(IDI_ICON));
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
	wcex2.hInstance = GlobalObjects.hInst;
	wcex2.hIcon = LoadIconW(GlobalObjects.hInst, MAKEINTRESOURCE(IDI_ICON));
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
	SendMessageW(GlobalObjects.hWndMainWindow, WM_CLOSE, (WPARAM)(INT)0, 0);
}

// Main Window Window Procedure
LRESULT CALLBACK GUI::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
		case WM_INITDIALOG:
		{
			GUIObj.hBackground = static_cast<HBITMAP>(LoadImageW(GlobalObjects.hInst, MAKEINTRESOURCE(IDB_BACKGROUND_BMP), IMAGE_BITMAP, 0, 0, 0));
			GUIObj.hFakeWindow = static_cast<HBITMAP>(LoadImageW(GlobalObjects.hInst, MAKEINTRESOURCE(IDB_FAKEWND_BMP), IMAGE_BITMAP, 0, 0, 0));
			GUIObj.hBanner = static_cast<HBITMAP>(LoadImageW(GlobalObjects.hInst, MAKEINTRESOURCE(IDB_BANNER_BMP), IMAGE_BITMAP, 0, 0, 0));
			GUIObj.hSmallLogo = static_cast<HBITMAP>(LoadImageW(GlobalObjects.hInst, MAKEINTRESOURCE(IDB_SMALLLOGO_BMP), IMAGE_BITMAP, 0, 0, 0));
		}
		break;
		case WM_CLOSE:
		{
			if (GUIObj.doNotClose == TRUE) {
				return 0;
			}
			DestroyWindow(GlobalObjects.hWndDialogWindow);
			DestroyWindow(GlobalObjects.hWndSetupWindow);
			DestroyWindow(GlobalObjects.hWndMainWindow);
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
			BITMAP          SmallLogoBitmap;
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
				HGDIOBJ oldWndBitmap = (HBITMAP)SelectObject(hdcWndMem, GUIObj.hFakeWindow);
				GetObjectW(GUIObj.hFakeWindow, sizeof(WndBitmap), &WndBitmap);
				BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
				int xPos = (horizontal - WndBitmap.bmWidth) / 2;
				int yPos = (vertical - WndBitmap.bmHeight) / 2;
				HDC hdcMem02 = CreateCompatibleDC(hdc);
				AlphaBlend(hdc, xPos, yPos, WndBitmap.bmWidth, WndBitmap.bmHeight, hdcWndMem, 0, 0, WndBitmap.bmWidth, WndBitmap.bmHeight, bf);
				SelectObject(hdcWndMem, oldWndBitmap);

				
				// Draw Small Logo
				HDC hdcSmallLogoMem = CreateCompatibleDC(hdc);
				HGDIOBJ oldSmallLogoBitmap = (HBITMAP)SelectObject(hdcSmallLogoMem, GUIObj.hSmallLogo);
				GetObjectW(GUIObj.hSmallLogo, sizeof(SmallLogoBitmap), &SmallLogoBitmap);
				BLENDFUNCTION bf1 = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
				HDC hdcMem03 = CreateCompatibleDC(hdc);
				AlphaBlend(hdc, xPos + 56, yPos + 26, SmallLogoBitmap.bmWidth, SmallLogoBitmap.bmHeight, hdcSmallLogoMem, 0, 0, SmallLogoBitmap.bmWidth, SmallLogoBitmap.bmHeight, bf1);
				SelectObject(hdcSmallLogoMem, oldSmallLogoBitmap);

				
				// Draw Title Text
				wchar_t *text = gr7::LoadStringToW(GetModuleHandleW(NULL), IDS_TITLEBAR);
				SetBkMode(hdc, TRANSPARENT);
				SetTextColor(hdc, RGB(0, 0, 0));
				HFONT hFont, hTmp;
				int nHeight = -MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
				hFont = CreateFontW(nHeight, 0, 0, 0, FW_LIGHT, 0, 0, 0, 0, 0, 0, 2, 0, L"Segoe UI");
				hTmp = (HFONT)SelectObject(hdc, hFont);
				size_t size = wcslen(text);
				int convertsize = static_cast<int>(size);
				TextOutW(hdc, xPos + 56 + 24, yPos + 26, text, convertsize);

				DeleteDC(hdcSmallLogoMem);
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
				::DestroyWindow(GlobalObjects.hWndDialogWindow);
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
				::ShowWindow(GlobalObjects.hWndDialogWindow, 0);

				// Welcome Page
				if (GlobalObjects.Page == 1) {
					GlobalObjects.hWndDialogWindow = CreateDialogW(GlobalObjects.hInst, MAKEINTRESOURCE(IDD_PAGE1), GlobalObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);
					::SendMessageW(GlobalObjects.hBackBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
					if (GlobalObjects.NormalButtonState == 3) {
						::SendMessageW(GlobalObjects.hNormalBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
					}

					// Draw Logo Text
					BITMAP BannerBitmap;
					HDC hdc = ::GetDC(GlobalObjects.hWndSetupWindow);
					HDC hdcBannerMem = CreateCompatibleDC(hdc);
					HGDIOBJ oldBannerBitmap = (HBITMAP)SelectObject(hdcBannerMem, GUIObj.hBanner);
					GetObjectW(GUIObj.hBanner, sizeof(BannerBitmap), &BannerBitmap);
					BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
					AlphaBlend(hdc, 0, (428 / 2) - 72, BannerBitmap.bmWidth, BannerBitmap.bmHeight, hdcBannerMem, 0, 0, BannerBitmap.bmWidth, BannerBitmap.bmHeight, bf);
					SelectObject(hdcBannerMem, oldBannerBitmap);
					DeleteDC(hdcBannerMem);
					ReleaseDC(GlobalObjects.hWndSetupWindow, hdc);
				}

				// License Page
				if (GlobalObjects.Page == 2) {
					if (GlobalObjects.BackButtonDisabled == FALSE) {
						::SendMessageW(GlobalObjects.hBackBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
					}
					GlobalObjects.hWndDialogWindow = CreateDialogW(GlobalObjects.hInst, MAKEINTRESOURCE(IDD_PAGE2), GlobalObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);

					::SendMessageW(GlobalObjects.hNormalBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);

					wchar_t file[MAX_PATH];
					wcsncpy_s(file, GlobalObjects.installSources, sizeof(file));
					wcsncat_s(file, L"\\license.rtf", sizeof(file));

					GlobalObjects.hWndRichEditCtrl = gr7::CreateRichEdit(GlobalObjects.hWndDialogWindow, 42, 62, 543, 272, GlobalObjects.hInst);
					gr7::FillRichEditFromFile(GlobalObjects.hWndRichEditCtrl, file, SF_RTF);

					memset(file, 0, sizeof(file));
				}

				// Changelog Page
				if (GlobalObjects.Page == 3) {
					if (GlobalObjects.BackButtonDisabled == TRUE) {
						::SendMessageW(GlobalObjects.hBackBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
					}

					GlobalObjects.hWndDialogWindow = CreateDialogW(GlobalObjects.hInst, MAKEINTRESOURCE(IDD_PAGE3), GlobalObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);

					wchar_t file[MAX_PATH];
					wcsncpy_s(file, GlobalObjects.installSources, sizeof(file));
					wcsncat_s(file, L"\\changelog.rtf", sizeof(file));

					GlobalObjects.hWndRichEditCtrl = gr7::CreateRichEdit(GlobalObjects.hWndDialogWindow, 42, 62, 543, 272, GlobalObjects.hInst);
					gr7::FillRichEditFromFile(GlobalObjects.hWndRichEditCtrl, file, SF_RTF);

					memset(file, 0, sizeof(file));
					::SendMessageW(GlobalObjects.hNormalBtn, BTN_UPDATE, (WPARAM)(INT)0, 0);
				}

				// Install Options Page
				if (GlobalObjects.Page == 4) {
					GlobalObjects.hWndDialogWindow = CreateDialogW(GlobalObjects.hInst, MAKEINTRESOURCE(IDD_PAGE4), GlobalObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);
					::SendMessageW(GlobalObjects.hNormalBtn, BTN_UPDATE, (WPARAM)(INT)0, 0);
				}

				// Partition Page
				if (GlobalObjects.Page == 5) {
					GlobalObjects.hWndDialogWindow = CreateDialogW(GlobalObjects.hInst, MAKEINTRESOURCE(IDD_PAGE5), GlobalObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);
					::SendMessageW(GlobalObjects.hNormalBtn, BTN_UPDATE, (WPARAM)(INT)0, 0);
				}

				// Installing Page
				if (GlobalObjects.Page == 6) {
					::SendMessageW(GlobalObjects.hNormalBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
					::SendMessageW(GlobalObjects.hBackBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
					::SendMessageW(GlobalObjects.hCloseBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
					::ShowWindow(GlobalObjects.hNormalBtn, 0);
					GlobalObjects.hWndDialogWindow = CreateDialogW(GlobalObjects.hInst, MAKEINTRESOURCE(IDD_PAGE6), GlobalObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);
					GlobalObjects.CollectingInfoPercentage = 100;
					ProgressBar::updateProgressBar();

					GlobalObjects.CollectingInfoPercentage = GlobalObjects.CollectingInfoPercentage + 1;
					::SendMessageW(GlobalObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
					GUIObj.doNotClose = 1;
					EnableMenuItem(GetSystemMenu(GlobalObjects.hWndSetupWindow, 0), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
					::SendMessageW(GlobalObjects.hWndSetupWindow, SETUPWND_CREATE_PROG_TX, (WPARAM)(INT)0, 0);
					InstallGrass7();
				}

				// Restarting Page
				if (GlobalObjects.Page == 7) {
					::SendMessageW(GlobalObjects.hCloseBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
					GlobalObjects.hWndDialogWindow = CreateDialogW(GlobalObjects.hInst, MAKEINTRESOURCE(IDD_PAGE7), GlobalObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);
					std::thread Restart(GUI::RestartSoon);
					Restart.detach();
				}

				// We show the window and update it to see our dialog page
				::ShowWindow(GlobalObjects.hWndDialogWindow, 1);
				::UpdateWindow(GlobalObjects.hWndDialogWindow);
			}
			break;

		// Call the function responsible for creating the install progress text on the installing page
		case SETUPWND_CREATE_PROG_TX:
			ProgressBar::createProgressText();
			break;

		case SETUPWND_UPDATE_PROG_TX:
			break;

		// This code runs after install was successfuly finished
		case SETUPWND_INSTALL_FINISH:
		{
			GUIObj.doNotClose = 0;
			GlobalObjects.Page = 6;
			EnableMenuItem(GetSystemMenu(GlobalObjects.hWndSetupWindow, 1), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			::SendMessageW(GlobalObjects.hWndSetupWindow, SETUPWND_UPDATE_DIALOG, (WPARAM)(INT)0, 0);
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
			::DestroyWindow(GlobalObjects.hWndMainWindow);
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
					::SendMessageW(GlobalObjects.hNormalBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
				}
				if (IsDlgButtonChecked(hWnd, ID_ACCEPT_LICENSE) == BST_UNCHECKED) {
					::SendMessageW(GlobalObjects.hNormalBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
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