#pragma once

class Global {
public:
	
	HINSTANCE hInst;

	// Main Window
	HWND hWndMainWindow;

	// Setup Window
	HWND hWndSetupWindow;

	// Dialog Window
	HWND hWndDialogWindow;
	int Page;

	// Progress Bar
	HWND hProgressCtrlCollectingInfo;
	HWND hProgressCtrlInstalling;
	int CollectingInfoPercentage, InstallingPercentage;

	// Back Button
	HWND hBackBtn;
	int BackButtonDisabled;

	// Close Button
	HWND hCloseBtn;
	int CloseButtonDisabled;

	// Normal Button
	HWND hNormalBtn;
	int NormalButtonState;
	int NormalButtonDisabled;
	int InstallButtonText;

	// Rich Edit Control
	HWND hWndRichEditCtrl;
	int RichEditCtrlX;
	int RichEditCtrlY;

	// Progress Text Percentage
	int CopyingFilesPercentage, ExpandingFilesPercentage, InstallingFeaturesPercentage, InstallingUpdatesPercentage;

	// Image Installation
	int ImageIndex;
	wchar_t *srcImage;
	wchar_t *destDrive;
	wchar_t *installSources;
};

// Make this class global for all code files
extern Global GlobalInstance;
extern Global GlobalObjects;