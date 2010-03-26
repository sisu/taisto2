#include "Area.hpp"
#include "Unit.hpp"
#include "physics.hpp"

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
	double d=.5;

	if (a.blocked(ix-1,iy)) fix(v, ix, y, d);
	if (a.blocked(ix+1,iy)) fix(v, ix+1, y, d);
	if (a.blocked(ix,iy-1)) fix(v, x, iy, d);
	if (a.blocked(ix,iy+1)) fix(v, x, iy+1, d);
	if (a.blocked(ix-1,iy-1)) fix(v, ix,iy, d);
	if (a.blocked(ix+1,iy-1)) fix(v, ix+1,iy, d);
	if (a.blocked(ix+1,iy+1)) fix(v, ix+1,iy+1, d);
	if (a.blocked(ix-1,iy+1)) fix(v, ix,iy+1, d);
}

void moveUnits(Unit** us, int n, const Area& a)
{
	for(int i=0; i<n; ++i) {
		Unit& u = *us[i];
		Vec2 d(cos(u.d), sin(u.d));
		d *= MOVE_SPEED;
		u.loc += u.movey * d;
		u.loc += u.movex * Vec2(d.x, d.y);

		handleWalls(u, a);
	}
}
