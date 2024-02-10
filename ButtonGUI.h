#pragma once

class ButtonGUI{
public:
	static void				InitButtonBitmaps();
	static void				InitBackBtn();
	static void				InitCloseBtn();
	static void				InitNormalBtn();
	static void				InitAutoPartitionButton();
	static void				InitManualPartitionButton();
	static void				Paint(HWND &hWnd, int &drawButton, HBITMAP &hButtonImg, int xBmpPos, int yBmpPos, int drawText, LPCWSTR text, int customTextXY, int textX, int textY);
	static void				ChangeBitmapState(HWND &hWnd, int &ButtonDisabled, int &drawButton, HBITMAP &hButtonTmpImg, HBITMAP &hButtonImg, int setState);
	static LRESULT CALLBACK	BackButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
	static LRESULT CALLBACK	CloseButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
	static LRESULT CALLBACK	NormalButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
	static LRESULT CALLBACK	AutoPartButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
	static LRESULT CALLBACK	ManualPartButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

private:
	// Temporary bitmaps, used for making the buttons look transparent
	HBITMAP hBackBtnTmpImg,
		hCloseBtnTmpImg,
		hNormalBtnTmpImg,
		hAutoPartBtnTmpImg,
		hManualPartBtnTmpImg;

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