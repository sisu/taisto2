#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include "texgen.hpp"
#include "explosion.hpp"
#include "Game.hpp"
#include "texgen.hpp"
using namespace std;

void drawExplosions(Game& g)
{
	if (g.eparts.empty()) return;
//	cout<<"drawing "<<g.eparts.size()<<" particles\n";
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glDepthMask(0);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, explosionTex);
	glDisable(GL_DEPTH_TEST);
	const double S=.1;
	glBegin(GL_QUADS);
	for(unsigned i=0; i<g.eparts.size(); ++i) {
		ExplosionP& p=g.eparts[i];
		Vec2 v=p.loc;
//		cout<<v<<'\n';
		glTexCoord2f(0,0), glVertex2f(v.x-S, v.y-S);
		glTexCoord2f(1,0), glVertex2f(v.x+S, v.y-S);
		glTexCoord2f(1,1), glVertex2f(v.x+S, v.y+S);
		glTexCoord2f(0,1), glVertex2f(v.x-S, v.y+S);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDepthMask(1);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}
