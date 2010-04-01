#include "Unit.hpp"
#include "Area.hpp"
#include "physics.hpp"
#include "timef.h" // form randf()
#include <cstdlib>
#include "Server.hpp"
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>
#include <iostream>
#include <cstdio>
#include "BotInformation.hpp"

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

void moveBot(Server& server,Unit& u, const Area& area, const std::vector<Unit>& units, BotInformation* yourInfo)
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
    bool enemies=  true;

	if(myunits.size() == 0) {
        u.movex = u.movey=0;
        u.shooting = false;
        enemies=false;
    }

	std::sort(myunits.begin(),myunits.end(),cmp);

	Vec2 curLoc, myLoc;
	curLoc.x = u.loc.x;
	curLoc.y = u.loc.y;
	myLoc.x = curLoc.x;
	myLoc.y = area.bases[server.curSpawn];

	bool ok = false;

	for(unsigned i = 0; i < myunits.size(); ++i) {
		myLoc.x = myunits[i].loc.x;
		myLoc.y = myunits[i].loc.y;


		wallHitPoint(curLoc, myLoc, area, &ok);
		if(!ok && enemies){
			u.d = atan2(myLoc.y - curLoc.y, myLoc.x - curLoc.x);
			Vec2 tv = myLoc-curLoc;
			u.shooting = length(tv)<20;	
			if(length(tv)<2)return;
			tv = normalize(tv);
			u.movex = tv.x;
			u.movey = tv.y;
			return;
		}
	}
	//myLoc.x = curLoc.x;
	//myLoc.y = area.bases[server.curSpawn];

	if(yourInfo->planTime == 0 || !ok) {

		// check if we can see someone, move towards, should be possible

		// write bfs here

		int dy[] = {1,-1,0,0,1,1,-1,-1};
		int dx[] = {0,0,1,-1,1,-1,1,-1};
        //std::random_shuffle(dx,dx+8);
        //std::random_shuffle(dy,dy+8);

		const int LIMIT = 21;

		std::vector<bfsnode> Q;

		bfsnode bn = {int(u.loc.x), int(u.loc.y), -1, 0};

		Q.push_back(bn);

		int index = 0;
		bool fail = true;

		std::set<bfsnode> used;
		used.insert(bn);

		while(index < int(Q.size())) {
			bfsnode cur = Q[index];

			if(cur.x == int(myLoc.x) && cur.y == int(myLoc.y)) {
				fail = false;
				break;
			}

			if(cur.len > LIMIT) {
				break;
			}

			for(int i = 0; i < 8; ++i) {
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

		if(false/*fail*/) {
			// did not find a target, do something useful
			
			// LETS DO SOMETHING ANYWAY
		} else {
			// found a target, start following
			// list route


			std::vector<square> SQ;
			bfsnode cur = Q[rand() % std::max(1,(int(Q.size()) / 2)) + Q.size() / 2];
            int foolcount=10;
            while(abs(cur.y-area.bases[server.curSpawn])
                    >
                    abs(u.loc.y-area.bases[server.curSpawn])
                && foolcount--
                    ){
                cur = Q[rand() % std::max(1,(int(Q.size()) / 2)) + Q.size() / 2];
            }

			if(!fail) {
				cur = Q[index];
			}

			//std::cout<<"asdasd "<<u.loc<<'\n';
			while(true) {
				//std::cout<<"lol "<<cur.x<<' '<<cur.y<<'\n';
				SQ.push_back((square){cur.x, cur.y});
				if(cur.prev == -1) break;
				cur = Q[cur.prev];
			}

			yourInfo->planTime = 30*(3+rand()%4);
			yourInfo->plan.resize(0);
			for(int i = 0; i < SQ.size() - 1; ++i) {
				Vec2 vc;
				vc.x = SQ[i].x + 0.5;
				vc.y = SQ[i].y + 0.5;
				yourInfo->plan.push_back(vc);
			}

			std::reverse(SQ.begin(),SQ.end());
#if 0
			for(unsigned i = 0; i < std::min(5U,unsigned(SQ.size())); ++i) {
				std::cout<<"("<<SQ[i].x<<","<<SQ[i].y<<")"<<", ";
			}
			std::cout<<std::endl;
#endif

			if(SQ.size() < 2) return;

			double x = (int)u.loc.x + 0.5, y = (int)u.loc.y + 0.5;
			double nx = SQ[1].x + 0.5, ny = SQ[1].y + 0.5; 

			//std::cout<<x<<" "<<nx<<" "<<y<<" "<<ny<<std::endl;

			double angle = atan2(ny - y, nx - x);

			//printf("Current (%f,%f), target (%f,%f)\n",x,y,nx,ny);
			//std::cout<<"Angle: \t"<<angle<<std::endl;

			u.d = angle;
            double ay = ny-y;
            double ax = nx-x;
            if(abs(ax)<0.3)ax=0;
            if(abs(ay)<0.3)ay=0;
            double len = sqrt(ax*ax+ay*ay);
            ax/=len;
            ay/=len;
			u.movey =0.5*u.movey+0.5*ay;
			u.movex =0.5*u.movex+0.5*ax;
            /*
            u.movex = u.movey=0;
            
            if(abs(ax)>abs(ay))
            {
                u.movex = sign(ax);
            }else
            {
                u.movey = sign(ay);
            }*/
            

			//std::cout<<"MOVEY: "<<u.movey<<std::endl;
			//std::cout<<"MOVEX: "<<u.movex<<std::endl;

			if(!ok && !fail) {
				u.shooting = 0;	
				u.d = atan2(myLoc.y - curLoc.y, myLoc.x - curLoc.x);
				if (u.type==BOUNCEGUN) u.d+=.4*(2*randf()-1);
			} else if(!ok) {
				u.d = atan2(myLoc.y - curLoc.y, myLoc.x - curLoc.x);
			} else {
				u.shooting = 0;
			}	

		}
	} else {
		/*
		yourInfo->planTime = 30*(3+rand()%4);

		int r = rand() % 10 + 1;

		if(r <= 3) {
			yourInfo->stayStill = true;
		} else if() {

		}
		*/

		--yourInfo->planTime;


		int x = u.loc.x, y = u.loc.y;
jou:

		if(yourInfo->plan.size() == 0) {
			yourInfo->planTime = 0;
			return;
		}

        if(length2(u.loc-yourInfo->plan.back())>0.5){
            /*
			u.d = atan2(, yourInfo->plan.back().x - u.loc.x);
			u.movey = sign((int)(10*sin(u.d)));
			u.movex = sign((int)(10*cos(u.d)));
            */
            double ax = yourInfo->plan.back().x - u.loc.x;
            double ay = yourInfo->plan.back().y - u.loc.y;
            double len = sqrt(ax*ax+ay*ay);
            ax/=len;
            ay/=len;
            //if(abs(ax)<0.1)ax=0;
            //if(abs(ay)<0.1)ay=0;
            //u.movex = ax;
            //u.movey = ay;
			u.movey =0.5*u.movey+0.5*ay;
			u.movex =0.5*u.movex+0.5*ax;
            #if 0
            u.movex = u.movey=0;
            
            if(abs(ax)>abs(ay))
            {
                u.movex = sign(ax);
            }else
            {
                u.movey = sign(ay);
            }
#endif
		} else {
			yourInfo->plan.pop_back();
			goto jou;
		}

	}


	/*
	u.movey = -1;
	u.movex = (rand()%3)-1;
	u.shooting = 1;
	*/
}
