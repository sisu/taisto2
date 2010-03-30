#include "Game.hpp"
#include "physics.hpp"
#include "timef.h"
#include "Bullet.hpp"
#include "Vector.hpp"

Game::Game():bullets(bullets_map.vec),items(items_map.vec), socket(*this)
{
	weapon=0;
	//bulletIndex = new int[1<<16];
}
void Game::updateNetwork()
{
	socket.sendState();
	socket.handleMessages();
    for(unsigned i=0;i<units.size();i++)
    {
        if(units[i].id>=(int)unitIndex.size())
            unitIndex.resize(units[i].id+20);
        unitIndex[units[i].id]=i;
    }
}
void Game::updateState(double t)
{
	moveUnits(&units[0], units.size(), area, t);
//	moveBullets(&bullets[0], bullets.size(), &units[0], units.size(), area, t);
	for(unsigned i=0; i<bullets.size(); ) {
		int tmp;
		if (moveBullet(bullets[i], &units[0], units.size(), area, t, &tmp)) ++i;
		else destroyBullet(bullets[i].id, bullets[i].loc.x, bullets[i].loc.y);
	}
	for(unsigned i=0; i<eparts.size(); ) {
		if (eparts[i].update(t)) ++i;
		else eparts[i]=eparts.back(), eparts.pop_back();
	}
	for(unsigned i=0; i<lightnings.size(); ) {
		lightnings[i].first += t;
		if (lightnings[i].first < 1.5*LIGHTNING_TIME) ++i;
		else lightnings[i]=lightnings.back(), lightnings.pop_back();
	}
}
static float rndf()
{
	return (float)rand()/RAND_MAX;
}
void Game::destroyBullet(int id, double xx, double yy)
{
	//if (x>=bullets.size()) return;

    Vec2 c(xx,yy);
    Bullet bx = bullets_map.getById(id);
	if (bx.id!=id) return;

	//bullets[x] = bullets.back();
	//bullets.pop_back();
	//bulletIndex[bullets[x].id] = x;

    bullets_map.remove(id);

    bx.loc = c;
    bx.hitt = timef();
    lastBullets.push_back(bx);

	if (bx.type==1) {
		// generate explosion particles
		for(int i=0; i<1024; ++i) {
//			double d=2*M_PI*rand()/RAND_MAX;
//			eparts.push_back(ExplosionP(c, v*Vec2(cos(d),sin(d))));
			double s=3*EXPLOSION_SIZE*rndf();
			Vec3 v(2*rndf()-1,2*rndf()-1,2*rndf()-1);
			eparts.push_back(ExplosionP(Vec3(c,1), s*v));
		}
	}

//	cout<<"asd "<<bx.origin<<' '<<g.player->loc<<' '<<bx.loc<<' '<<bx.v<<'\n';
}
