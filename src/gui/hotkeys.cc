#include <gui/hotkeys.hh>
#include <gui/guiwindow.hh>
#include <imgui.h>
#include <Windows.h>

namespace gui::hotkeys
{
	void handleHotkeys()
	{
		ImGuiIO& io{ ImGui::GetIO() };

		if (ImGui::IsKeyPressed(ImGuiKey_Insert, false))
			guiwindow::byondex::open = !guiwindow::byondex::open;

	}

}
