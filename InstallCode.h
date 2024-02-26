#pragma once

class Install {
public:
	static void InstallMain(HDC &hdc);

private:
	static void InstallerThread();
	static int ApplyImage();
	static enum wimlib_progress_status extract_progress(enum wimlib_progress_msg msg, union wimlib_progress_info *info, void *progctx);

	class Progress {
	public:
		static void CopyingFiles(HWND &hWndParent);
		static void ExpandingFiles(HWND &hWndParent);
		static void InstallingFeatures(HWND &hWndParent);
		static void InstallingUpdates(HWND &hWndParent);
	};

	BOOL WaitThreadRunning,
		WaitThreadExit,
		InstallInProgress,
		ValuesInit;

	std::wstring DotsText;

	int CopyingFilesPercentage,
		ExpandingFilesPercentage,
		InstallingFeaturesPercentage,
		InstallingUpdatesPercentage;
};