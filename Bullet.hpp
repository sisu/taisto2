#ifndef BULLET_HPP
#define BULLET_HPP

#include "Vector.hpp"

struct Bullet {
	Vec2 loc;
	Vec2 v;
	int type,id;
};

extern double loadTimes[];

#endif
