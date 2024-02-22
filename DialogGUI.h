#pragma once

class DialogGUI {
public:
	static void Init();
	static void SetRichEditPos();
	static void Update();

private:
	static void Paint(HWND &hWnd);
	static		LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

	HWND hWndRichEditCtrl;
	int RichEditCtrlX,
		RichEditCtrlY;
};