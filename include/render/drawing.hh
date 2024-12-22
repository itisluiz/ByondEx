#pragma once
#include <d3d9.h>

namespace render::drawing
{
	HRESULT draw(IDirect3DDevice9* pDevice);
	void deinit();
}
