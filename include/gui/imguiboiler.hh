#pragma once
#include <d3d9.h>

namespace gui::imguiboiler
{
	bool handleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void beginFrame(IDirect3DDevice9* pDevice);
	void endFrame(IDirect3DDevice9* pDevice);
	void deinit();

}
