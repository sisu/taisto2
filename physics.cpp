#include "Area.hpp"
#include<iostream>
#include "Unit.hpp"
#include "physics.hpp"
#include "Bullet.hpp"
#include "timef.h"
#include <vector>
#include <cassert>
using namespace std;

static const int dx[] = {0,0,1,1,1,0,-1,-1,-1};
static const int dy[] = {0,1,1,0,-1,-1,-1,0,1};

static void fix(Vec2& v, const Vec2& p, double d)
{
	Vec2 dv = v-p;
	if (length2(dv)<1e-3) {
		v += .5*Vec2(2*randf()-1,2*randf()-1);
		return;
	}
	if (length2(dv) < d*d) v = p+d*normalize(dv);
}
static void fix(Vec2& v, double px, double py, double d)
{
	fix(v, Vec2(px,py), d);
}
static void handleWalls(Unit& u, const Area& a)
{
	Vec2& v=u.loc;
	double x=v.x, y=v.y;
	int ix=x, iy=y;
	double d=UNIT_RAD;

	if (a.blocked(ix-1,iy)) fix(v, ix, y, d);
	if (a.blocked(ix+1,iy)) fix(v, ix+1, y, d);
	if (a.blocked(ix,iy-1)) fix(v, x, iy, d);
	if (a.blocked(ix,iy+1)) fix(v, x, iy+1, d);
	if (a.blocked(ix-1,iy-1)) fix(v, ix,iy, d);
	if (a.blocked(ix+1,iy-1)) fix(v, ix+1,iy, d);
	if (a.blocked(ix+1,iy+1)) fix(v, ix+1,iy+1, d);
	if (a.blocked(ix-1,iy+1)) fix(v, ix,iy+1, d);
}

typedef PhysicsContext PC;
void moveUnits(Unit* us, int n, const Area& a,double dt, PC& p)
{
	++p.curTick;
	if ((int)p.cellUnits.size()!=a.w*a.h) {
		cout<<"resizing "<<a.w<<' '<<a.h<<'\n';
		p.cellUnits.resize(a.w*a.h);
		p.cellTimes.resize(a.w*a.h);
	}

	for(int i=0; i<n; ++i) {
		Unit& u = us[i];
#if 0
		Vec2 d(cos(u.d), -sin(u.d));
		d *= MOVE_SPEED*dt;
		u.loc += u.movey * d;
		u.loc += u.movex * Vec2(d.y, -d.x);
#else

		double s = MOVE_SPEED * dt;
        if(u.type==0)s*=1.5;
		u.loc.y += u.movey * s;
		u.loc.x += u.movex * s;
#endif

#if 0
		for(int j=0; j<n; ++j) {
			if (j==i) continue;
			fix(us[i].loc, us[j].loc.x, us[j].loc.y, 2*UNIT_RAD);
		}
#else
		handleWalls(u, a);
		int c = a.w*int(u.loc.y)+int(u.loc.x);
		if (u.loc.x>=0 && u.loc.y>=0 && u.loc.x<a.w && u.loc.y<a.h && p.cellTimes[c]==p.curTick) {
			assert(c>=0);
			assert(c<p.cellUnits.size());
			for(unsigned i=0; i<p.cellUnits[c].size(); ++i) {
				Unit& t = *p.cellUnits[c][i];
				if (t.id != u.id) fix(u.loc, t.loc, 2*UNIT_RAD);
			}
		}
		for(int j=0; j<9; ++j) {
			int x=u.loc.x+UNIT_RAD*dx[j], y=u.loc.y+UNIT_RAD*dy[j];
			if (x<0||y<0||x>=a.w||y>=a.h) continue;
			if (j && x==(int)u.loc.x && y==(int)u.loc.y) continue;
			int z = y*a.w+x;
//			cout<<i<<' '<<cellUnits.size()<<'\n';
			assert(z >= 0);
			assert(z < p.cellUnits.size());
			if (p.cellTimes[z]!=p.curTick) {
				p.cellUnits[z].clear();
				p.cellTimes[z]=p.curTick;
//				cout<<"lol\n";
			}
//			cout<<x<<' '<<y<<' '<<z<<" ; "<<a.w<<' '<<a.h<<" ; "<<cellUnits.size()<<'\n';
			p.cellUnits[z].push_back(&u);
		}
#endif

		handleWalls(u, a);
	}
}

Vec2 wallHitPoint(Vec2 from, Vec2 to, const Area& a, bool* hitp)
{
	Vec2 c = from;
	Vec2 v = to-from;
	Vec2 nv = normalize(v);

	double l2 = length2(v);
	int dx=v.x>0?1:-1, dy=v.y>0?1:-1;
	int ix=c.x, iy=c.y;
	int iix=dx>0?ix+1:ix, iiy=dy>0?iy+1:iy;
	double ryx = fabs(v.y/v.x), rxy = fabs(v.x/v.y);
	bool hit=0;
	while(length2(c-from) < l2) {
		if (a.blocked(ix,iy)) {
			hit=1;
			break;
		}
		double xx = fabs(iix-c.x);
		double yy = fabs(iiy-c.y);
		if (fabs(v.x)*yy > fabs(v.y)*xx) {
			c.y += dy * xx * ryx;
			c.x = iix;
			ix+=dx,iix+=dx;
		} else {
			c.x += dx * yy * rxy;
			c.y = iiy;
			iy+=dy, iiy+=dy;
		}
	}
	if (hitp) *hitp = hit;
	return hit?c:to;
}

static const float bsizes[] = {.4,.1,.1,0,.3,0,.4,.1};
const double GRENADE_SLOW = .9;

std::vector<int> moveRail(Bullet& b, Unit* us, int n, const Area& a, double t, bool* hitt) {
	assert(b.type == RAILGUN);

	std::vector<int> ret;

	Vec2 nv = normalize(b.v);
	Vec2 newLoc = b.loc + b.v * t;	

	for(int j=0; j<n; ++j) {
		Unit& u = us[j];
		Vec2 w = u.loc-b.loc;
		if (dot(w,nv)<0) continue;
        if (dot(u.loc-newLoc,-nv)<0) continue;
		double d = fabs(cross(w, nv)) - bsizes[b.type];
		if (d>UNIT_RAD) continue;

		ret.push_back(j);
	}


	b.loc = wallHitPoint(b.loc, newLoc, a, hitt);
    

	return ret;
}

bool moveBullet(Bullet& b, Unit* us, int n, const Area& a, double t, int* hitt, PC& p)
{
	//assert(b.type != RAILGUN);

	if (b.type==GRENADE) {
		*hitt=-1;
		bool hit;
		double v0 = length(b.v);
		Vec2 nv = b.v/v0;
		Vec2 s = b.loc + b.v*t - .5*GRENADE_SLOW*t*t*nv;
		Vec2 c = wallHitPoint(b.loc, s, a, &hit);
		if (hit) {
			double l = length(c-b.loc);
			b.loc=c;
			if (!b.bounce) return 0;
			if (c.x==(int)c.x) b.v.x*=-1;
			else b.v.y*=-1;
			b.loc += b.v*.01;
//			t -= l/length(b.v);
			double tt = (sqrt(v0*v0 - 2*GRENADE_SLOW*l)-v0)/GRENADE_SLOW;
			b.v *= 1-GRENADE_SLOW*tt;
			--b.bounce;
			return moveBullet(b, us, n, a, t-tt, hitt, p);
		} else {
			b.loc=c;
			b.v *= 1-GRENADE_SLOW*t;
		}
		double vv = v0 - GRENADE_SLOW*t;
		return vv > 6;
	}else if(b.type==RAILGUN)
    {
        bool hit;
        b.loc = wallHitPoint(b.loc, b.loc+b.v*t, a, &hit);
        *hitt=-1;
        //std::cout<<"fuu "<<hit<<" "<<b.loc.x<<" "<<b.loc.y<<"\n";
        return true;
        //return hit;
        //return false;
    //    return true;
    }

#if 1
	Vec2 l = b.loc + b.v*t;
	Vec2 nv = normalize(b.v);
	// FIXME: optimize
	double bdd2=1e50, bj=-1;
	for(int j=0; j<n; ++j) {
		Unit& u = us[j];
		Vec2 w = u.loc-b.loc;
		if (dot(w,nv)<0) continue;
		double d = fabs(cross(w, nv));
		if (d-bsizes[b.type]>UNIT_RAD) continue;
		double dd2 = length2(w) - d*d;
		if (dd2 < bdd2) bdd2=dd2, bj=j;
	}
	//		if (bj>=0) cout<<"asd "<<bj<<' '<<bdd2<<' '<<sqrt(bdd2)<<'\n';

	double l2 = length2(b.v*t);
	if (bdd2<l2) l2=bdd2;
	else bj=-1;
	bool hit;
	Vec2 c = wallHitPoint(b.loc, b.loc+nv*sqrt(l2), a, &hit);
#if 0
	if (hit && b.bounce) {
		if (c.x==(int)c.x) b.v.x*=-1;
		else b.v.y*=-1;
		b.loc += b.v*.01;
		t -= length(c-b.loc)/length(b.v);
		--b.bounce;
		return moveBullet(b, us, n, a, t, hitt);
	} else
#endif
	{
		if (hit) b.loc=c, *hitt=-1;
		else if (bj>=0) b.loc+=normalize(b.v)*sqrt(l2), *hitt=bj;
		else b.loc=l, *hitt=-1;
        assert(!isnan(b.loc.x)&&!isnan(b.loc.y));
		return bj<0 && !hit;
	}
#else
	Vec2 to = b.loc + b.v*t;
	double l2 = length2(b.v*t);
	int dx=b.v.x>0?1:-1, dy=b.v.y>0?1:-1;
	Vec2 c = b.loc;
	int ix=c.x, iy=c.y;
	int iix=dx>0?ix+1:ix, iiy=dy>0?iy+1:iy;
	double ryx = fabs(b.v.y/b.v.x), rxy = fabs(b.v.x/b.v.y);
	bool hit=0;
	Vec2 nv = normalize(b.v);
	while(length2(c-b.loc) < l2) {
		if (a.blocked(ix,iy)) {
			hit=1;
			b.loc = c;
			break;
		}
		int z = iy*a.w + ix;
		if (p.cellTimes[z]==p.curTick) {
			double bdd2=1e100;
			Unit* bu=0;
			for(unsigned i=0; i<p.cellUnits[z].size(); ++i) {
				Unit& u = *p.cellUnits[z][i];
				if (u.id == b.shooter) continue;
				Vec2 w = u.loc-b.loc;
				if (dot(w,nv)<0) continue;
				double d = fabs(cross(w, nv));
				if (d>UNIT_RAD+bsizes[b.type]) continue;
				double dd2 = length2(w) - d*d;
				if (dd2 < bdd2) bdd2=dd2, bu=&u;
			}
			if (bu) {
				if (hitt) *hitt = bu-us;
				hit=1;
				b.loc += nv*sqrt(bdd2);
				break;
			}
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
	if (!hit) b.loc = to;
	return !hit;
#endif
}
