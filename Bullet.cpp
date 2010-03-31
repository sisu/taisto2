#include "Bullet.hpp"
#include <cstdlib>
#include <iostream>
using namespace std;

double loadTimes[] = {.01,.5,.08,LIGHTNING_TIME,.3,.6,1};
double damages[] = {.1,1.2,.7,0.15,6,25,9001};
static double speeds[] = {10,30,100,0,30,25,100};
static double spreads[] = {.3,.5,.15,0,.15,.2,0};

Bullet genBullet(int type, Vec2 l, double d, int id)
{
	d += ((double)rand()/RAND_MAX-.5)*spreads[type];
	Vec2 v(cos(d),sin(d));
	Bullet b(l+.7*v, v * speeds[type], type, id);
	if (type==GRENADE) b.bounce=3;
    b.hitt=0;
	return b;
}
