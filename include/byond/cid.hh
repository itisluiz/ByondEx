#pragma once

struct Cid
{
	enum Type : unsigned short
	{
		kNone = 0,
		kMob = 1,
		kObject = 2,
		kTurf = 3
	};

	Type type;
	unsigned int id;
};
