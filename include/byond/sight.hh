#pragma once

struct Sight
{
	unsigned short sight;

private:
	char pad0[3];

public:
	unsigned char see_in_dark;
	unsigned char see_invisble;

public:
	static Sight* getSight();

};
