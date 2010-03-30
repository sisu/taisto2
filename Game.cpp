#include "Game.hpp"
#include "physics.hpp"
#include "timef.h"
#include "Bullet.hpp"
#include "Vector.hpp"

Game::Game():items(items_map.vec),bullets(bullets_map.vec), socket(*this)
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
	for(unsigned i=0; i<particles.size(); ) {
		if (particles[i].update(t,area)) ++i;
		else particles[i]=particles.back(), particles.pop_back();
	}
	for(unsigned i=0; i<lightnings.size(); ) {
		lightnings[i].first += t;
		if (lightnings[i].first < 1.5*LIGHTNING_TIME) ++i;
		else lightnings[i]=lightnings.back(), lightnings.pop_back();
	}
	nextSpawn -= t;
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
//    if (bx.type==BOUNCEGUN) lastBullets.push_back(bx);

	c -= .05*normalize(bx.v);
	if (bx.type==ROCKET) {
		// generate explosion particles
		for(int i=0; i<1024; ++i) {
//			double d=2*M_PI*rand()/RAND_MAX;
//			particles.push_back(ExplosionP(c, v*Vec2(cos(d),sin(d))));
			double s=3*EXPLOSION_SIZE*randf();
			Vec3 v = rvec3();
			particles.push_back(Particle(Vec3(c,1), s*v, EXPLOSION_P, EXPLOSION_SIZE));
		}
	} else if (bx.type==BOUNCEGUN) {
		for(int i=0; i<64; ++i) {
			double s=10*randf();
			particles.push_back(Particle(Vec3(c,1), s*rvec3(), SPARK_P, .05, .1));
		}
	} else if (bx.type==GRENADE) {
		for(int i=0; i<2048; ++i) {
			double s = 4*GRENADE_SIZE*randf();
			Vec3 v = rvec3();
			particles.push_back(Particle(Vec3(c,1), s*v, EXPLOSION_P, GRENADE_SIZE));
		}
	}

//	cout<<"asd "<<bx.origin<<' '<<g.player->loc<<' '<<bx.loc<<' '<<bx.v<<'\n';
}
