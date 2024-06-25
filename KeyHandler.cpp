#include "stdafx.h"
#include "KeyHandler.h"
#include "RecoveryLauncher.h"

void KeyHandler::InvokeKeyHandler(WPARAM &wParam)
{
	if (wParam == VK_BACK) {
		::SendMessageW(ButtonObjects.hBackBtn, WM_LBUTTONUP, (WPARAM)(INT)0, 0);
	}
	if (wParam == VK_ESCAPE) {
		::SendMessageW(ButtonObjects.hCloseBtn, WM_LBUTTONUP, (WPARAM)(INT)0, 0);
	}
	if (wParam == VK_RETURN) {
		::SendMessageW(ButtonObjects.hNormalBtn, WM_LBUTTONUP, (WPARAM)(INT)0, 0);
	}
	if (wParam == VK_SPACE) {
		if (IsDlgButtonChecked(MainObjects.hWndDialogWindow, ID_ACCEPT_LICENSE) == BST_UNCHECKED) {
			CheckDlgButton(MainObjects.hWndDialogWindow, ID_ACCEPT_LICENSE, BST_CHECKED);
			::SendMessageW(MainObjects.hWndDialogWindow, WM_COMMAND, MAKEWPARAM(ID_ACCEPT_LICENSE, BST_CHECKED), (LPARAM)GetDlgItem(MainObjects.hWndDialogWindow, ID_ACCEPT_LICENSE));
		}
		else {
			CheckDlgButton(MainObjects.hWndDialogWindow, ID_ACCEPT_LICENSE, BST_UNCHECKED);
			::SendMessageW(MainObjects.hWndDialogWindow, WM_COMMAND, MAKEWPARAM(ID_ACCEPT_LICENSE, BST_UNCHECKED), (LPARAM)GetDlgItem(MainObjects.hWndDialogWindow, ID_ACCEPT_LICENSE));
		}
	}
	if (!MainObjects.Debug) {
		if (wParam == VK_F9) {
			std::thread RecoveryLauncherThread(RecoveryLauncher::LaunchRecovery);
			RecoveryLauncherThread.detach();
			::ShowWindow(MainObjects.hWndMainWindow, SW_MINIMIZE);
		}
	}
}