#pragma once

class GUI{
public:
	static BOOL	InitInstance();
	static ATOM	RegisterClasses();

	static		LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
	static		LRESULT CALLBACK	WndProcSetupWnd(HWND, UINT, WPARAM, LPARAM);
	static		LRESULT CALLBACK	WndProcDialogWnd(HWND, UINT, WPARAM, LPARAM);

	static void	GetDesktopResolution(int& horizontal, int& vertical);
	static void RestartSoon();
	static void DialogPaintCode();

private:
	HBITMAP hBackground;
	HBITMAP hFakeWindow;
	HBITMAP hBanner;
	HBITMAP hSmallLogo;
	HBITMAP hBottomPanel;
	int doNotClose;
};