#ifndef BULLET_HPP
#define BULLET_HPP

#include "Vector.hpp"

#pragma pack(push,1)
const int MACHINEGUN = 0;
const int SHOTGUN = 1;
const int BOUNCEGUN = 2;
const int LIGHTNING = 3;
const int ROCKET = 4;
const int GRENADE = 5;
const int RAILGUN = 6;
struct Bullet {
    Vec2 origin;
	Vec2 loc;
	Vec2 v;
	int type,id;
    float hitt;
    float height;
	int bounce;
    int shooter;
	Bullet(){}
	Bullet(Vec2 l, Vec2 v, int t, int i):origin(l), loc(l), v(v), type(t), id(i) ,hitt(-1),height(0),bounce(0){}
};

extern double loadTimes[];
extern double damages[];
Bullet genBullet(int type, Vec2 l, double d, int id);

#define EXPLOSION_SIZE (5.0)
#define LIGHTNING_ANGLE (0.7)
#define LIGHTNING_RAD (17.0)
#define LIGHTNING_TIME (.05)
#define GRENADE_SIZE (8.0)

#pragma pack(pop)

#endif
