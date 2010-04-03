#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#define PHYSICS_FRAME (0.020)
#define MOVE_SPEED ( 4.5)
#define TURN_SPEED ( 1)
#include"Vector.hpp"
#include <vector>
struct Area;
struct Unit;
struct Bullet;

struct PhysicsContext {
	std::vector<std::vector<Unit*> > cellUnits;
	std::vector<int> cellTimes;
	int curTick;

	PhysicsContext(): curTick(0) {}
};

void moveUnits(Unit* us, int n, const Area& a,double dt, PhysicsContext& p);
bool moveBullet(Bullet& b, Unit* us, int n, const Area& a, double t, int* hitt, PhysicsContext& p);
std::vector<int> moveRail(Bullet& b, Unit* us, int n, const Area& a, double t, bool* hitt);
Vec2 wallHitPoint(Vec2 from, Vec2 to, const Area& a, bool* hitp=0);

const double UNIT_RAD = .4;

#endif
