#pragma once

class GUI{
public:
	static BOOL	InitInstance();
	static ATOM	RegisterClasses();

	static		LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
	static		LRESULT CALLBACK	WndProcSetupWnd(HWND, UINT, WPARAM, LPARAM);
	static		LRESULT CALLBACK	WndProcDialogWnd(HWND, UINT, WPARAM, LPARAM);

	static void	GetDesktopResolution(int& horizontal, int& vertical);
	static void LoadStrings();
	static void RestartSoon();
	static void DialogPaintCode();

private:
	HBITMAP hBackground,
		hFakeWindow,
		hBanner,
		hSmallLogo,
		hBottomPanel;
	int doNotClose;
};