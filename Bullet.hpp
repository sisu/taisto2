#ifndef BULLET_HPP
#define BULLET_HPP

#include "Vector.hpp"

struct Bullet {
    Vec2 origin;
	Vec2 loc;
	Vec2 v;
	int type,id;
    float hitt;

	Bullet(){}
	Bullet(Vec2 l, Vec2 v, int t, int i):origin(l), loc(l), v(v), type(t), id(i) ,hitt(-1){}
};

extern double loadTimes[];
extern double damages[];
Bullet genBullet(int type, Vec2 l, double d, int id);

#endif
