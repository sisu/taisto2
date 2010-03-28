#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#define PHYSICS_FRAME (0.020)
#define MOVE_SPEED ( 4.5)
#define TURN_SPEED ( 1)
struct Area;
struct Unit;
struct Bullet;
void moveUnits(Unit* us, int n, const Area& a,double dt=PHYSICS_FRAME);
void moveBullets(Bullet* bs, int n, Unit* us, int m, const Area& a, double dt);
bool moveBullet(Bullet& b, Unit* us, int n, const Area& a, double t, int* hitt);

#endif
