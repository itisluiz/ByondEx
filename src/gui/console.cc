#include <gui/console.hh>
#include <memory>

static std::unique_ptr<ImGuiConsole> pConsole;

namespace gui::console
{
	void draw()
	{
		if (!pConsole)
			pConsole = std::make_unique<ImGuiConsole>("ByondEx Console");

		pConsole->Draw();
	}

	csys::System* system()
	{
		if (!pConsole)
			return nullptr;

		return &(pConsole->System());
	}

}
