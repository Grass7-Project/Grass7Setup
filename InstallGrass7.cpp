#include "stdafx.h"
#include "InstallGrass7.h"
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
		GlobalObjects.ExpandingFilesPercentage = (INT)TO_PERCENT(info->extract.completed_bytes, info->extract.total_bytes);
		::SendMessageW(GlobalObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
		ProgressBar::updateProgressText(63, 143, GlobalObjects.ExpandingFilesPercentage, gr7::LoadStringToW(GlobalObjects.hInst, IDS_EXPANDING_FILES));
		break;
	default:
		break;
	}
	return WIMLIB_PROGRESS_STATUS_CONTINUE;
}
int ApplyImage()
{
	int ret;
	WIMStruct *wim = NULL;

	ret = wimlib_open_wim(GlobalObjects.srcImage, 0, &wim);

	if (ret != 0)
	{
		wimlib_global_cleanup();
		return ret;
	}

	wimlib_register_progress_function(wim, extract_progress, NULL);

	ret = wimlib_extract_image(wim, GlobalObjects.ImageIndex, GlobalObjects.destDrive, 0);

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
	GlobalObjects.CopyingFilesPercentage = 0;
	GlobalObjects.ExpandingFilesPercentage = 0;
	GlobalObjects.InstallingFeaturesPercentage = 0;
	GlobalObjects.InstallingUpdatesPercentage = 0;
	GlobalObjects.InstallingPercentage = 0;
	ProgressBar::updateProgressText(63, 123, GlobalObjects.CopyingFilesPercentage, gr7::LoadStringToW(GlobalObjects.hInst, IDS_COPYING_FILES));
	ProgressBar::updateProgressText(63, 143, GlobalObjects.ExpandingFilesPercentage, gr7::LoadStringToW(GlobalObjects.hInst, IDS_EXPANDING_FILES));
	ProgressBar::updateProgressText(63, 163, GlobalObjects.InstallingFeaturesPercentage, gr7::LoadStringToW(GlobalObjects.hInst, IDS_INSTALLING_FEATURES));
	ProgressBar::updateProgressText(63, 183, GlobalObjects.InstallingUpdatesPercentage, gr7::LoadStringToW(GlobalObjects.hInst, IDS_INSTALLING_UPDATES));

	// Copying files
	Sleep(5000);
	GlobalObjects.CopyingFilesPercentage = 100;
	GlobalObjects.InstallingPercentage = GlobalObjects.InstallingPercentage + 25;
	::SendMessageW(GlobalObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
	ProgressBar::updateProgressText(63, 123, GlobalObjects.CopyingFilesPercentage, gr7::LoadStringToW(GlobalObjects.hInst, IDS_COPYING_FILES));

	// Expanding files
	int err = ApplyImage();
	if (err != 0) {

	}
	GlobalObjects.ExpandingFilesPercentage = 100;
	GlobalObjects.InstallingPercentage = GlobalObjects.InstallingPercentage + 25;
	::SendMessageW(GlobalObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
	ProgressBar::updateProgressText(63, 143, GlobalObjects.ExpandingFilesPercentage, gr7::LoadStringToW(GlobalObjects.hInst, IDS_EXPANDING_FILES));

	// Installing Features
	Sleep(5000);
	GlobalObjects.InstallingFeaturesPercentage = 100;
	GlobalObjects.InstallingPercentage = GlobalObjects.InstallingPercentage + 25;
	::SendMessageW(GlobalObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
	ProgressBar::updateProgressText(63, 163, GlobalObjects.InstallingFeaturesPercentage, gr7::LoadStringToW(GlobalObjects.hInst, IDS_INSTALLING_FEATURES));

	// Installing Updates
	Sleep(5000);
	GlobalObjects.InstallingUpdatesPercentage = 100;
	GlobalObjects.InstallingPercentage = GlobalObjects.InstallingPercentage + 25;
	::SendMessageW(GlobalObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
	ProgressBar::updateProgressText(63, 183, GlobalObjects.InstallingUpdatesPercentage, gr7::LoadStringToW(GlobalObjects.hInst, IDS_INSTALLING_UPDATES));

	// Finished
	Sleep(2000);
	GlobalObjects.InstallingPercentage = 100;
	::SendMessageW(GlobalObjects.hWndMainWindow, MAINWND_UPDATE_PROG_BAR, (WPARAM)(INT)0, 0);
	::SendMessageW(GlobalObjects.hWndSetupWindow, SETUPWND_INSTALL_FINISH, (WPARAM)(INT)0, 0);
}

// Caller install function
void InstallGrass7()
{
	wchar_t srcImage[MAX_PATH];
	wcsncpy_s(srcImage, GlobalObjects.installSources, sizeof(srcImage));
	wcsncat_s(srcImage, L"\\install.wim", sizeof(srcImage));

	GlobalObjects.srcImage = srcImage;
	GlobalObjects.destDrive = L"C:\\Users\\Genki\\Desktop\\apply";
	GlobalObjects.ImageIndex = 1;
	std::thread InstallCode(InstallMain);
	InstallCode.detach();
}