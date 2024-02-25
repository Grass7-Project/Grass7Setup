#include "stdafx.h"
#include "InstallCode.h"
#include "ProgressGUI.h"
#include "BootSetup.h"
#include <wimlib.h>

Install InstallPrivateObjects;

// Install initiation function
void Install::InstallMain(HDC &hdc)
{
	ProgressGUI::createProgressText(hdc,
		InstallPrivateObjects.CopyingFilesPercentage,
		InstallPrivateObjects.ExpandingFilesPercentage,
		InstallPrivateObjects.InstallingFeaturesPercentage,
		InstallPrivateObjects.InstallingUpdatesPercentage,
		AppResStringsObjects.CopyingFilesText,
		AppResStringsObjects.ExpandingFilesText,
		AppResStringsObjects.InstallingFeaturesText,
		AppResStringsObjects.InstallingUpdatesText);

	::UpdateWindow(MainObjects.hWndSetupWindow);

	std::thread InstallCode(Install::InstallerThread);
	InstallCode.detach();
}

// Installation Thread
void Install::InstallerThread()
{
	// Copying files
	Install::Progress::CopyingFiles(MainObjects.hWndSetupWindow);

	// Expanding files
	Install::Progress::ExpandingFiles(MainObjects.hWndSetupWindow);

	// Installing Features
	Install::Progress::InstallingFeatures(MainObjects.hWndSetupWindow);

	// Installing Updates
	Install::Progress::InstallingUpdates(MainObjects.hWndSetupWindow);

	// Finished
	Sleep(3000);
	ProgressBarObjects.InstallingPercentage = 100;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_INSTALLING_PROG_BAR, (WPARAM)(INT)0, 0);

#ifdef _DEBUG
#else
	// Make system able to boot
	BootSetup::SetupSystemBoot();
#endif

	// Message the Setup Window that installation has finished
	::SendMessageW(MainObjects.hWndSetupWindow, SETUPWND_INSTALL_FINISH, (WPARAM)(INT)0, 0);
}

void Install::Progress::CopyingFiles(HWND &hWndParent)
{
	ProgressGUI::updateProgressTexthWnd(hWndParent, 63, 123, InstallPrivateObjects.CopyingFilesPercentage, AppResStringsObjects.CopyingFilesText, RGB(0, 0, 0), TRUE, L"", FW_BOLD);

	InstallPrivateObjects.WaitThreadExit = FALSE;
	std::thread WaitThread(ProgressGUI::WaitThread, std::ref(InstallPrivateObjects.WaitThreadRunning), std::ref(InstallPrivateObjects.WaitThreadExit), std::ref(InstallPrivateObjects.CopyingFilesPercentage), std::ref(AppResStringsObjects.CopyingFilesText), 63, 123, std::ref(InstallPrivateObjects.DotsText));
	WaitThread.detach();
	Sleep(5000);
	InstallPrivateObjects.CopyingFilesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_INSTALLING_PROG_BAR, (WPARAM)(INT)0, 0);
	InstallPrivateObjects.WaitThreadExit = TRUE;
	while (InstallPrivateObjects.WaitThreadRunning == TRUE) {
		Sleep(1000);
	}

	HDC hdc = ::GetDC(hWndParent);
	gr7::PaintTransparentBitmap(hdc, 43, 120, BitmapObjects.hCheckmark, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(hWndParent, hdc);

	ProgressGUI::updateProgressTexthWnd(hWndParent, 63, 123, InstallPrivateObjects.CopyingFilesPercentage, AppResStringsObjects.CopyingFilesText, RGB(128, 128, 128), FALSE, L"", FW_LIGHT);
}

void Install::Progress::ExpandingFiles(HWND &hWndParent)
{
	ProgressGUI::updateProgressTexthWnd(hWndParent, 63, 143, InstallPrivateObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText, RGB(0, 0, 0), TRUE, L"", FW_BOLD);

	InstallPrivateObjects.WaitThreadExit = FALSE;
	std::thread WaitThread(ProgressGUI::WaitThread, std::ref(InstallPrivateObjects.WaitThreadRunning), std::ref(InstallPrivateObjects.WaitThreadExit), std::ref(InstallPrivateObjects.ExpandingFilesPercentage), std::ref(AppResStringsObjects.ExpandingFilesText), 63, 143, std::ref(InstallPrivateObjects.DotsText));
	WaitThread.detach();
	Sleep(5000);

	if (!ImageInstallObjects.NoDeploy) {
		int err = Install::ApplyImage();
		ErrorHandler::InvokeErrorHandler(err, 0, L"Failed to apply installation image.", AppResStringsObjects.AppTitleText);
	}

	InstallPrivateObjects.WaitThreadExit = TRUE;
	while (InstallPrivateObjects.WaitThreadRunning == TRUE) {
		Sleep(1000);
	}

	HDC hdc = ::GetDC(hWndParent);
	gr7::PaintTransparentBitmap(hdc, 43, 140, BitmapObjects.hCheckmark, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(hWndParent, hdc);

	ProgressGUI::updateProgressTexthWnd(hWndParent, 63, 143, InstallPrivateObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText, RGB(128, 128, 128), FALSE, L"", FW_LIGHT);
}

void Install::Progress::InstallingFeatures(HWND &hWndParent)
{
	ProgressGUI::updateProgressTexthWnd(hWndParent, 63, 163, InstallPrivateObjects.InstallingFeaturesPercentage, AppResStringsObjects.InstallingFeaturesText, RGB(0, 0, 0), TRUE, L"", FW_BOLD);

	InstallPrivateObjects.WaitThreadExit = FALSE;
	std::thread WaitThread(ProgressGUI::WaitThread, std::ref(InstallPrivateObjects.WaitThreadRunning), std::ref(InstallPrivateObjects.WaitThreadExit), std::ref(InstallPrivateObjects.InstallingFeaturesPercentage), std::ref(AppResStringsObjects.InstallingFeaturesText), 63, 163, std::ref(InstallPrivateObjects.DotsText));
	WaitThread.detach();
	Sleep(5000);
	InstallPrivateObjects.InstallingFeaturesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_INSTALLING_PROG_BAR, (WPARAM)(INT)0, 0);
	InstallPrivateObjects.WaitThreadExit = TRUE;
	while (InstallPrivateObjects.WaitThreadRunning == TRUE) {
		Sleep(1000);
	}
	HDC hdc = ::GetDC(hWndParent);
	gr7::PaintTransparentBitmap(hdc, 43, 158, BitmapObjects.hCheckmark, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(hWndParent, hdc);

	ProgressGUI::updateProgressTexthWnd(hWndParent, 63, 163, InstallPrivateObjects.InstallingFeaturesPercentage, AppResStringsObjects.InstallingFeaturesText, RGB(128, 128, 128), FALSE, L"", FW_LIGHT);
}

void Install::Progress::InstallingUpdates(HWND &hWndParent)
{
	ProgressGUI::updateProgressTexthWnd(hWndParent, 63, 183, InstallPrivateObjects.InstallingUpdatesPercentage, AppResStringsObjects.InstallingUpdatesText, RGB(0, 0, 0), TRUE, L"", FW_BOLD);

	InstallPrivateObjects.WaitThreadExit = FALSE;
	std::thread WaitThread(ProgressGUI::WaitThread, std::ref(InstallPrivateObjects.WaitThreadRunning), std::ref(InstallPrivateObjects.WaitThreadExit), std::ref(InstallPrivateObjects.InstallingUpdatesPercentage), std::ref(AppResStringsObjects.InstallingUpdatesText), 63, 183, std::ref(InstallPrivateObjects.DotsText));
	WaitThread.detach();
	Sleep(5000);
	InstallPrivateObjects.InstallingUpdatesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_INSTALLING_PROG_BAR, (WPARAM)(INT)0, 0);
	InstallPrivateObjects.WaitThreadExit = TRUE;
	while (InstallPrivateObjects.WaitThreadRunning == TRUE) {
		Sleep(1000);
	}

	HDC hdc = ::GetDC(hWndParent);
	gr7::PaintTransparentBitmap(hdc, 43, 178, BitmapObjects.hCheckmark, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(hWndParent, hdc);

	ProgressGUI::updateProgressTexthWnd(hWndParent, 63, 183, InstallPrivateObjects.InstallingUpdatesPercentage, AppResStringsObjects.InstallingUpdatesText, RGB(128, 128, 128), FALSE, L"", FW_LIGHT);
}

#define TO_PERCENT(numerator, denominator) \
	((float)(((denominator) == 0) ? 0 : ((numerator) * 100 / (float)(denominator))))

enum wimlib_progress_status Install::extract_progress(enum wimlib_progress_msg msg, union wimlib_progress_info *info, void *progctx)
{
	switch (msg)
	{
	case WIMLIB_PROGRESS_MSG_EXTRACT_STREAMS:
		InstallPrivateObjects.ExpandingFilesPercentage = (INT)TO_PERCENT(info->extract.completed_bytes, info->extract.total_bytes);
		#pragma warning(suppress : 4244)
		ProgressBarObjects.InstallingPercentage = 25 + InstallPrivateObjects.ExpandingFilesPercentage * 0.25;
		::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_INSTALLING_PROG_BAR, (WPARAM)(INT)0, 0);
		ProgressGUI::updateProgressTexthWnd(MainObjects.hWndSetupWindow, 63, 143, InstallPrivateObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText, RGB(0, 0, 0), TRUE, InstallPrivateObjects.DotsText, FW_BOLD);
		break;
	default:
		break;
	}
	return WIMLIB_PROGRESS_STATUS_CONTINUE;
}

int Install::ApplyImage()
{
	int ret;
	WIMStruct *wim = NULL;

	ret = wimlib_open_wim(ImageInstallObjects.ImagePath.c_str(), 0, &wim);

	if (ret != 0)
	{
		wimlib_global_cleanup();
		return ret;
	}

	wimlib_register_progress_function(wim, Install::extract_progress, NULL);

	ret = wimlib_extract_image(wim, ImageInstallObjects.ImageIndex, ImageInstallObjects.destDrive.c_str(), 0);

	wimlib_free(wim);

	if (ret != 0)
	{
		wimlib_global_cleanup();
		return ret;
	}

	wimlib_global_cleanup();
	return 0;
}