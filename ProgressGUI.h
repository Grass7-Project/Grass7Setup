#pragma once

class ProgressGUI {
public:
	static void createProgressBar();
	static void createProgressBarNew(HWND &hWndParent, HINSTANCE &hInst, HWND &hWndProgressBar, int xPos, int yPos, int Width, int Height, int &Percentage, COLORREF BarColor, COLORREF BackgroundColor, int MessageID);
	static void updateProgressBar(HWND &hParentWnd, HWND &hProgressBarWnd, int &ProgressBarPercentage);
	static void createProgressText();
	static void updateProgressText(HWND &hWnd, int x, int y, int &ProgressPercantage, std::wstring &Text, COLORREF txtColor, int AddPercentage, std::wstring ExtraText, int cWeight);
	static void paintTextBelowProgressBar(HWND &hWnd, const int &x, const int &y, std::wstring Text, const int &FontSize);
	static void WaitThread();
};