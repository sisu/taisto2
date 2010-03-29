#include "Bullet.hpp"
#include <cstdlib>
#include <iostream>
using namespace std;

double loadTimes[] = {.05,.3,LIGHTNING_TIME,1};
double damages[] = {1,15,.5};
static double speeds[] = {1000,30,0};
static double spreads[] = {.1,.15,0,0.2};

Bullet genBullet(int type, Vec2 l, double d, int id)
{
	d += ((double)rand()/RAND_MAX-.5)*spreads[type];
	Vec2 v(cos(d),sin(d));
	Bullet b(l+.3*v, v * speeds[type], type, id);
	return b;
}
