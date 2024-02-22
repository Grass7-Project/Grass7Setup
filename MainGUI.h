#pragma once

class MainGUI{
public:
	static void Init(HWND &hWnd, HINSTANCE &hInst);

private:
	static void Paint(HWND &hWnd);
	static		LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
};