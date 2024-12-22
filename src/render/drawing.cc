#include <render/drawing.hh>
#include <gui/imguiboiler.hh>
#include <gui/guiwindow.hh>
#include <gui/hotkeys.hh>

namespace render::drawing
{
	HRESULT draw(IDirect3DDevice9* pDevice)
	{
		gui::imguiboiler::beginFrame(pDevice);

		gui::hotkeys::handleHotkeys();
		gui::guiwindow::console::draw();
		gui::guiwindow::byondex::draw();

		gui::imguiboiler::endFrame(pDevice);

		return D3D_OK;
	}

	void deinit()
	{
		gui::imguiboiler::deinit();
	}
}
