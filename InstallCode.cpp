#include "stdafx.h"
#include "InstallCode.h"
#include "ProgressGUI.h"
#include "MainGUI.h"
#include "BootSetup.h"
#include <wimlib.h>

// Caller install function
void Install::InstallMain()
{
	std::thread InstallCode(Install::InstallerThread);
	InstallCode.detach();
}

// Installation Thread
void Install::InstallerThread()
{
	// Copying files
	Install::Progress::CopyingFiles();

	// Expanding files
	Install::Progress::ExpandingFiles();

	// Installing Features
	Install::Progress::InstallingFeatures();

	// Installing Updates
	Install::Progress::InstallingUpdates();

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

void Install::Progress::CopyingFiles()
{
	ImageInstallObjects.CopyingFiles = TRUE;
	ProgressGUI::updateProgressTexthWnd(MainObjects.hWndSetupWindow, 63, 123, ProgressTextPercentageObjects.CopyingFilesPercentage, AppResStringsObjects.CopyingFilesText, RGB(0, 0, 0), TRUE, L"", FW_BOLD);

	ImageInstallObjects.WaitThreadGo = FALSE;
	std::thread WaitThread(ProgressGUI::WaitThread);
	WaitThread.detach();
	Sleep(5000);
	ProgressTextPercentageObjects.CopyingFilesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_INSTALLING_PROG_BAR, (WPARAM)(INT)0, 0);
	ImageInstallObjects.WaitThreadGo = TRUE;
	while (ImageInstallObjects.WaitThreadRunning == TRUE) {
		Sleep(1000);
	}

	HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
	gr7::PaintTransparentBitmap(hdc, 43, 120, BitmapObjects.hCheckmark, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(MainObjects.hWndSetupWindow, hdc);

	ProgressGUI::updateProgressTexthWnd(MainObjects.hWndSetupWindow, 63, 123, ProgressTextPercentageObjects.CopyingFilesPercentage, AppResStringsObjects.CopyingFilesText, RGB(128, 128, 128), FALSE, L"", FW_LIGHT);
	ImageInstallObjects.CopyingFiles = FALSE;
}

void Install::Progress::ExpandingFiles()
{
	ImageInstallObjects.ExpandingFiles = TRUE;
	ProgressGUI::updateProgressTexthWnd(MainObjects.hWndSetupWindow, 63, 143, ProgressTextPercentageObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText, RGB(0, 0, 0), TRUE, L"", FW_BOLD);

	ImageInstallObjects.WaitThreadGo = FALSE;
	std::thread WaitThread(ProgressGUI::WaitThread);
	WaitThread.detach();
	Sleep(5000);
	ImageInstallObjects.WaitThreadGo = TRUE;
	while (ImageInstallObjects.WaitThreadRunning == TRUE) {
		Sleep(1000);
	}

	if (!ImageInstallObjects.NoDeploy) {
		int err = Install::ApplyImage();
		ErrorHandler::InvokeErrorHandler(err, 0, L"Failed to apply installation image.", AppResStringsObjects.AppTitleText);
	}

	ProgressTextPercentageObjects.ExpandingFilesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_INSTALLING_PROG_BAR, (WPARAM)(INT)0, 0);

	HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
	gr7::PaintTransparentBitmap(hdc, 43, 140, BitmapObjects.hCheckmark, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(MainObjects.hWndSetupWindow, hdc);

	ProgressGUI::updateProgressTexthWnd(MainObjects.hWndSetupWindow, 63, 143, ProgressTextPercentageObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText, RGB(128, 128, 128), FALSE, L"", FW_LIGHT);
	ImageInstallObjects.ExpandingFiles = FALSE;
}

void Install::Progress::InstallingFeatures()
{
	ImageInstallObjects.InstallingFeatures = TRUE;
	ProgressGUI::updateProgressTexthWnd(MainObjects.hWndSetupWindow, 63, 163, ProgressTextPercentageObjects.InstallingFeaturesPercentage, AppResStringsObjects.InstallingFeaturesText, RGB(0, 0, 0), TRUE, L"", FW_BOLD);

	ImageInstallObjects.WaitThreadGo = FALSE;
	std::thread WaitThread(ProgressGUI::WaitThread);
	WaitThread.detach();
	Sleep(5000);
	ProgressTextPercentageObjects.InstallingFeaturesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_INSTALLING_PROG_BAR, (WPARAM)(INT)0, 0);
	ImageInstallObjects.WaitThreadGo = TRUE;
	while (ImageInstallObjects.WaitThreadRunning == TRUE) {
		Sleep(1000);
	}
	HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
	gr7::PaintTransparentBitmap(hdc, 43, 158, BitmapObjects.hCheckmark, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(MainObjects.hWndSetupWindow, hdc);

	ProgressGUI::updateProgressTexthWnd(MainObjects.hWndSetupWindow, 63, 163, ProgressTextPercentageObjects.InstallingFeaturesPercentage, AppResStringsObjects.InstallingFeaturesText, RGB(128, 128, 128), FALSE, L"", FW_LIGHT);
	ImageInstallObjects.InstallingFeatures = FALSE;
}

void Install::Progress::InstallingUpdates()
{
	ImageInstallObjects.InstallingUpdates = TRUE;
	ProgressGUI::updateProgressTexthWnd(MainObjects.hWndSetupWindow, 63, 183, ProgressTextPercentageObjects.InstallingUpdatesPercentage, AppResStringsObjects.InstallingUpdatesText, RGB(0, 0, 0), TRUE, L"", FW_BOLD);

	ImageInstallObjects.WaitThreadGo = 0;
	std::thread WaitThread(ProgressGUI::WaitThread);
	WaitThread.detach();
	Sleep(5000);
	ProgressTextPercentageObjects.InstallingUpdatesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_INSTALLING_PROG_BAR, (WPARAM)(INT)0, 0);
	ImageInstallObjects.WaitThreadGo = TRUE;
	while (ImageInstallObjects.WaitThreadRunning == TRUE) {
		Sleep(1000);
	}

	HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
	gr7::PaintTransparentBitmap(hdc, 43, 178, BitmapObjects.hCheckmark, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(MainObjects.hWndSetupWindow, hdc);

	ProgressGUI::updateProgressTexthWnd(MainObjects.hWndSetupWindow, 63, 183, ProgressTextPercentageObjects.InstallingUpdatesPercentage, AppResStringsObjects.InstallingUpdatesText, RGB(128, 128, 128), FALSE, L"", FW_LIGHT);
	ImageInstallObjects.InstallingUpdates = FALSE;
}

#define TO_PERCENT(numerator, denominator) \
	((float)(((denominator) == 0) ? 0 : ((numerator) * 100 / (float)(denominator))))

static enum wimlib_progress_status
extract_progress(enum wimlib_progress_msg msg,
	union wimlib_progress_info *info, void *progctx)
{
	switch (msg)
	{
	case WIMLIB_PROGRESS_MSG_EXTRACT_STREAMS:
		ProgressTextPercentageObjects.ExpandingFilesPercentage = (INT)TO_PERCENT(info->extract.completed_bytes, info->extract.total_bytes);
		::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_INSTALLING_PROG_BAR, (WPARAM)(INT)0, 0);
		ProgressGUI::updateProgressTexthWnd(MainObjects.hWndSetupWindow, 63, 143, ProgressTextPercentageObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText, RGB(0, 0, 0), TRUE, L"", FW_BOLD);
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

	wimlib_register_progress_function(wim, extract_progress, NULL);

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