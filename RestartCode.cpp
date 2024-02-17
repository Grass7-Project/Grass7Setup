#include "stdafx.h"
#include "RestartCode.h"
#include "ProgressGUI.h"
#include "MainGUI.h"

// Internal restarting now code
void Restart::InitiateRestart()
{
	std::thread PaintText(Restart::PaintText);
	PaintText.detach();
	std::thread RestartThread(Restart::MainRestartThread);
	RestartThread.detach();
}

// Internal restarting now code
void Restart::MainRestartThread()
{
	while (ProgressBarObjects.RestartingPercentage != 100) {
		Sleep(100);
		ProgressBarObjects.RestartingPercentage = ProgressBarObjects.RestartingPercentage + 1;
		::SendMessageW(MainObjects.hWndSetupWindow, SETUPWND_RESTARTING_TIMER, (WPARAM)(INT)0, 0);
	}
	Sleep(1000);
	SendMessageW(MainObjects.hWndMainWindow, WM_CLOSE, (WPARAM)(INT)0, 0);
}

void Restart::PaintText()
{
	int timer = 10;
	while (timer != 0) {
		Restart::CreateTimerText(MainObjects.hWndSetupWindow, 42, 80, timer);
		Sleep(1000);
		timer = timer - 1;
	}
	timer = 0;
	Restart::CreateTimerText(MainObjects.hWndSetupWindow, 42, 80, timer);
}

void Restart::CreateTimerText(HWND &hWnd, const int &xPos, const int &yPos, const int &timer)
{
	std::wstring TimerText = L"Restarting in ";
	std::wstring second(MAX_PATH, 0);

	second.resize((size_t)swprintf_s(&second[0], second.size(), L"%d", timer));

	TimerText.append(second);
	TimerText.append(L" seconds");
	TimerText.append(L"   ");

	HDC hdc = ::GetDC(MainObjects.hWndSetupWindow);
	PaintTextOptions PaintTextOpt;
	PaintTextOpt.xPos = xPos;
	PaintTextOpt.yPos = yPos;
	PaintTextOpt.font = L"Segoe UI";
	PaintTextOpt.color = RGB(0, 0, 0);
	PaintTextOpt.text = TimerText.c_str();
	PaintTextOpt.nSize = 9;
	PaintTextOpt.BkMode = OPAQUE;
	PaintTextOpt.cWeight = FW_LIGHT;

	gr7::PaintText(hdc, PaintTextOpt);
	ReleaseDC(MainObjects.hWndSetupWindow, hdc);
	::UpdateWindow(MainObjects.hWndSetupWindow);
}