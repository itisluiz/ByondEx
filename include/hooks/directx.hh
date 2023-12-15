#pragma once
#include <d3d9.h>

namespace hooks::directx
{
	namespace release
	{
		HRESULT __stdcall hook(IDirect3DDevice9* pDevice);
		extern decltype(&hook) address;
		extern decltype(&hook) tramp;
	}

	namespace reset
	{
		HRESULT __stdcall hook(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
		extern decltype(&hook) address;
		extern decltype(&hook) tramp;
	}

	namespace present
	{
		HRESULT __stdcall hook(IDirect3DDevice9* pDevice, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion);
		extern decltype(&hook) address;
		extern decltype(&hook) tramp;
	}

	bool setup();
	void deinit();

}
