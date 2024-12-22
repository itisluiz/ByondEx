#pragma once
#include <imgui.h>
#include <stdio.h>
#include <imguiconsole/imgui_console.h>

namespace gui::guiwindow
{
	namespace byondex
	{
		extern bool open;
		void draw();
	}

	namespace console
	{
		extern bool open;
		void draw();
	}
}
