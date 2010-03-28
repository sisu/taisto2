#include "Bullet.hpp"
#include <cstdlib>
#include <iostream>
using namespace std;
double loadTimes[] = {.05,.3};
double damages[] = {1,0};
static double speeds[] = {1000,10};
static double offsets[] = {.1,.15};

Bullet genBullet(int type, Vec2 l, double d, int id)
{
	d += ((double)rand()/RAND_MAX-.5)*offsets[type];
	Vec2 v(cos(d),sin(d));
	Bullet b(l+.1*v, v * speeds[type], type, id);
	return b;
}
