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

	HBITMAP hBackBtnImg1;
	HBITMAP hBackBtnImg2;
	HBITMAP hBackBtnImg3;
	HBITMAP hBackBtnImg4;

	HBITMAP hCloseBtnImg1;
	HBITMAP hCloseBtnImg2;
	HBITMAP hCloseBtnImg3;
	HBITMAP hCloseBtnImg4;

	HBITMAP hNormalBtnImg1;
	HBITMAP hNormalBtnImg2;
	HBITMAP hNormalBtnImg3;
	HBITMAP hNormalBtnImg4;

	HBITMAP hBackBtnTmpImg;
	HBITMAP hCloseBtnTmpImg;
	HBITMAP hNormalBtnTmpImg;

	HBITMAP hbmpWndScreenshot;
	HDC hdcWndScreenshot;
	HDC hdcMainWnd;

	int drawBackButton, drawCloseButton, drawNormalButton;

	BOOLEAN bButtonTracking, bButtonHover, cButtonTracking, cButtonHover, nButtonTracking, nButtonHover;

};