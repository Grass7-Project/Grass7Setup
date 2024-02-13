#pragma once

class ProgressGUI {
public:
	static void createProgressBar();
	static void updateProgressBar();
	static void createProgressText();
	static void updateProgressText(int x, int y, int &ProgressPercantage, std::wstring Text, COLORREF txtColor, int AddPercentage, std::wstring ExtraText);
	static void paintTextBelowProgressBar(HWND hWnd, int x, int y, std::wstring Text, int FontSize);
	static void WaitThread();
};