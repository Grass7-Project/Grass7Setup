#include "stdafx.h"
#include "DialogGUI.h"
#include "ProgressGUI.h"
#include "InstallCode.h"
#include "ButtonGUI.h"
#include "RestartCode.h"

DialogGUI DialogGUIPrivateObjects;

void DialogGUI::Init()
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = DialogGUI::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = MainObjects.hInst;
	wcex.hIcon = LoadIconW(MainObjects.hInst, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"DialogWindow";
	wcex.hIconSm = LoadIconW(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	DialogGUI::SetRichEditPos();
}

void DialogGUI::SetRichEditPos()
{
	RECT rc;

	GetWindowRect(MainObjects.hWndSetupWindow, &rc);

	int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;

	// Set position of the rich edit control
	DialogGUIPrivateObjects.RichEditCtrlX = xPos - 3;
	DialogGUIPrivateObjects.RichEditCtrlY = yPos + 14;
}

void DialogGUI::Update()
{
	::ShowWindow(MainObjects.hWndDialogWindow, 0);

	// Welcome Page
	if (MainObjects.Page == 1) {
		::SendMessageW(ButtonObjects.hAutoPartitionBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
		::SendMessageW(ButtonObjects.hManualPartitionBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
		::ShowWindow(ButtonObjects.hAutoPartitionBtn, FALSE);
		::ShowWindow(ButtonObjects.hManualPartitionBtn, FALSE);

		MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_WELCOMEPAGE), MainObjects.hWndSetupWindow, (DLGPROC)DialogGUI::WndProc);
		::SendMessageW(ButtonObjects.hBackBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
		if (ButtonObjects.NormalButtonState == 3) {
			::SendMessageW(ButtonObjects.hNormalBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
		}

		if (DialogGUIPrivateObjects.hWndRichEditCtrl != NULL)
		{
			DestroyWindow(DialogGUIPrivateObjects.hWndRichEditCtrl);
		}
	}

	// License Page
	if (MainObjects.Page == 2) {
		if (ButtonObjects.BackButtonDisabled == FALSE) {
			::SendMessageW(ButtonObjects.hBackBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);
		}
		MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_LICENSEPAGE), MainObjects.hWndSetupWindow, (DLGPROC)DialogGUI::WndProc);

		::SendMessageW(ButtonObjects.hNormalBtn, BTN_DISABLE, (WPARAM)(INT)0, 0);

		// Show license
		std::wstring file = ImageInstallObjects.installSources;
		file.append(L"\\license.rtf");

		DialogGUIPrivateObjects.hWndRichEditCtrl = gr7::CreateRichEdit(MainObjects.hWndDialogWindow, 42, 62, 543, 272, MainObjects.hInst);
		gr7::FillRichEditFromFile(DialogGUIPrivateObjects.hWndRichEditCtrl, file.c_str(), SF_RTF);
		::SendMessageW(DialogGUIPrivateObjects.hWndRichEditCtrl, EM_SETREADONLY, TRUE, 0);
	}

	// Changelog Page
	if (MainObjects.Page == 3) {
		::ShowWindow(ButtonObjects.hAutoPartitionBtn, FALSE);
		::ShowWindow(ButtonObjects.hManualPartitionBtn, FALSE);

		if (ButtonObjects.NormalButtonState == 3) {
			::SendMessageW(ButtonObjects.hNormalBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
		}

		if (DialogGUIPrivateObjects.hWndRichEditCtrl != NULL)
		{
			DestroyWindow(DialogGUIPrivateObjects.hWndRichEditCtrl);
		}
		if (ButtonObjects.BackButtonDisabled == TRUE) {
			::SendMessageW(ButtonObjects.hBackBtn, BTN_ENABLE, (WPARAM)(INT)0, 0);
		}

		MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_CHANGELOGPAGE), MainObjects.hWndSetupWindow, (DLGPROC)DialogGUI::WndProc);

		std::wstring file = ImageInstallObjects.installSources;
		file.append(L"\\changelog.rtf");

		DialogGUIPrivateObjects.hWndRichEditCtrl = gr7::CreateRichEdit(MainObjects.hWndDialogWindow, 42, 62, 543, 272, MainObjects.hInst);
		gr7::FillRichEditFromFile(DialogGUIPrivateObjects.hWndRichEditCtrl, file.c_str(), SF_RTF);
		::SendMessageW(DialogGUIPrivateObjects.hWndRichEditCtrl, EM_SETREADONLY, TRUE, 0);
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

		MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_PARTITIONPAGE), MainObjects.hWndSetupWindow, (DLGPROC)DialogGUI::WndProc);

		::ShowWindow(ButtonObjects.hAutoPartitionBtn, TRUE);
		::ShowWindow(ButtonObjects.hManualPartitionBtn, TRUE);
	}

	// Installing Page
	if (MainObjects.Page == 5) {
		::ShowWindow(ButtonObjects.hAutoPartitionBtn, FALSE);
		::ShowWindow(ButtonObjects.hManualPartitionBtn, FALSE);
		::ShowWindow(ButtonObjects.hNormalBtn, FALSE);
		::ShowWindow(ButtonObjects.hBackBtn, FALSE);

		MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_INSTALLINGPAGE), MainObjects.hWndSetupWindow, (DLGPROC)DialogGUI::WndProc);
	}

	// Restarting Page
	if (MainObjects.Page == 6) {
		::ShowWindow(ButtonObjects.hCloseBtn, FALSE);
		::ShowWindow(ProgressBarObjects.hProgressCtrlRestarting, TRUE);
		MainObjects.hWndDialogWindow = CreateDialogW(MainObjects.hInst, MAKEINTRESOURCE(IDD_RESTARTINGPAGE), MainObjects.hWndSetupWindow, (DLGPROC)DialogGUI::WndProc);
	}

	if (!MainObjects.hWndDialogWindow) {
		ErrorHandler::InvokeErrorHandler(1, 0, L"Failed to create Dialog Window", AppResStringsObjects.AppTitleText);
	}
	else {
		// We show the window and update it to see our dialog page
		::ShowWindow(MainObjects.hWndDialogWindow, TRUE);
		::UpdateWindow(MainObjects.hWndDialogWindow);

		::SendMessageW(ButtonObjects.hNormalBtn, BTN_UPDATE, (WPARAM)(INT)0, 0);
	}
}

void DialogGUI::Paint(HWND &hWnd)
{
	PAINTSTRUCT     ps;
	HDC             hdc;

	hdc = BeginPaint(hWnd, &ps);
	gr7::PaintTransparentBitmap(hdc, 0, 382, BitmapObjects.hBottomPanel, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });

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
		MainObjects.doNotClose = 1;
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

	EndPaint(hWnd, &ps);
}

// Dialog Window Window Procedure
LRESULT CALLBACK DialogGUI::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		{
			DialogGUI::Paint(hWnd);
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