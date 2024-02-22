#pragma once

#define MAX_LOADSTRING 100
#define WINDOWS_VISTA_SEVEN_MAJORVERSION	6
#define WINDOWS_XP_MAJORVERSION				5

class MainInit {
public:
	static int Init(MSG &msg, HINSTANCE &hInstance, HINSTANCE &hPrevInstance, LPTSTR &lpCmdLine, int &nCmdShow);
	static BOOL InitGUI();
	static void Exit();
};