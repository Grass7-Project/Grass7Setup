#pragma once

class ButtonGUI{
public:
	static void				InitButtonBitmaps();
	static void				InitBackBtn();
	static void				InitCloseBtn();
	static void				InitNormalBtn();
	static void				InitAutoPartitionButton();
	static void				InitManualPartitionButton();
	static void				Paint(HWND &hWnd, int &drawButton, HBITMAP &hButtonImg, int xBmpPos, int yBmpPos, int drawText, wchar_t *text, int customTextXY, int textX, int textY);
	static void				ChangeBitmapState(HWND &hWnd, int &ButtonDisabled, int &drawButton, HBITMAP &hButtonTmpImg, HBITMAP &hButtonImg, int setState);
	static LRESULT CALLBACK	BackButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
	static LRESULT CALLBACK	CloseButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
	static LRESULT CALLBACK	NormalButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
	static LRESULT CALLBACK	AutoPartButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
	static LRESULT CALLBACK	ManualPartButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

private:
	HBITMAP hBackBtnTmpImg, // Back Button Temporary Bitmap
		hCloseBtnTmpImg, // Close Button Temporary Bitmap
		hNormalBtnTmpImg, // Normal Button Temporary Bitmap
		hAutoPartBtnTmpImg, // Automatic Partitioning Button Temporary Bitmap
		hManualPartBtnTmpImg; // Manual Partitioning Button Temporary Bitmap

	HBITMAP hbmpWndScreenshot;
	HDC hdcWndScreenshot,
		hdcMainWnd;

	int drawBackButton,
		drawCloseButton,
		drawNormalButton,
		drawAutoPartButton,
		drawManualPartButton;

	BOOLEAN BackButtonTracking,
		BackButtonHover,
		CloseButtonTracking,
		CloseButtonHover,
		NormalButtonTracking,
		NormalButtonHover,
		AutoPartButtonTracking,
		AutoPartButtonHover,
		ManualPartButtonTracking,
		ManualPartButtonHover;
};