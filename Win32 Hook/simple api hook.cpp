#include "hook.h"
#include <Windows.h>

Hook myhook;

int WINAPI MyMessageBoxW(HWND, LPCWSTR, LPCWSTR, UINT) {
	myhook.Clear();
	int val = MessageBoxW(NULL, L"AASS", L"X", NULL);
	myhook.Restore();
	return val;
}

int main() {

	myhook.Init(L"user32.dll", "MessageBoxW", MyMessageBoxW);
	
	MessageBoxW(NULL, L"My Message", L"NULL", NULL);

	return 0;
}


