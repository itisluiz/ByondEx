#include <hooks/dungclient.hh>
#include <byond/sight.hh>
#include <gui/console.hh>
#include <MinHook.h>

namespace hooks::dungclient
{
	namespace genclickcommand
	{
		int __fastcall hook(DungClient* thisptr, void* edx, MouseParams const& mouseParams)
		{
			return tramp(thisptr, edx, mouseParams);
		}

		decltype(&hook) address;
		decltype(&hook) tramp;
	}

	bool setup()
	{
		int(__thiscall DungClient::*pGenClickCommand)(MouseParams const&) = &DungClient::GenClickCommand;
		genclickcommand::address = reinterpret_cast<decltype(&genclickcommand::hook)>(reinterpret_cast<void*&>(pGenClickCommand));
		if (MH_CreateHook(genclickcommand::address, genclickcommand::hook, reinterpret_cast<LPVOID*>(&genclickcommand::tramp)) != MH_OK)
			return false;

		return true;
	}

	void deinit()
	{
		MH_RemoveHook(genclickcommand::address);
	}
}
