#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "Vector.hpp"
#include <GL/gl.h>

struct ExplosionP {
	Vec2 loc, v;
	double time,dist;

	ExplosionP(){}
	ExplosionP(Vec2 l, Vec2 v): loc(l), v(v), time(0), dist(0) {}

	bool update(double t) {
		loc += v*t;
		time += t;
		dist += t;
		return time<.2 && dist<.5;
	}
};
struct Game;
void drawExplosions(Game& g);

#endif
