#include <byond/sight.hh>
#include <byond/dsstat.hh>
#include <acquire.hh>

Sight* Sight::getSight()
{
	static Sight* pSightCache{ nullptr };
	
	if (pSightCache)
		return pSightCache;

	void(__thiscall DSStat::*pRefreshStats)() = &DSStat::RefreshStats;
	char* pStatData{ **reinterpret_cast<char***>(reinterpret_cast<char*&>(pRefreshStats) + 1) };
	int sightOffset{ *acquire::fromPattern<int*>({ "83 C4 0C BA FF FF 00 00 8B B0", 10 }, { ".text", "byondcore.dll" }) };

	pSightCache = reinterpret_cast<Sight*>(pStatData + sightOffset);
	return pSightCache;
}
