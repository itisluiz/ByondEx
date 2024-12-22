#pragma once
#include <d3d9.h>

namespace render::dxboiler
{
	HRESULT present(IDirect3DDevice9* pDevice, RECT* pDestRect);
	void deinit();
}
