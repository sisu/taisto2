#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "Vector.hpp"
#include "Area.hpp"
#include "physics.hpp"

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

	bool update(double t, const Area& a) {
//		loc += v*t;
		loc = Vec3(wallHitPoint(loc, loc+v*t, a), loc.z+v.z*t);
		time += t;
		dist += spd*t;
		return time<maxt && dist<maxd;
	}
};
#define EXPLOSION_P 0
#define SPARK_P 1
#define SMOKE_P 2

void drawParticles(Particle* ps, int n);

#endif
