#include "stdafx.h"
#include "MainInit.h"
#include <shellapi.h>

GlobalMain MainObjects;
GlobalButtons ButtonObjects;
GlobalProgressBar ProgressBarObjects;
GlobalProgressTextPercentage ProgressTextPercentageObjects;
GlobalAppResStrings AppResStringsObjects;
GlobalImageInstall ImageInstallObjects;
GlobalBitmaps BitmapObjects;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);

	MSG msg;

	int ret = MainInit::Init(msg, hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	if (!ret) {
		return FALSE;
	}

	return (int) msg.wParam;
}