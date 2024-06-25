#pragma once

class MainInit {
public:
	static int Init(MSG &msg, HINSTANCE &hInstance, HINSTANCE &hPrevInstance, LPTSTR &lpCmdLine, int &nCmdShow);
	static BOOL InitGUI();
	static void Exit();
};