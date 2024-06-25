#include "stdafx.h"
#include "RecoveryLauncher.h"

void RecoveryLauncher::LaunchRecovery()
{
	PROCESS_INFORMATION processInfo;
	STARTUPINFO info = { sizeof(info) };
	std::wstring recoveryexe = L"\"X:\\sources\\recovery\\recenv.exe\"";
	if (CreateProcessW(NULL, &recoveryexe[0], NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo))
	{
		WaitForSingleObject(processInfo.hProcess, INFINITE);
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}
}