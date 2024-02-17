#pragma once

class Restart {
public:
	static void InitiateRestart();
private:
	static void MainRestartThread();
	static void PaintText();
	static void CreateTimerText(HWND &hWnd, const int &xPos, const int &yPos, const int &timer);
};