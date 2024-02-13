#pragma once

// Main Class
class GlobalMain {
public:
	HINSTANCE hInst; // Handle to executable

	HWND hWndMainWindow,
		hWndSetupWindow,
		hWndDialogWindow;

	int Page;
};

// Buttons
class GlobalButtons {
public:
	HWND hBackBtn,
		hCloseBtn,
		hNormalBtn,
		hAutoPartitionBtn,
		hManualPartitionBtn;
	int BackButtonDisabled,
		CloseButtonDisabled,
		NormalButtonDisabled,
		NormalButtonState,
		AutoPartButtonDisabled,
		AutoPartButtonState,
		ManualPartButtonDisabled,
		ManualPartButtonState,
		InstallButtonText;
};

// Rich Edit Control
class GlobalRichEditControl {
public:
	HWND hWndRichEditCtrl;
	int RichEditCtrlX,
		RichEditCtrlY;
};

// Main Progress Bar
class GlobalProgressBar {
public:
	HWND hProgressCtrlCollectingInfo,
		hProgressCtrlInstalling;
	int CollectingInfoPercentage,
		InstallingPercentage;
};

// Installation Progress Window Text Percentage Status
class GlobalProgressTextPercentage {
public:
	int CopyingFilesPercentage,
		ExpandingFilesPercentage,
		InstallingFeaturesPercentage,
		InstallingUpdatesPercentage;
};

// Application Resource Strings
class GlobalAppResStrings {
public:
	std::wstring AppTitleText;
	std::wstring TitleBarText;
	std::wstring ProgressBarText1;
	std::wstring ProgressBarText2;
	std::wstring NextButtonText;
	std::wstring InstallButtonText;
	std::wstring EulaTitleText;
	std::wstring ChangelogTitleText;
	std::wstring PartitionTitleText;
	std::wstring InstallOptionTitleText;
	std::wstring CollectingInfoText;
	std::wstring InstallingText;
	std::wstring CopyingFilesText;
	std::wstring ExpandingFilesText;
	std::wstring InstallingFeaturesText;
	std::wstring InstallingUpdatesText;
	std::wstring InstallingTitleText;
	std::wstring RestartingTitleText;
	std::wstring AutoPartButtonText;
	std::wstring ManualPartButtonText;
	std::wstring CompatibilityErrorText;
	std::wstring RunInWinPERequiredErrorText;
	std::wstring NoInstallImageFoundErrorText;
	std::wstring ApplyInstallImageErrorText;
};

// Image installation variables
class GlobalImageInstall {
public:
	int WaitThreadGo;
	int WaitThreadRunning;
	int CopyingFiles;
	int ExpandingFiles;
	int InstallingFeatures;
	int InstallingUpdates;
	int ImageIndex;
	std::wstring ImagePath;
	std::wstring destDrive;
	std::wstring installSources;
};

// Bitmaps
class GlobalBitmaps {
public:
	HBITMAP hBackground,
		hFakeWindow,
		hBanner,
		hSmallLogo,
		hBottomPanel,
		hCheckmark,
		hBackBtnImg1, // Back Button Bitmaps
		hBackBtnImg2,
		hBackBtnImg3,
		hBackBtnImg4,
		hCloseBtnImg1, // Close Button Bitmaps
		hCloseBtnImg2,
		hCloseBtnImg3,
		hCloseBtnImg4,
		hNormalBtnImg1, // Normal Button Bitmaps
		hNormalBtnImg2,
		hNormalBtnImg3,
		hNormalBtnImg4,
		hAutoPartBtnImg1, // Automatic Partitioning Button Bitmaps
		hAutoPartBtnImg2,
		hAutoPartBtnImg3,
		hAutoPartBtnImg4,
		hManualPartBtnImg1, // Manual Partitioning Button Bitmaps
		hManualPartBtnImg2,
		hManualPartBtnImg3,
		hManualPartBtnImg4;
};

extern GlobalMain MainObjects;
extern GlobalButtons ButtonObjects;
extern GlobalRichEditControl RichEditControlObjects;
extern GlobalProgressBar ProgressBarObjects;
extern GlobalProgressTextPercentage ProgressTextPercentageObjects;
extern GlobalAppResStrings AppResStringsObjects;
extern GlobalImageInstall ImageInstallObjects;
extern GlobalBitmaps BitmapObjects;