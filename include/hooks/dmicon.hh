#pragma once
#include <byond/dmicon.hh>

namespace hooks::dmicon
{
	namespace extracticon
	{
		int hook(DMIcon* icon, char const* name, int a3, int a4, int a5);
		extern decltype(&hook) address;
		extern decltype(&hook) tramp;
	}

	bool setup();
	void deinit();

}
