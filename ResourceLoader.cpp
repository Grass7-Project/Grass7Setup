#include "stdafx.h"
#include "ResourceLoader.h"
#include "MainGUI.h"

void ResourceLoader::LoadBitmaps()
{
	// Background
	BitmapObjects.hBackground = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_BACKGROUND_BMP), IMAGE_BITMAP, 0, 0, 0));

	// Setup Window Border
	BitmapObjects.hFakeWindow = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_FAKEWND_BMP), IMAGE_BITMAP, 0, 0, 0));

	// Banner
	BitmapObjects.hBanner = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_BANNER_BMP), IMAGE_BITMAP, 0, 0, 0));

	// Logo next to the "Install Grass7" text
	BitmapObjects.hSmallLogo = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_SMALLLOGO_BMP), IMAGE_BITMAP, 0, 0, 0));
	
	// Bottom panel, on top of it is the next/install button
	BitmapObjects.hBottomPanel = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_BOTTOM_PANEL_BMP), IMAGE_BITMAP, 0, 0, 0));

	// Checkmark
	BitmapObjects.hCheckmark = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_CHECKMARK_BMP), IMAGE_BITMAP, 0, 0, 0));

	// Normal Button States
	BitmapObjects.hBackBtnImg1 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_BACK_BTN1_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hCloseBtnImg1 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_CLOSE_BTN1_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hNormalBtnImg1 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_NORMAL_BTN1_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hAutoPartBtnImg1 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_AUTOPART_BTN1_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hManualPartBtnImg1 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_MANUALPART_BTN1_BMP), IMAGE_BITMAP, 0, 0, 0));

	// Pressed Button States
	BitmapObjects.hBackBtnImg2 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_BACK_BTN2_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hCloseBtnImg2 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_CLOSE_BTN2_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hNormalBtnImg2 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_NORMAL_BTN2_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hAutoPartBtnImg2 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_AUTOPART_BTN2_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hManualPartBtnImg2 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_MANUALPART_BTN2_BMP), IMAGE_BITMAP, 0, 0, 0));

	// Disabled Button States
	BitmapObjects.hBackBtnImg3 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_BACK_BTN3_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hCloseBtnImg3 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_CLOSE_BTN3_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hNormalBtnImg3 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_NORMAL_BTN3_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hAutoPartBtnImg3 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_AUTOPART_BTN3_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hManualPartBtnImg3 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_MANUALPART_BTN3_BMP), IMAGE_BITMAP, 0, 0, 0));

	// Hover Button States
	BitmapObjects.hBackBtnImg4 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_BACK_BTN4_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hCloseBtnImg4 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_CLOSE_BTN4_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hNormalBtnImg4 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_NORMAL_BTN4_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hAutoPartBtnImg4 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_AUTOPART_BTN4_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hManualPartBtnImg4 = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_MANUALPART_BTN4_BMP), IMAGE_BITMAP, 0, 0, 0));
}

// Load strings
void ResourceLoader::LoadStrings()
{
	// Init variables
	std::wstring AppTitleText(MAX_PATH, 0);
	std::wstring TitleBarText(MAX_PATH, 0);

	std::wstring EulaTitleText(MAX_PATH, 0);
	std::wstring ChangelogTitleText(MAX_PATH, 0);
	std::wstring PartitionTitleText(MAX_PATH, 0);
	std::wstring InstallingTitleText(MAX_PATH, 0);
	std::wstring RestartingTitleText(MAX_PATH, 0);

	std::wstring NextButtonText(MAX_PATH, 0);
	std::wstring InstallButtonText(MAX_PATH, 0);
	std::wstring AutoPartButtonText(MAX_PATH, 0);
	std::wstring ManualPartButtonText(MAX_PATH, 0);

	std::wstring ProgressBarText1(MAX_PATH, 0);
	std::wstring ProgressBarText2(MAX_PATH, 0);

	std::wstring CollectingInfoText(MAX_PATH, 0);
	std::wstring CopyingFilesText(MAX_PATH, 0);
	std::wstring ExpandingFilesText(MAX_PATH, 0);
	std::wstring InstallingFeaturesText(MAX_PATH, 0);
	std::wstring InstallingUpdatesText(MAX_PATH, 0);

	std::wstring SetupExit(MAX_PATH, 0);
	std::wstring SetupExitDuringSetup(MAX_PATH, 0);

	// Basic application strings
	AppTitleText.resize((size_t)LoadStringW(MainObjects.hInst, IDS_APP_TITLE, &AppTitleText[0], (int)AppTitleText.size()));
	TitleBarText.resize((size_t)LoadStringW(MainObjects.hInst, IDS_TITLEBAR, &TitleBarText[0], (int)TitleBarText.size()));

	// Dialog title strings
	EulaTitleText.resize(LoadStringW(MainObjects.hInst, IDS_EULA_TITLE, &EulaTitleText[0], (int)EulaTitleText.size()));
	ChangelogTitleText.resize(LoadStringW(MainObjects.hInst, IDS_CHANGELOG_TITLE, &ChangelogTitleText[0], (int)ChangelogTitleText.size()));
	PartitionTitleText.resize(LoadStringW(MainObjects.hInst, IDS_PARTITION_TITLE, &PartitionTitleText[0], (int)PartitionTitleText.size()));
	InstallingTitleText.resize(LoadStringW(MainObjects.hInst, IDS_INSTALLING_TITLE, &InstallingTitleText[0], (int)InstallingTitleText.size()));
	RestartingTitleText.resize(LoadStringW(MainObjects.hInst, IDS_RESTARTING_TITLE, &RestartingTitleText[0], (int)RestartingTitleText.size()));

	// Button strings
	NextButtonText.resize(LoadStringW(MainObjects.hInst, IDS_NEXTBTN, &NextButtonText[0], (int)NextButtonText.size()));
	InstallButtonText.resize(LoadStringW(MainObjects.hInst, IDS_INSTALLBTN, &InstallButtonText[0], (int)InstallButtonText.size()));
	AutoPartButtonText.resize(LoadStringW(MainObjects.hInst, IDS_AUTOPARTBTN, &AutoPartButtonText[0], (int)AutoPartButtonText.size()));
	ManualPartButtonText.resize(LoadStringW(MainObjects.hInst, IDS_MANUALPARTBTN, &ManualPartButtonText[0], (int)ManualPartButtonText.size()));

	// Progress Bar strings
	ProgressBarText1.resize(LoadStringW(MainObjects.hInst, IDS_PROGBAR_TEXT1, &ProgressBarText1[0], (int)ProgressBarText1.size()));
	ProgressBarText2.resize(LoadStringW(MainObjects.hInst, IDS_PROGBAR_TEXT2, &ProgressBarText2[0], (int)ProgressBarText2.size()));

	// Installation progress strings
	CollectingInfoText.resize(LoadStringW(MainObjects.hInst, IDS_COLLECTING_INFO, &CollectingInfoText[0], (int)CollectingInfoText.size()));
	CopyingFilesText.resize(LoadStringW(MainObjects.hInst, IDS_COPYING_FILES, &CopyingFilesText[0], (int)CopyingFilesText.size()));
	ExpandingFilesText.resize(LoadStringW(MainObjects.hInst, IDS_EXPANDING_FILES, &ExpandingFilesText[0], (int)ExpandingFilesText.size()));
	InstallingFeaturesText.resize(LoadStringW(MainObjects.hInst, IDS_INSTALLING_FEATURES, &InstallingFeaturesText[0], (int)InstallingFeaturesText.size()));
	InstallingUpdatesText.resize(LoadStringW(MainObjects.hInst, IDS_INSTALLING_UPDATES, &InstallingUpdatesText[0], (int)InstallingUpdatesText.size()));

	// Setup information strings
	SetupExit.resize(LoadStringW(MainObjects.hInst, IDS_SETUP_EXIT, &SetupExit[0], (int)SetupExit.size()));
	SetupExitDuringSetup.resize(LoadStringW(MainObjects.hInst, IDS_SETUP_EXIT_PROGRESS, &SetupExitDuringSetup[0], (int)SetupExitDuringSetup.size()));

	// Set loaded wstrings
	AppResStringsObjects.AppTitleText = AppTitleText;
	AppResStringsObjects.TitleBarText = TitleBarText;

	AppResStringsObjects.EulaTitleText = EulaTitleText;
	AppResStringsObjects.ChangelogTitleText = ChangelogTitleText;
	AppResStringsObjects.PartitionTitleText = PartitionTitleText;
	AppResStringsObjects.InstallingTitleText = InstallingTitleText;
	AppResStringsObjects.RestartingTitleText = RestartingTitleText;

	AppResStringsObjects.NextButtonText = NextButtonText;
	AppResStringsObjects.InstallButtonText = InstallButtonText;
	AppResStringsObjects.AutoPartButtonText = AutoPartButtonText;
	AppResStringsObjects.ManualPartButtonText = ManualPartButtonText;

	AppResStringsObjects.ProgressBarText1 = ProgressBarText1;
	AppResStringsObjects.ProgressBarText2 = ProgressBarText2;

	AppResStringsObjects.CollectingInfoText = CollectingInfoText;
	AppResStringsObjects.CopyingFilesText = CopyingFilesText;
	AppResStringsObjects.ExpandingFilesText = ExpandingFilesText;
	AppResStringsObjects.InstallingFeaturesText = InstallingFeaturesText;
	AppResStringsObjects.InstallingUpdatesText = InstallingUpdatesText;

	AppResStringsObjects.SetupExit = SetupExit;
	AppResStringsObjects.SetupExitDuringSetup = SetupExitDuringSetup;
}