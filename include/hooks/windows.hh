#pragma once
#include <windows.h>

namespace hooks::windows
{
	namespace wndproc
	{
		LRESULT CALLBACK hook(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		extern WNDPROC originalProc;
	}

	namespace getmsgproc
	{
		LRESULT CALLBACK hook(int nCode, WPARAM wParam, LPARAM lParam);
		extern HHOOK hookHandle;
	}

	bool setup();
	void deinit();

}
