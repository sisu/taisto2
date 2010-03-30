#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "Vector.hpp"

struct Particle {
	Vec3 loc, v;
	double time,dist;
	int type;
	double maxd;
	double maxt;
	double spd;

	Particle() {}
	Particle(Vec3 l, Vec3 v, int t, double maxd=1e50, double maxt=.4): loc(l), v(v), time(0), dist(0), type(t), maxd(maxd), maxt(maxt) {
		spd = length(v);
	}

	bool update(double t) {
		loc += v*t;
		time += t;
		dist += spd*t;
		return time<.4 && dist<maxd;
	}
};
#define EXPLOSION_P 0
#define SPARK_P 1

struct Game;
void drawSparks(Particle* ps, int n);
void drawExplosions(Game& g);

#endif
