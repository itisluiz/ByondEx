#include <state.hh>
#include <render/dxboiler.hh>
#include <gui/imguiboiler.hh>
#include <hooks/directx.hh>
#include <hooks/dungclient.hh>
#include <hooks/dmicon.hh>
#include <hooks/windows.hh>
#include <minhook.h>

namespace state
{
	HMODULE hModule{ NULL };
	HWND hRenderWindow{ NULL };
	HWND hDragWindow{ NULL };
	bool unloading{ false };

	bool setup()
	{
		unloading = false;

		if (MH_Initialize() != MH_OK)
			return false;

		if (!hooks::directx::setup())
			return false;

		if (!hooks::dungclient::setup())
			return false;

		if (!hooks::dmicon::setup())
			return false;

		if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
			return false;

		return true;
	}

	void deinit()
	{
		unloading = true;
		hooks::windows::deinit();
		hooks::dmicon::deinit();
		hooks::dungclient::deinit();
		hooks::directx::deinit();

		gui::imguiboiler::deinit();
		render::dxboiler::deinit();

		MH_RemoveHook(MH_ALL_HOOKS);
		MH_Uninitialize();

		RECT clientRect;
		GetClientRect(hRenderWindow, &clientRect);
		SendMessageW(hRenderWindow, WM_SIZE, SIZE_RESTORED, ((clientRect.right - clientRect.left) & 0xFFFF) | (((clientRect.bottom - clientRect.top) & 0xFFFF) << 16));
	}
}
