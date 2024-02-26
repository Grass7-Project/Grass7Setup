#pragma once

class ButtonGUI{
public:
	static void				InitButtonBitmaps();
	static void				InitBackBtn();
	static void				InitCloseBtn();
	static void				InitNormalBtn();
	static void				InitAutoPartitionButton();
	static void				InitManualPartitionButton();

private:
	static void				Paint(HWND &hWnd, HBITMAP &hButtonImg, int xBmpPos, int yBmpPos, int drawText, LPCWSTR text, int customTextXY, int textX, int textY);
	static void				ChangeBitmapState(HWND &hWnd, BOOL& ButtonDisabled, HBITMAP &hButtonTmpImg, HBITMAP &hButtonImg, BOOL setState);
	static LRESULT CALLBACK	BackButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
	static LRESULT CALLBACK	CloseButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
	static LRESULT CALLBACK	NormalButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
	static LRESULT CALLBACK	AutoPartButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
	static LRESULT CALLBACK	ManualPartButtonProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

	// Temporary bitmaps, used for making the buttons look transparent
	HBITMAP hBackBtnTmpImg,
		hCloseBtnTmpImg,
		hNormalBtnTmpImg,
		hAutoPartBtnTmpImg,
		hManualPartBtnTmpImg;

	HBITMAP hbmpWndScreenshot;
	HDC hdcWndScreenshot,
		hdcMainWnd;

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