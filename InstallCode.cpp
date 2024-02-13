#include "stdafx.h"
#include "InstallCode.h"
#include "Global.h"
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

// Internal restarting now code
void Install::RestartSoon()
{
	Sleep(5000);
	SendMessageW(MainObjects.hWndMainWindow, WM_CLOSE, (WPARAM)(INT)0, 0);
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
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);

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
	ImageInstallObjects.CopyingFiles = 1;
	ProgressGUI::updateProgressText(63, 123, ProgressTextPercentageObjects.CopyingFilesPercentage, AppResStringsObjects.CopyingFilesText, RGB(0, 0, 0), TRUE, L"");

	ImageInstallObjects.WaitThreadGo = 0;
	std::thread WaitThread(ProgressGUI::WaitThread);
	WaitThread.detach();
	Sleep(5000);
	ProgressTextPercentageObjects.CopyingFilesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
	ImageInstallObjects.WaitThreadGo = 1;
	while (ImageInstallObjects.WaitThreadRunning == 1) {
		Sleep(1000);
	}

	HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
	gr7::PaintTransparentBitmap(hdc, 43, 120, BitmapObjects.hCheckmark, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(MainObjects.hWndSetupWindow, hdc);

	ProgressGUI::updateProgressText(63, 123, ProgressTextPercentageObjects.CopyingFilesPercentage, AppResStringsObjects.CopyingFilesText, RGB(128, 128, 128), FALSE, L"");
	ImageInstallObjects.CopyingFiles = 0;
}

void Install::Progress::ExpandingFiles()
{
	ImageInstallObjects.ExpandingFiles = 1;
	ProgressGUI::updateProgressText(63, 143, ProgressTextPercentageObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText, RGB(0, 0, 0), TRUE, L"");

	ImageInstallObjects.WaitThreadGo = 0;
	std::thread WaitThread(ProgressGUI::WaitThread);
	WaitThread.detach();
	Sleep(5000);
	ImageInstallObjects.WaitThreadGo = 1;
	while (ImageInstallObjects.WaitThreadRunning == 1) {
		Sleep(1000);
	}

	/*int err = Install::ApplyImage();
	if (err != 0) {
	MessageBoxW(NULL,
	AppResStringsObjects.ApplyInstallImageErrorText,
	AppResStringsObjects.AppTitleText, MB_ICONERROR | MB_OK);
	exit(0);
	}*/

	ProgressTextPercentageObjects.ExpandingFilesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);

	HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
	gr7::PaintTransparentBitmap(hdc, 43, 140, BitmapObjects.hCheckmark, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(MainObjects.hWndSetupWindow, hdc);

	ProgressGUI::updateProgressText(63, 143, ProgressTextPercentageObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText, RGB(128, 128, 128), FALSE, L"");
	ImageInstallObjects.ExpandingFiles = 0;
}

void Install::Progress::InstallingFeatures()
{
	ImageInstallObjects.InstallingFeatures = 1;
	ProgressGUI::updateProgressText(63, 163, ProgressTextPercentageObjects.InstallingFeaturesPercentage, AppResStringsObjects.InstallingFeaturesText, RGB(0, 0, 0), TRUE, L"");

	ImageInstallObjects.WaitThreadGo = 0;
	std::thread WaitThread(ProgressGUI::WaitThread);
	WaitThread.detach();
	Sleep(5000);
	ProgressTextPercentageObjects.InstallingFeaturesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
	ImageInstallObjects.WaitThreadGo = 1;
	while (ImageInstallObjects.WaitThreadRunning == 1) {
		Sleep(1000);
	}
	HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
	gr7::PaintTransparentBitmap(hdc, 43, 158, BitmapObjects.hCheckmark, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(MainObjects.hWndSetupWindow, hdc);

	ProgressGUI::updateProgressText(63, 163, ProgressTextPercentageObjects.InstallingFeaturesPercentage, AppResStringsObjects.InstallingFeaturesText, RGB(128, 128, 128), FALSE, L"");
	ImageInstallObjects.InstallingFeatures = 0;
}

void Install::Progress::InstallingUpdates()
{
	ImageInstallObjects.InstallingUpdates = 1;
	ProgressGUI::updateProgressText(63, 183, ProgressTextPercentageObjects.InstallingUpdatesPercentage, AppResStringsObjects.InstallingUpdatesText, RGB(0, 0, 0), TRUE, L"");

	ImageInstallObjects.WaitThreadGo = 0;
	std::thread WaitThread(ProgressGUI::WaitThread);
	WaitThread.detach();
	Sleep(5000);
	ProgressTextPercentageObjects.InstallingUpdatesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
	ImageInstallObjects.WaitThreadGo = 1;
	while (ImageInstallObjects.WaitThreadRunning == 1) {
		Sleep(1000);
	}
	HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
	gr7::PaintTransparentBitmap(hdc, 43, 178, BitmapObjects.hCheckmark, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(MainObjects.hWndSetupWindow, hdc);

	ProgressGUI::updateProgressText(63, 183, ProgressTextPercentageObjects.InstallingUpdatesPercentage, AppResStringsObjects.InstallingUpdatesText, RGB(128, 128, 128), FALSE, L"");
	ImageInstallObjects.InstallingUpdates = 0;
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
		::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
		ProgressGUI::updateProgressText(63, 143, ProgressTextPercentageObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText, RGB(0, 0, 0), TRUE, L"");
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