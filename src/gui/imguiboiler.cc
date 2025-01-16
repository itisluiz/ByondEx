#include <gui/imguiboiler.hh>
#include <hooks/windows.hh>
#include <state.hh>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx9.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace
{
	bool initialized{ false };
	bool invalidated{ false };
	bool windowsHooked{ false };
}

namespace gui::imguiboiler
{
	bool handleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
		ImGuiIO& io{ ImGui::GetIO() };

		bool keyboardCaptured{ msg >= WM_KEYFIRST && msg <= WM_KEYLAST && io.WantCaptureKeyboard };
		bool mouseCaptured{ msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST && io.WantCaptureMouse };

		return keyboardCaptured || mouseCaptured;
	}

	void beginFrame(IDirect3DDevice9* pDevice)
	{
		if (!initialized)
		{
			ImGui::CreateContext();
			ImGui_ImplWin32_Init(state::hRenderWindow);
			ImGui_ImplDX9_Init(pDevice);

			if (!windowsHooked)
			{
				hooks::windows::setup();
				windowsHooked = true;
			}

			initialized = true;
			invalidated = false;
		}

		if (invalidated)
		{
			ImGui_ImplDX9_CreateDeviceObjects();
			invalidated = false;
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void endFrame(IDirect3DDevice9* pDevice)
	{
		ImGui::Render();
		pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	void deinit(bool shutdown)
	{
		if (!ImGui::GetCurrentContext())
			return;

		if (state::unloading || shutdown)
		{	
			ImGuiIO io{ ImGui::GetIO() };

			if (io.BackendRendererUserData)
				ImGui_ImplDX9_Shutdown();

			if (io.BackendPlatformUserData)
				ImGui_ImplWin32_Shutdown();

			ImGui::DestroyContext();
			initialized = false;
		}
		else
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			invalidated = true;
		}
	}
}
