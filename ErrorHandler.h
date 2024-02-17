#pragma once

class ErrorHandler {
public:
	static void InvokeErrorHandler(int ErrorValue, int ErrorMustBe, std::wstring DesiredErrorText, std::wstring DesiredErrorTitle);
};