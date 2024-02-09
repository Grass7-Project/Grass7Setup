#include "stdafx.h"
#include "ResourceLoader.h"
#include "MainGUI.h"
#include "Global.h"

void ResourceLoader::LoadWindowBitmaps()
{
	BitmapObjects.hBackground = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_BACKGROUND_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hFakeWindow = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_FAKEWND_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hBanner = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_BANNER_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hSmallLogo = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_SMALLLOGO_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hBottomPanel = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_BOTTOM_PANEL_BMP), IMAGE_BITMAP, 0, 0, 0));
	BitmapObjects.hCheckmark = static_cast<HBITMAP>(LoadImageW(MainObjects.hInst, MAKEINTRESOURCE(IDB_CHECKMARK_BMP), IMAGE_BITMAP, 0, 0, 0));
}

void ResourceLoader::LoadButtonBitmaps()
{
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
	// Basic application strings
	AppResStringsObjects.AppTitleText = gr7::LoadStringToW(MainObjects.hInst, IDS_APP_TITLE);
	AppResStringsObjects.TitleBarText = gr7::LoadStringToW(MainObjects.hInst, IDS_TITLEBAR);

	// Dialog title strings
	AppResStringsObjects.EulaTitleText = gr7::LoadStringToW(MainObjects.hInst, IDS_EULA_TITLE);
	AppResStringsObjects.ChangelogTitleText = gr7::LoadStringToW(MainObjects.hInst, IDS_CHANGELOG_TITLE);
	AppResStringsObjects.PartitionTitleText = gr7::LoadStringToW(MainObjects.hInst, IDS_PARTITION_TITLE);
	AppResStringsObjects.InstallingTitleText = gr7::LoadStringToW(MainObjects.hInst, IDS_INSTALLING_TITLE);
	AppResStringsObjects.RestartingTitleText = gr7::LoadStringToW(MainObjects.hInst, IDS_RESTARTING_TITLE);

	// Button strings
	AppResStringsObjects.NextButtonText = gr7::LoadStringToW(MainObjects.hInst, IDS_NEXTBTN);
	AppResStringsObjects.InstallButtonText = gr7::LoadStringToW(MainObjects.hInst, IDS_INSTALLBTN);
	AppResStringsObjects.AutoPartButtonText = gr7::LoadStringToW(MainObjects.hInst, IDS_AUTOPARTBTN);
	AppResStringsObjects.ManualPartButtonText = gr7::LoadStringToW(MainObjects.hInst, IDS_MANUALPARTBTN);

	// Progress Bar strings
	AppResStringsObjects.ProgressBarText1 = gr7::LoadStringToW(MainObjects.hInst, IDS_PROGBAR_TEXT1);
	AppResStringsObjects.ProgressBarText2 = gr7::LoadStringToW(MainObjects.hInst, IDS_PROGBAR_TEXT2);

	// Installation progress strings
	AppResStringsObjects.CollectingInfoText = gr7::LoadStringToW(MainObjects.hInst, IDS_COLLECTING_INFO);
	AppResStringsObjects.CopyingFilesText = gr7::LoadStringToW(MainObjects.hInst, IDS_COPYING_FILES);
	AppResStringsObjects.ExpandingFilesText = gr7::LoadStringToW(MainObjects.hInst, IDS_EXPANDING_FILES);
	AppResStringsObjects.InstallingFeaturesText = gr7::LoadStringToW(MainObjects.hInst, IDS_INSTALLING_FEATURES);
	AppResStringsObjects.InstallingUpdatesText = gr7::LoadStringToW(MainObjects.hInst, IDS_INSTALLING_UPDATES);

	// Error strings
	AppResStringsObjects.CompatibilityErrorText = gr7::LoadStringToW(MainObjects.hInst, IDS_COMPATIBILITY_ERR);
	AppResStringsObjects.RunInWinPERequiredErrorText = gr7::LoadStringToW(MainObjects.hInst, IDS_WINPE_REQUIRED_ERR);
	AppResStringsObjects.NoInstallImageFoundErrorText = gr7::LoadStringToW(MainObjects.hInst, IDS_NO_INSTALL_IMG_ERR);
	AppResStringsObjects.ApplyInstallImageErrorText = gr7::LoadStringToW(MainObjects.hInst, IDS_IMG_APPLY_ERR);
}