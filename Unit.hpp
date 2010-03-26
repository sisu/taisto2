#ifndef UNIT_HPP
#define UNIT_HPP

#include "Vector.hpp"

struct Unit {
	Vec2 loc;
	double d;

	int movex,movey;
	double health;

	int type,id;
};

#endif
