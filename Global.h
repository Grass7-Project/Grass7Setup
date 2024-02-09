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
	wchar_t *AppTitleText;
	wchar_t *TitleBarText;
	wchar_t *ProgressBarText1;
	wchar_t *ProgressBarText2;
	wchar_t *NextButtonText;
	wchar_t *InstallButtonText;
	wchar_t *EulaTitleText;
	wchar_t *ChangelogTitleText;
	wchar_t *PartitionTitleText;
	wchar_t *InstallOptionTitleText;
	wchar_t *CollectingInfoText;
	wchar_t *InstallingText;
	wchar_t *CopyingFilesText;
	wchar_t *ExpandingFilesText;
	wchar_t *InstallingFeaturesText;
	wchar_t *InstallingUpdatesText;
	wchar_t *InstallingTitleText;
	wchar_t *RestartingTitleText;
	wchar_t *AutoPartButtonText;
	wchar_t *ManualPartButtonText;
	wchar_t *CompatibilityErrorText;
	wchar_t *RunInWinPERequiredErrorText;
	wchar_t *NoInstallImageFoundErrorText;
	wchar_t *ApplyInstallImageErrorText;
};

// Image installation variables
class GlobalImageInstall {
public:
	int ImageIndex;
	wchar_t *ImagePath;
	wchar_t *destDrive;
	wchar_t *installSources;
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