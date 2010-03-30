#ifndef UNIT_HPP
#define UNIT_HPP

#include "Vector.hpp"

#pragma pack(push,1)
struct Unit {
	Vec2 loc;
	double d;

	double movex,movey;
	double health;

	int type,id;
	bool shooting;
	double shootTime;

	Unit() {}
	Unit(Vec2 l, int t, int i): loc(l), d(0), movex(0), movey(0), health(1), type(t), id(i), shooting(0), shootTime(0) {}
};
#pragma pack(pop)

#endif
