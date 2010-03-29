#include "Unit.hpp"
#include "Area.hpp"
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>
#include <iostream>
#include <cstdio>

double distance(const Unit& u, const Unit& another) {
	double xd = u.loc.x - another.loc.x;
	double yd = u.loc.y - another.loc.y;
	return sqrt(xd*xd + yd*yd);
}

struct Cmp {
	Unit* u;

	Cmp(Unit* unit) {
		u = unit;
	}

	bool operator () (const Unit& u1, const Unit& u2) {
		return distance(*u,u1) < distance(*u,u2);
	}
};

struct bfsnode {
	int x, y;	
	int prev;
	int len;

	bool operator < (const bfsnode& bn) const {
		if(bn.x == x) return y < bn.y;
		return x < bn.x;
	}
};

struct square {
	int x, y;
};

double anglify(double rad) {
	if(rad < 0) {
		return 2*M_PI + rad;
	}
	return rad;
}

int sign(double a) {
	if(a == 0) return 0;
	if(a < 0) return -1;
	return 1;
}

void moveBot(Unit& u, const Area& area, const std::vector<Unit>& units)
{
	if(units.size() == 0) return;

	//std::cout<<"Bot location: "<<u.loc.x<<" "<<u.loc.y<<std::endl;

	Cmp cmp(&u);

	std::vector<Unit> myunits;

	for(unsigned i = 0; i < units.size(); ++i) {
		if(units[i].type == 0) {
			myunits.push_back(units[i]);
			//std::cout<<"Player loc: "<<units[i].loc.x<<" "<<units[i].loc.y<<std::endl;
		}
	}
	
	if(myunits.size() == 0) return;

	std::sort(myunits.begin(),myunits.end(),cmp);

	// check if we can see someone, move towards, should be possible

	// write bfs here

	int dx[] = {1,0,0,-1};
	int dy[] = {0,1,-1,0};

	const int LIMIT = 25;

	std::vector<bfsnode> Q;

	bfsnode bn = {int(u.loc.x), int(u.loc.y), -1, 0};

	Q.push_back(bn);

	int index = 0;
	bool fail = true;

	std::set<bfsnode> used;
	used.insert(bn);

	while(index < int(Q.size())) {
		bfsnode cur = Q[index];

		if(cur.x == int(myunits[0].loc.x) && cur.y == int(myunits[0].loc.y)) {
			fail = false;
			break;
		}

		if(cur.len > LIMIT) {
			break;
		}

		for(int i = 0; i < 4; ++i) {
			if(!area.blocked(cur.x + dx[i], cur.y + dy[i])) {
				bfsnode nbn = (bfsnode){cur.x + dx[i], cur.y + dy[i], index, cur.len + 1};
				if(!used.count(nbn)) {
					Q.push_back(nbn);
					used.insert(nbn);
				}
			}
		}
		++index;
	}

	if(fail) {
		// did not find a target, do something useful
	} else {
		// found a target, start following
		// list route
		std::vector<square> SQ;
		bfsnode& cur = Q[index];

		//std::cout<<"asdasd "<<u.loc<<'\n';
		while(true) {
			//std::cout<<"lol "<<cur.x<<' '<<cur.y<<'\n';
			SQ.push_back((square){cur.x, cur.y});
			if(cur.prev == -1) break;
			cur = Q[cur.prev];
		}

		std::reverse(SQ.begin(),SQ.end());
#if 0
		for(unsigned i = 0; i < std::min(5U,unsigned(SQ.size())); ++i) {
			std::cout<<"("<<SQ[i].x<<","<<SQ[i].y<<")"<<", ";
		}
		std::cout<<std::endl;
#endif

		if(SQ.size() < 2) return;

		double x = int(u.loc.x) + 0.5, y = int(u.loc.y) + 0.5;
		double nx = SQ[1].x + 0.5, ny = SQ[1].y + 0.5; 

		//std::cout<<x<<" "<<nx<<" "<<y<<" "<<ny<<std::endl;

		double angle = atan2(ny - y, nx - x);

		//printf("Current (%f,%f), target (%f,%f)\n",x,y,nx,ny);
		//std::cout<<"Angle: \t"<<angle<<std::endl;

		u.d = angle;
		u.movey = sign(ny - y);
		u.movex = sign(nx - x);

		//std::cout<<"MOVEY: "<<u.movey<<std::endl;
		//std::cout<<"MOVEX: "<<u.movex<<std::endl;

		u.shooting = 1;
	}


	/*
	u.movey = -1;
	u.movex = (rand()%3)-1;
	u.shooting = 1;
	*/
}
