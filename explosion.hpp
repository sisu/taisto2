#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "Vector.hpp"
#include "Bullet.hpp"
#include <GL/gl.h>

struct ExplosionP {
	Vec3 loc, v;
	double time,dist;
	double spd;

	ExplosionP(){}
	ExplosionP(Vec3 l, Vec3 v): loc(l), v(v), time(0), dist(0), spd(length(v)) {}

	bool update(double t) {
		loc += v*t;
		time += t;
		dist += spd*t;
		return time<.4 && dist<EXPLOSION_SIZE;
	}
};
struct Game;
void drawExplosions(Game& g);

#endif
