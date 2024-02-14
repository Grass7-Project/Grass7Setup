#pragma once

class MainGUI{
public:
	static BOOL	InitInstance();
	static ATOM	RegisterClasses();
	static void Exit();
	static void DialogPaintCode();

	static		LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
	static		LRESULT CALLBACK	WndProcSetupWnd(HWND, UINT, WPARAM, LPARAM);
	static		LRESULT CALLBACK	WndProcDialogWnd(HWND, UINT, WPARAM, LPARAM);

private:
	int doNotClose;
};