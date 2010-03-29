#include "Game.hpp"
#include "physics.hpp"
#include "timef.h"
#include "Bullet.hpp"

Game::Game(): socket(*this)
{
	weapon=0;
	bulletIndex = new int[1<<16];
}
void Game::updateNetwork()
{
	socket.sendState();
	socket.handleMessages();
    for(int i=0;i<units.size();i++)
    {
        if(units[i].id>=unitIndex.size())
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
void Game::destroyBullet(int id, double xx, double yy)
{
	unsigned x = bulletIndex[id];
	if (x>=bullets.size()) return;
    Vec2 c(xx,yy);
    Bullet bx = bullets[x];
	if (bx.id!=id) return;
	bullets[x] = bullets.back();
	bullets.pop_back();
	bulletIndex[bullets[x].id] = x;
    bx.loc = c;
    bx.hitt = timef();
    lastBullets.push_back(bx);

	if (bx.type==1) {
		// generate explosion particles
		for(int i=0; i<512; ++i) {
			double d=2*M_PI*rand()/RAND_MAX;
			double v=3*EXPLOSION_SIZE*rand()/RAND_MAX;
			eparts.push_back(ExplosionP(c, v*Vec2(cos(d),sin(d))));
		}
	}

//	cout<<"asd "<<bx.origin<<' '<<g.player->loc<<' '<<bx.loc<<' '<<bx.v<<'\n';
}
