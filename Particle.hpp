#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "Vector.hpp"

struct Particle {
	Vec3 loc, v;
	double time,dist;
	double spd;
	int type;
	double maxd;

	Particle() {}
	Particle(Vec3 l, Vec3 v, int t, double maxd=1e50): loc(l), v(v), time(0), dist(0), spd(length(v)), type(t), maxd(maxd) {}

	bool update(double t) {
		loc += v*t;
		time += t;
		dist += spd*t;
		return time<.4 && dist<maxd;
//		return time<.4 && dist<EXPLOSION_SIZE;
	}
};
#define EXPLOSION_P 0
#define SPARK_P 1

struct Game;
void drawSparks(Game& g);
void drawExplosions(Game& g);

#endif
