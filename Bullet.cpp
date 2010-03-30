#include "Bullet.hpp"
#include <cstdlib>
#include <iostream>
using namespace std;

double loadTimes[] = {.01,.3,LIGHTNING_TIME,0.5,.08,.6};
double damages[] = {0.1,5,.5,1.5,.6,25};
static double speeds[] = {10,30,0,30,100,25};
static double spreads[] = {.3,.15,0,0.5,.15,.2};

Bullet genBullet(int type, Vec2 l, double d, int id)
{
	d += ((double)rand()/RAND_MAX-.5)*spreads[type];
	Vec2 v(cos(d),sin(d));
	Bullet b(l+.7*v, v * speeds[type], type, id);
	if (type==GRENADE) b.bounce=3;
	return b;
}
