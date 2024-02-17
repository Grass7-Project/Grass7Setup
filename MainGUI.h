#pragma once

class MainGUI{
public:
	static BOOL	InitInstance();
	static void Exit();
	static void DialogPaintCode();

private:
	static		ATOM				RegisterClasses();
	static		LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
	static		LRESULT CALLBACK	WndProcSetupWnd(HWND, UINT, WPARAM, LPARAM);
	static		LRESULT CALLBACK	WndProcDialogWnd(HWND, UINT, WPARAM, LPARAM);

	BOOL doNotClose;
};