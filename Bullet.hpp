#ifndef BULLET_HPP
#define BULLET_HPP

#include "Vector.hpp"

struct Bullet {
	Vec2 loc;
	Vec2 v;
	int type,id;

	Bullet(){}
	Bullet(Vec2 l, Vec2 v, int t, int i): loc(l), v(v), type(t), id(i) {}
};

extern double loadTimes[];

#endif
