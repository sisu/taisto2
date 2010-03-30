#include "Particle.hpp"
#include "Game.hpp"
#include "Bullet.hpp"
#include "timef.h"
#include <GL/gl.h>
#include <GL/glu.h>

extern GLuint explosionTex;

void drawSparks(Particle* ps, int n)
{
	if (!n) return;
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glDepthMask(0);
	glColor4f(.9,.9,.2,.6);
	glLineWidth(1);
	
	glBegin(GL_LINES);
	for(int i=0; i<n; ++i) {
		Particle& p = ps[i];
		if (p.type!=SPARK_P) continue;
		Vec3 l=p.loc, v=p.v + .05*rvec3();
		glVertex3f(l.x,l.y,l.z);
		l+=v;
		glVertex3f(l.x,l.y,l.z);
	}
	glEnd();

	glDepthMask(1);
	glLineWidth(1);
}

void drawExplosions(Game& g)
{
	if (g.particles.empty()) return;
//	cout<<"drawing "<<g.eparts.size()<<" particles\n";
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, explosionTex);
	glEnable(GL_BLEND);
	glColor4f(1,1,1,1);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glDepthMask(0);
	glDisable(GL_LIGHTING);
	const double S=.35;
	glBegin(GL_QUADS);
	for(unsigned i=0; i<g.particles.size(); ++i) {
		Particle& p=g.particles[i];
		if (p.type!=EXPLOSION_P) continue;
		Vec3 v=p.loc;
//		cout<<v<<'\n';
		glTexCoord2f(0,0), glVertex3f(v.x-S, v.y-S, v.z);
		glTexCoord2f(1,0), glVertex3f(v.x+S, v.y-S, v.z);
		glTexCoord2f(1,1), glVertex3f(v.x+S, v.y+S, v.z);
		glTexCoord2f(0,1), glVertex3f(v.x-S, v.y+S, v.z);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDepthMask(1);
	glEnable(GL_LIGHTING);
}
