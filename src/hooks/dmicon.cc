#include <hooks/dmicon.hh>
#include <gui/console.hh>
#include <MinHook.h>

namespace hooks::dmicon
{
	namespace extracticon
	{
		int hook(DMIcon* icon, char const* name, int a3, int a4, int a5)
		{
			gui::console::system()->Log(csys::INFO) << "Loaded icon: " << name;
			return tramp(icon, name, a3, a4, a5);
		}

		decltype(&hook) address;
		decltype(&hook) tramp;
	}

	bool setup()
	{
		extracticon::address = reinterpret_cast<decltype(&extracticon::hook)>(&extractIcon);
		if (MH_CreateHook(extracticon::address, extracticon::hook, reinterpret_cast<LPVOID*>(&extracticon::tramp)) != MH_OK)
			return false;

		return true;
	}

	void deinit()
	{
		MH_DisableHook(extracticon::address);
	}
}
