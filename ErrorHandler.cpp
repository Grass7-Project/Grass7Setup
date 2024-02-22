#include "stdafx.h"

void ErrorHandler::InvokeErrorHandler(int ErrorValue, int ErrorMustBe, std::wstring DesiredErrorText, std::wstring DesiredErrorTitle)
{
	if (ErrorValue != ErrorMustBe) {
		MessageBoxW(NULL,
			DesiredErrorText.c_str(),
			DesiredErrorTitle.c_str(), MB_ICONERROR | MB_OK);
		exit(0);
	}
}