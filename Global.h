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
	wchar_t *OptionSelectionText;
	wchar_t *CollectingInfoText;
	wchar_t *InstallingText;
	wchar_t *CopyingFilesText;
	wchar_t *ExpandingFilesText;
	wchar_t *InstallingFeaturesText;
	wchar_t *InstallingUpdatesText;
	wchar_t *InstallingTitleText;
	wchar_t *RestartingTitleText;
};

// Image installation vairiables
class GlobalImageInstall {
public:
	int ImageIndex;
	wchar_t *ImagePath;
	wchar_t *destDrive;
	wchar_t *installSources;
};

extern GlobalMain MainObjects;
extern GlobalButtons ButtonObjects;
extern GlobalRichEditControl RichEditControlObjects;
extern GlobalProgressBar ProgressBarObjects;
extern GlobalProgressTextPercentage ProgressTextPercentageObjects;
extern GlobalAppResStrings AppResStringsObjects;
extern GlobalImageInstall ImageInstallObjects;