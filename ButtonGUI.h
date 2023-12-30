#pragma once

class ButtonGUI{
public:
	static void				InitButtonBitmaps();
	static void				InitBackBtn();
	static void				InitCloseBtn();
	static void				InitNormalBtn();
	static void				Paint(HWND &hWnd, int &drawButton, HBITMAP &hButtonImg, int xBmpPos, int yBmpPos, int drawText, wchar_t *text);
	static void				ChangeBitmapState(HWND &hWnd, int &ButtonDisabled, int &drawButton, HBITMAP &hButtonTmpImg, HBITMAP &hButtonImg, int setState);
	static LRESULT CALLBACK	BackButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
	static LRESULT CALLBACK	CloseButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
	static LRESULT CALLBACK	NormalButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

private:
	HBITMAP hBackBtnImg1, // Back Button Bitmaps
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
		hBackBtnTmpImg, // Back Button Temporary Bitmap
		hCloseBtnTmpImg, // Close Button Temporary Bitmap
		hNormalBtnTmpImg; // Normal Button Temporary Bitmap

	HBITMAP hbmpWndScreenshot;
	HDC hdcWndScreenshot,
		hdcMainWnd;

	int drawBackButton,
		drawCloseButton,
		drawNormalButton;

	BOOLEAN bButtonTracking,
		bButtonHover,
		cButtonTracking,
		cButtonHover,
		nButtonTracking,
		nButtonHover;
};