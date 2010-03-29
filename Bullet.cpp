#include "Bullet.hpp"
#include <cstdlib>
#include <iostream>
using namespace std;
double loadTimes[] = {.10,.3};
double damages[] = {1,10};
static double speeds[] = {800,15};

static double offsets[] = {.1,.15};

Bullet genBullet(int type, Vec2 l, double d, int id)
{
	d += ((double)rand()/RAND_MAX-.5)*offsets[type];
	Vec2 v(cos(d),sin(d));
	Bullet b(l+.3*v, v * speeds[type], type, id);
	return b;
}
