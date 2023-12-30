#include "stdafx.h"
#include "InstallCode.h"
#include "Global.h"
#include "ProgressBar.h"
#include "GUI.h"
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
		ProgressBar::updateProgressText(63, 143, ProgressTextPercentageObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText);
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

	ret = wimlib_open_wim(ImageInstallObjects.srcImage, 0, &wim);

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

void InstallMain()
{
	// Create progress text
	ProgressBar::createProgressText();

	// Copying files
	Sleep(5000);
	ProgressTextPercentageObjects.CopyingFilesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
	ProgressBar::updateProgressText(63, 123, ProgressTextPercentageObjects.CopyingFilesPercentage, AppResStringsObjects.CopyingFilesText);

	// Expanding files
	int err = Install::ApplyImage();
	if (err != 0) {

	}
	ProgressTextPercentageObjects.ExpandingFilesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
	ProgressBar::updateProgressText(63, 143, ProgressTextPercentageObjects.ExpandingFilesPercentage, AppResStringsObjects.ExpandingFilesText);

	// Installing Features
	Sleep(5000);
	ProgressTextPercentageObjects.InstallingFeaturesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
	ProgressBar::updateProgressText(63, 163, ProgressTextPercentageObjects.InstallingFeaturesPercentage, AppResStringsObjects.InstallingFeaturesText);

	// Installing Updates
	Sleep(5000);
	ProgressTextPercentageObjects.InstallingUpdatesPercentage = 100;
	ProgressBarObjects.InstallingPercentage = ProgressBarObjects.InstallingPercentage + 25;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
	ProgressBar::updateProgressText(63, 183, ProgressTextPercentageObjects.InstallingUpdatesPercentage, AppResStringsObjects.InstallingUpdatesText);

	// Finished
	Sleep(2000);
	ProgressBarObjects.InstallingPercentage = 100;
	::SendMessageW(MainObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
	::SendMessageW(MainObjects.hWndSetupWindow, SETUPWND_INSTALL_FINISH, (WPARAM)(INT)0, 0);
}

// Caller install function
void Install::InstallMain()
{
	wchar_t srcImage[MAX_PATH];
	wcsncpy_s(srcImage, ImageInstallObjects.installSources, sizeof(srcImage));
	wcsncat_s(srcImage, L"\\install.wim", sizeof(srcImage));

	ImageInstallObjects.srcImage = srcImage;
	ImageInstallObjects.destDrive = L"C:\\Users\\Genki\\Desktop\\apply";
	ImageInstallObjects.ImageIndex = 1;
	std::thread InstallCode(InstallMain);
	InstallCode.detach();
}