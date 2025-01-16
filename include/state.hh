#pragma once
#include <Windows.h>

namespace state
{
	extern HMODULE hModule;
	extern HWND hRenderWindow;
	extern HWND hDragWindow;
	extern bool unloading;

	bool setup();
	void deinit();
}
