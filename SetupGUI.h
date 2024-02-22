#pragma once

class SetupGUI {
public:
	static void Init(HWND &hWnd, HWND &hWndParent, HINSTANCE &hInst);
private:
	static		LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
};