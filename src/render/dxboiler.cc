#include <render/dxboiler.hh>
#include <render/drawing.hh>
#include <state.hh>

namespace
{
	IDirect3DSwapChain9* pSwapChain{ nullptr };
	IDirect3DSurface9* pBackBuffer{ nullptr };

	HRESULT setupSwapChain(IDirect3DDevice9* pDevice)
	{
		HRESULT hResult{ D3D_OK };

		D3DDEVICE_CREATION_PARAMETERS cp;
		hResult = pDevice->GetCreationParameters(&cp);
		if (hResult != D3D_OK)
			return hResult;

		RECT clientRect;
		state::hRenderWindow = cp.hFocusWindow;
		if (!GetClientRect(cp.hFocusWindow, &clientRect))
			return E_FAIL;

		D3DPRESENT_PARAMETERS d3dpp{ };
		d3dpp.BackBufferWidth = clientRect.right - clientRect.left;
		d3dpp.BackBufferHeight = clientRect.bottom - clientRect.top;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.Windowed = static_cast<bool>(!(GetWindowLongPtrW(cp.hFocusWindow, GWL_STYLE) & WS_POPUP));

		hResult = pDevice->CreateAdditionalSwapChain(&d3dpp, &pSwapChain);
		if (hResult != D3D_OK)
			return hResult;

		hResult = pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
		return hResult;
	}
}

namespace render::dxboiler
{
	HRESULT present(IDirect3DDevice9* pDevice, RECT* pDestRect)
	{
		HRESULT hResult{ D3D_OK };

		IDirect3DSurface9* pOriginalSurface{ nullptr };
		hResult = pDevice->GetRenderTarget(0, &pOriginalSurface);
		if (hResult != D3D_OK)
			return hResult;

		if (!pSwapChain)
		{
			hResult = setupSwapChain(pDevice);
			if (hResult != D3D_OK)
			{
				pOriginalSurface->Release();
				return hResult;
			}
		}

		hResult = pDevice->SetRenderTarget(0, pBackBuffer);
		if (hResult != D3D_OK)
		{
			pOriginalSurface->Release();
			return hResult;
		}

		hResult = pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		if (hResult != D3D_OK)
		{
			pOriginalSurface->Release();
			return hResult;
		}

		pDevice->BeginScene();

		hResult = pDevice->StretchRect(pOriginalSurface, NULL, pBackBuffer, pDestRect, D3DTEXF_NONE);
		if (hResult != D3D_OK)
		{
			pOriginalSurface->Release();
			return hResult;
		}

		hResult = render::drawing::draw(pDevice);

		pDevice->EndScene();

		if (hResult != D3D_OK)
		{
			pOriginalSurface->Release();
			return hResult;
		}

		hResult = pDevice->SetRenderTarget(0, pOriginalSurface);
		if (hResult != D3D_OK)
		{
			pOriginalSurface->Release();
			return hResult;
		}

		hResult = pDevice->SetRenderTarget(0, pOriginalSurface);
		if (hResult != D3D_OK)
		{
			pOriginalSurface->Release();
			return hResult;
		}

		hResult = pSwapChain->Present(NULL, NULL, NULL, NULL, NULL);

		pOriginalSurface->Release();
		return hResult;
	}

	void deinit()
	{
		render::drawing::deinit();

		if (pBackBuffer)
		{
			pBackBuffer->Release();
			pBackBuffer = nullptr;
		}

		if (pSwapChain)
		{
			pSwapChain->Release();
			pSwapChain = nullptr;
		}
	}
}
