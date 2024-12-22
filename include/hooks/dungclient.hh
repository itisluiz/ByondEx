#pragma once
#include <byond/dungclient.hh>

namespace hooks::dungclient
{
	namespace genclickcommand
	{
		int __fastcall hook(DungClient* thisptr, void* edx, MouseParams const& mouseParams);
		extern decltype(&hook) address;
		extern decltype(&hook) tramp;
	}

	bool setup();
	void deinit();
}
