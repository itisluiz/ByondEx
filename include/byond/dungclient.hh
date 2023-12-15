#pragma once
#include <byond/cid.hh>
#include <byond/mouseparams.hh>

class DungClient
{
public:
	__declspec(dllimport) Cid DungClient::GetPlayerCid();
	__declspec(dllimport) const char* DungClient::GetCidName(Cid cid);
	__declspec(dllimport) unsigned short GetCidIconList(Cid cid, unsigned long* outBuffer, unsigned short outBufferSize);
	__declspec(dllimport) int GenClickCommand(MouseParams const& mouseParams);
};
