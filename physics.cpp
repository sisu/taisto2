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
	for(int i=0; i<n; ++i) {
		Bullet& b = bs[i];
		b.loc += dt*b.v;
	}
}
bool moveBullet(Bullet& b, Unit* us, int n, const Area& a, double t, int* hitt)
{
	Vec2 l = b.loc + b.v*t;
	// FIXME: optimize
	Vec2 nv = normalize(b.v);
	double bdd2=1e50, bj=-1;
	for(int j=0; j<n; ++j) {
		Unit& u = us[j];
		Vec2 w = u.loc-b.loc;
		if (dot(w,nv)<0) continue;
		double d = fabs(cross(w, nv));
		if (d>.4) continue;
		double dd2 = length2(w) - d*d;
		if (dd2 < bdd2) bdd2=dd2, bj=j;
	}
	//		if (bj>=0) cout<<"asd "<<bj<<' '<<bdd2<<' '<<sqrt(bdd2)<<'\n';

	double l2 = length2(b.v*t);
	if (bdd2<l2) l2=bdd2;
	else bj=-1;
	Vec2 c = b.loc;
	int dx=b.v.x>0?1:-1, dy=b.v.y>0?1:-1;
	int ix=c.x, iy=c.y;
	int iix=dx>0?ix+1:ix, iiy=dy>0?iy+1:iy;
	double ryx = fabs(b.v.y/b.v.x), rxy = fabs(b.v.x/b.v.y);
	bool hit=0;
	while(length2(c-b.loc) < l2) {
		if (a.blocked(ix,iy)) {
			hit=1;
			break;
		}
		double xx = fabs(iix-c.x);
		double yy = fabs(iiy-c.y);
		if (fabs(b.v.x)*yy > fabs(b.v.y)*xx) {
			c.y += dy * xx * ryx;
			c.x = iix;
			ix+=dx,iix+=dx;
		} else {
			c.x += dx * yy * rxy;
			c.y = iiy;
			iy+=dy, iiy+=dy;
		}
	}
	if (hit) b.loc=c, *hitt=-1;
	else if (bj>=0) b.loc+=normalize(b.v)*sqrt(l2), *hitt=bj;
	else b.loc=l, *hitt=-1;
	return bj<0 && !hit;
}
