#include <hooks/directx.hh>
#include <render/dxboiler.hh>
#include <minhook.h>
#include <memory>

namespace
{
	std::unique_ptr<void*[]> vTableIDirect3DDevice9()
	{
		IDirect3D9* pD3D{ Direct3DCreate9(D3D_SDK_VERSION) };

		if (!pD3D)
			return nullptr;

		D3DPRESENT_PARAMETERS d3dpp{ };
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

		IDirect3DDevice9* pDevice{ nullptr };
		if (pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetDesktopWindow(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice) != D3D_OK)
		{
			pD3D->Release();
			return nullptr;
		}

		std::unique_ptr<void*[]> vTableCopy{ std::make_unique<void*[]>(119) };
		memcpy(vTableCopy.get(), *reinterpret_cast<void***>(pDevice), sizeof(void*) * 119);

		pDevice->Release();
		pD3D->Release();

		return vTableCopy;
	}

}

namespace hooks::directx
{
	namespace release
	{
		HRESULT __stdcall hook(IDirect3DDevice9* pDevice)
		{

			return tramp(pDevice);
		}

		decltype(&hook) address;
		decltype(&hook) tramp;
	}

	namespace reset
	{
		HRESULT __stdcall hook(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
		{
			render::dxboiler::deinit();

			return tramp(pDevice, pPresentationParameters);
		}

		decltype(&hook) address;
		decltype(&hook) tramp;
	}

	namespace present
	{
		HRESULT __stdcall hook(IDirect3DDevice9* pDevice, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion)
		{
			HRESULT renderResult{ render::dxboiler::present(pDevice, pDestRect) };

			return renderResult;
		}

		decltype(&hook) address;
		decltype(&hook) tramp;
	}

	bool setup()
	{
		std::unique_ptr<void*[]> vTableDevice9{ vTableIDirect3DDevice9() };

		if (!vTableDevice9)
			return false;

		release::address = reinterpret_cast<decltype(&release::hook)>(vTableDevice9[2]);
		if (MH_CreateHook(release::address, release::hook, reinterpret_cast<LPVOID*>(&release::tramp)) != MH_OK)
			return false;

		reset::address = reinterpret_cast<decltype(&reset::hook)>(vTableDevice9[16]);
		if (MH_CreateHook(reset::address, reset::hook, reinterpret_cast<LPVOID*>(&reset::tramp)) != MH_OK)
			return false;

		present::address = reinterpret_cast<decltype(&present::hook)>(vTableDevice9[17]);
		if (MH_CreateHook(present::address, present::hook, reinterpret_cast<LPVOID*>(&present::tramp)) != MH_OK)
			return false;

		return true;
	}

	void deinit()
	{
		MH_DisableHook(present::address);
		MH_DisableHook(reset::address);

		render::dxboiler::deinit();
	}

}
