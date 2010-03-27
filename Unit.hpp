#ifndef UNIT_HPP
#define UNIT_HPP

#include "Vector.hpp"

struct Unit {
	Vec2 loc;
	double d;

	int movex,movey;
	double health;

	int type,id;

	Unit() {}
	Unit(Vec2 l, int t, int i): loc(l), d(0), movex(0), movey(0), health(1), type(t), id(i) {}
};

#endif
