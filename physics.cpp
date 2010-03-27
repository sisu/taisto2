#include "Area.hpp"
#include<iostream>
#include "Unit.hpp"
#include "physics.hpp"
#include "Bullet.hpp"

static void fix(Vec2& v, double px, double py, double d)
{
	Vec2 p(px,py);
	Vec2 dv = v-p;
	if (length2(dv) < d*d) v = p+d*normalize(dv);
}
static void handleWalls(Unit& u, const Area& a)
{
	Vec2& v=u.loc;
	double x=v.x, y=v.y;
	int ix=x, iy=y;
	double d=.4;

	if (a.blocked(ix-1,iy)) fix(v, ix, y, d);
	if (a.blocked(ix+1,iy)) fix(v, ix+1, y, d);
	if (a.blocked(ix,iy-1)) fix(v, x, iy, d);
	if (a.blocked(ix,iy+1)) fix(v, x, iy+1, d);
	if (a.blocked(ix-1,iy-1)) fix(v, ix,iy, d);
	if (a.blocked(ix+1,iy-1)) fix(v, ix+1,iy, d);
	if (a.blocked(ix+1,iy+1)) fix(v, ix+1,iy+1, d);
	if (a.blocked(ix-1,iy+1)) fix(v, ix,iy+1, d);
}

void moveUnits(Unit* us, int n, const Area& a,double dt)
{
	for(int i=0; i<n; ++i) {
		Unit& u = us[i];
#if 0
		Vec2 d(cos(u.d), -sin(u.d));
		d *= MOVE_SPEED*dt;
		u.loc += u.movey * d;
		u.loc += u.movex * Vec2(d.y, -d.x);
#else
		double s = MOVE_SPEED * dt;
		u.loc.y += u.movey * s;
		u.loc.x += u.movex * s;
#endif

		handleWalls(u, a);
	}
}

void moveBullets(Bullet* bs, int n, Unit* us, int m, const Area& a, double dt)
{
    //std::cout<<"wtf: "<<n<<" bullets\n";
	for(int i=0; i<n; ++i) {
		Bullet& b = bs[i];
		b.loc += dt*b.v;
	}
}
