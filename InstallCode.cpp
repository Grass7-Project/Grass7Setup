#include "stdafx.h"
#include "InstallCode.h"
#include "Global.h"
#include "ProgressGUI.h"
#include "MainGUI.h"
#include "BootSetup.h"
#include <wimlib.h>

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
		ProgressGUI::updateProgressText(63, 143, ProgressTextPercentageObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText);
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

	ret = wimlib_open_wim(ImageInstallObjects.ImagePath, 0, &wim);

	if (ret != 0)
	{
		wimlib_global_cleanup();
		return ret;
	}

	wimlib_register_progress_function(wim, extract_progress, NULL);

	ret = wimlib_extract_image(wim, ImageInstallObjects.ImageIndex, ImageInstallObjects.destDrive, 0);

	wimlib_free(wim);

	if (ret != 0)
	{
		wimlib_global_cleanup();
		return ret;
	}

	wimlib_global_cleanup();
	return 0;
}

void Install::InstallerThread()
{
	HDC hdc;
	// Copying files
	Sleep(3000);
	ProgressTextPercentageObjects.CopyingFilesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);

	hdc = ::GetDC(MainObjects.hWndSetupWindow);
	gr7::PaintTransparentBitmap(hdc, 0, 121, BitmapObjects.hCheckmark, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(MainObjects.hWndSetupWindow, hdc);

	ProgressGUI::updateProgressText(63, 123, ProgressTextPercentageObjects.CopyingFilesPercentage, AppResStringsObjects.CopyingFilesText);


	// Expanding files
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

	hdc = ::GetDC(MainObjects.hWndSetupWindow);
	gr7::PaintTransparentBitmap(hdc, 0, 141, BitmapObjects.hCheckmark, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(MainObjects.hWndSetupWindow, hdc);

	ProgressGUI::updateProgressText(63, 143, ProgressTextPercentageObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText);

	// Installing Features
	Sleep(3000);
	ProgressTextPercentageObjects.InstallingFeaturesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);

	hdc = ::GetDC(MainObjects.hWndSetupWindow);
	gr7::PaintTransparentBitmap(hdc, 0, 159, BitmapObjects.hCheckmark, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(MainObjects.hWndSetupWindow, hdc);

	ProgressGUI::updateProgressText(63, 163, ProgressTextPercentageObjects.InstallingFeaturesPercentage, AppResStringsObjects.InstallingFeaturesText);

	// Installing Updates
	Sleep(3000);
	ProgressTextPercentageObjects.InstallingUpdatesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);

	hdc = ::GetDC(MainObjects.hWndSetupWindow);
	gr7::PaintTransparentBitmap(hdc, 0, 179, BitmapObjects.hCheckmark, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	ReleaseDC(MainObjects.hWndSetupWindow, hdc);

	ProgressGUI::updateProgressText(63, 183, ProgressTextPercentageObjects.InstallingUpdatesPercentage, AppResStringsObjects.InstallingUpdatesText);

	// Finished
	Sleep(2000);
	ProgressBarObjects.InstallingPercentage = 100;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);

	#ifdef _DEBUG
	#else
	// Make system able to boot
	BootSetup::SetupSystemBoot();
	#endif

	// Message the Setup Window that installation is finished
	::SendMessageW(MainObjects.hWndSetupWindow, SETUPWND_INSTALL_FINISH, (WPARAM)(INT)0, 0);
}

// Caller install function
void Install::InstallMain()
{
	std::thread InstallCode(Install::InstallerThread);
	InstallCode.detach();
}

void Install::RestartSoon()
{
	Sleep(5000);
	SendMessageW(MainObjects.hWndMainWindow, WM_CLOSE, (WPARAM)(INT)0, 0);
}