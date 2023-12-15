#include <gui/guiwindow.hh>
#include <gui/console.hh>
#include <main.hh>
#include <imgui.h>

#include <byond/sight.hh>

namespace gui::guiwindow
{
	namespace byondex
	{
		bool open{ true };
		static Sight* sight = Sight::getSight();
		static int sightSlider, sidSlider, siSlider;

		void draw()
		{
			if (open && ImGui::Begin("ByondEx", &open, ImGuiWindowFlags_MenuBar))
			{
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("Menu"))
					{
						ImGui::MenuItem("Console", NULL, &guiwindow::console::open);

						if (ImGui::MenuItem("Unload"))
							unload();

						ImGui::EndMenu();
					}

					ImGui::EndMenuBar();
				}

				if (ImGui::SliderInt("Sight", &sightSlider, 0, 512))
					sight->sight = sightSlider;

				if (ImGui::SliderInt("See in dark", &sidSlider, 0, 255))
					sight->see_in_dark = sidSlider;

				if (ImGui::SliderInt("See invisible", &siSlider, 0, 255))
					sight->see_invisble = siSlider;

			}
			ImGui::End();
		}

	}

	namespace console
	{
		bool open{ true };

		void draw()
		{
			if (byondex::open && open)
				gui::console::draw();

		}

	}

}
