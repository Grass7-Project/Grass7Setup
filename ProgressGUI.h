#pragma once

class ProgressGUI {
public:
	static void createProgressBar();
	static void updateProgressBar();
	static void createProgressText();
	static void updateProgressText(int x, int y, int &ProgressPercantage, wchar_t *Text);
	static void paintTextBelowProgressBar(HWND hWnd, int x, int y, wchar_t *Text, int FontSize);
};