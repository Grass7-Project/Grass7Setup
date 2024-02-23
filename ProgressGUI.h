#pragma once

class ProgressGUI {
public:
	static void createProgressBar();
	static void createProgressBarNew(HWND &hWndParent, HINSTANCE &hInst, HWND &hWndProgressBar, int xPos, int yPos, int Width, int Height, int &Percentage, COLORREF BarColor, COLORREF BackgroundColor, int MessageID);
	static void updateProgressBar(HWND &hParentWnd, HWND &hProgressBarWnd, int &ProgressBarPercentage);
	static void createProgressText(HDC &hdc, int &percent1, int &percent2, int &percent3, int &percent4, std::wstring &Text1, std::wstring &Text2, std::wstring &Text3, std::wstring &Text4);
	static void updateProgressTexthWnd(HWND &hWnd, int x, int y, int &ProgressPercantage, std::wstring &Text, COLORREF txtColor, int AddPercentage, std::wstring ExtraText, int cWeight);
	static void updateProgressText(HDC &hdc, int x, int y, int &ProgressPercantage, std::wstring &Text, COLORREF txtColor, int AddPercentage, std::wstring ExtraText, int cWeight);
	static void WaitThread(BOOL &WaitThreadRunning, BOOL &WaitThreadExit, int &Percentage, std::wstring &Text, int xPos, int yPos);
};