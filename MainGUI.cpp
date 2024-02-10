#include "stdafx.h"
#include "MainGUI.h"
#include "Global.h"
#include "ProgressGUI.h"
#include "InstallCode.h"
#include "ButtonGUI.h"
#include "PartitionCode.h"
#include "ResourceLoader.h"
#include <Uxtheme.h>
#include <gdiplus.h>

MainGUI MainGUIObj;

// Main GUI Code
BOOL MainGUI::InitInstance()
{
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

	EnableMenuItem(GetSystemMenu(MainObjects.hWndMainWindow, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	SetWindowLongW(MainObjects.hWndMainWindow, GWL_STYLE, GetWindowLongW(MainObjects.hWndMainWindow, GWL_STYLE) & ~WS_MINIMIZEBOX);

	// Change appearence of the Main Window
	DWORD dwStyle = GetWindowLongW(MainObjects.hWndMainWindow, GWL_STYLE);
	DWORD dwRemove = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	DWORD dwNewStyle = dwStyle & ~dwRemove;
	HDC hDC = GetWindowDC(NULL);
	SetWindowLongW(MainObjects.hWndMainWindow, GWL_STYLE, dwNewStyle);

	// Set position of the main window
	SetWindowPos(MainObjects.hWndMainWindow, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	SetWindowPos(MainObjects.hWndMainWindow, NULL, 0, 0, GetDeviceCaps(hDC, HORZRES), GetDeviceCaps(hDC, VERTRES), SWP_FRAMECHANGED);

	if (!MainObjects.hWndMainWindow) {
		return 0;
	}

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

void MainGUI::DialogPaintCode()
{
	// Text painting options
	PaintTextOptions PaintTextOpt;
	PaintTextOpt.xPos = 43;
	PaintTextOpt.yPos = 22;
	PaintTextOpt.font = L"Segoe UI";
	PaintTextOpt.color = RGB(0, 105, 51);
	PaintTextOpt.nSize = 12;

	// Welcome Page
	if (MainObjects.Page == 1) {
		// Draw Logo Text
		HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
		gr7::PaintTransparentBitmap(hdc, 0, (428 / 2) - 72, BitmapObjects.hBanner, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
		ReleaseDC(MainObjects.hWndSetupWindow, hdc);
	}

	// License Page
	if (MainObjects.Page == 2) {
		// Draw Dialog Title Text
		HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
		PaintTextOpt.text = AppResStringsObjects.EulaTitleText.c_str();
		gr7::PaintText(hdc, PaintTextOpt);
		ReleaseDC(MainObjects.hWndSetupWindow, hdc);

		::UpdateWindow(MainObjects.hWndSetupWindow);
	}

	// Changelog Page
	if (MainObjects.Page == 3) {
		// Draw Dialog Title Text
		HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
		PaintTextOpt.text = AppResStringsObjects.ChangelogTitleText.c_str();
		gr7::PaintText(hdc, PaintTextOpt);
		ReleaseDC(MainObjects.hWndSetupWindow, hdc);

		::UpdateWindow(MainObjects.hWndSetupWindow);
	}

	// Partition Page
	if (MainObjects.Page == 4) {
		// Draw Dialog Title Text
		HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
		PaintTextOpt.text = AppResStringsObjects.PartitionTitleText.c_str();
		gr7::PaintText(hdc, PaintTextOpt);
		int nHeightFont = -MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		ReleaseDC(MainObjects.hWndSetupWindow, hdc);

		::UpdateWindow(MainObjects.hWndSetupWindow);
	}

	// Installing Page
	if (MainObjects.Page == 5) {
		// Draw Dialog Title Text
		HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
		PaintTextOpt.text = AppResStringsObjects.InstallingTitleText.c_str();
		gr7::PaintText(hdc, PaintTextOpt);
		ReleaseDC(MainObjects.hWndSetupWindow, hdc);
		ProgressGUI::createProgressText();

		::UpdateWindow(MainObjects.hWndSetupWindow);
	}

	// Restarting Page
	if (MainObjects.Page == 6) {
		// Draw Dialog Title Text
		HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
		PaintTextOpt.text = AppResStringsObjects.RestartingTitleText.c_str();
		gr7::PaintText(hdc, PaintTextOpt);
		ReleaseDC(MainObjects.hWndSetupWindow, hdc);

		::UpdateWindow(MainObjects.hWndSetupWindow);
	}

	HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
	gr7::PaintTransparentBitmap(hdc, 0, 382, BitmapObjects.hBottomPanel, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(MainObjects.hWndSetupWindow, hdc);

	::SendMessageW(ButtonObjects.hNormalBtn, BTN_UPDATE, (WPARAM)(INT)0, 0);
}

// Main Window Window Procedure
LRESULT CALLBACK MainGUI::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
		case WM_CLOSE:
		{
			if (MainGUIObj.doNotClose == TRUE) {
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
				gr7::GetDesktopResolution(horizontal, vertical);
				hdc = BeginPaint(hWnd, &ps);
				SetStretchBltMode(hdc, HALFTONE);

				// Draw Background Image
				HDC hdcBkgMem = CreateCompatibleDC(hdc);
				HGDIOBJ oldBkgBitmap = (HBITMAP)SelectObject(hdcBkgMem, BitmapObjects.hBackground);
				GetObjectW(BitmapObjects.hBackground, sizeof(BkgBitmap), &BkgBitmap);
				StretchBlt(hdc, 0, 0, horizontal, vertical, hdcBkgMem, 0, 0, BkgBitmap.bmWidth, BkgBitmap.bmHeight, SRCCOPY);
				SelectObject(hdcBkgMem, oldBkgBitmap);

				// Draw Fake Window
				HDC hdcWndMem = CreateCompatibleDC(hdc);
				GetObjectW(BitmapObjects.hFakeWindow, sizeof(WndBitmap), &WndBitmap);
				int xPos = (horizontal - WndBitmap.bmWidth) / 2;
				int yPos = (vertical - WndBitmap.bmHeight) / 2;
				gr7::PaintTransparentBitmap(hdc, xPos, yPos - 33, BitmapObjects.hFakeWindow, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
				
				// Draw Small Logo
				gr7::PaintTransparentBitmap(hdc, xPos + 56, yPos + 26 - 33, BitmapObjects.hSmallLogo, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
				
				// Draw Title Text
				PaintTextOptions PaintTextOpt;
				PaintTextOpt.xPos = xPos + 56 + 23;
				PaintTextOpt.yPos = yPos + 26 - 33;
				PaintTextOpt.font = L"Segoe UI";
				PaintTextOpt.color = RGB(0, 0, 0);
				PaintTextOpt.text = AppResStringsObjects.TitleBarText.c_str();
				PaintTextOpt.nSize = 9;
				gr7::PaintText(hdc, PaintTextOpt);

				DeleteDC(hdcWndMem);
				DeleteDC(hdcBkgMem);
				EndPaint(hWnd, &ps);
			}
			break;

		// Calls the function to create the progress bar
		case MAINWND_CREATE_PROG_BAR:
			ProgressGUI::createProgressBar();
			break;

		// Calls the function to update the progress bar
		case MAINWND_UPDATE_PROG_BAR:
			ProgressGUI::updateProgressBar();
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
				if (MainGUIObj.doNotClose == TRUE) {
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
					gr7::FillRichEditFromFile(RichEditControlObjects.hWndRichEditCtrl, file.c_str() , SF_RTF);
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
				}

				// Partition Page
				if (MainObjects.Page == 4) {
					#ifdef _DEBUG
					::SendMessageW(ButtonObjects.hNormalBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
					::SendMessageW(ButtonObjects.hAutoPartitionBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
					::SendMessageW(ButtonObjects.hManualPartitionBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
					#else
					::SendMessageW(ButtonObjects.hNormalBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
					#endif
					MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_PARTITIONPAGE), MainObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);

					::ShowWindow(ButtonObjects.hAutoPartitionBtn, TRUE);
					::ShowWindow(ButtonObjects.hManualPartitionBtn, TRUE);
				}

				// Installing Page
				if (MainObjects.Page == 5) {
					::ShowWindow(ButtonObjects.hAutoPartitionBtn, FALSE);
					::ShowWindow(ButtonObjects.hManualPartitionBtn, FALSE);

					::SendMessageW(ButtonObjects.hCloseBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
					::ShowWindow(ButtonObjects.hNormalBtn, FALSE);
					::ShowWindow(ButtonObjects.hBackBtn, FALSE);

					MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_INSTALLINGPAGE), MainObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);
					ProgressBarObjects.CollectingInfoPercentage = 100;
					ProgressGUI::updateProgressBar();

					ProgressBarObjects.CollectingInfoPercentage = ProgressBarObjects.CollectingInfoPercentage + 1;
					::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
					MainGUIObj.doNotClose = 1;
					EnableMenuItem(GetSystemMenu(MainObjects.hWndSetupWindow, 0), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
					Install::InstallMain();
				}

				// Restarting Page
				if (MainObjects.Page == 6) {
					::SendMessageW(ButtonObjects.hCloseBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
					MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_RESTARTINGPAGE), MainObjects.hWndSetupWindow, (DLGPROC)WndProcDialogWnd);
					std::thread Restart(Install::RestartSoon);
					Restart.detach();
				}

				// We show the window and update it to see our dialog page
				::ShowWindow(MainObjects.hWndDialogWindow, TRUE);
				::UpdateWindow(MainObjects.hWndDialogWindow);

				//Activate custom paint code for dialogs
				MainGUI::DialogPaintCode();
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