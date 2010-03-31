#include "Particle.hpp"
#include "Game.hpp"
#include "Bullet.hpp"
#include "timef.h"
#include "texgen.hpp"
#include <GL/gl.h>
#include <GL/glu.h>

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

void drawSingleType(Particle* ps, int n, int type, GLuint tex, float S)
{
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, tex);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glDepthMask(0);

	glBegin(GL_QUADS);
	for(int i=0; i<n; ++i) {
		Particle& p=ps[i];
		if (p.type!=type) continue;
		Vec3 v=p.loc;
//		cout<<v<<'\n';
		glTexCoord2f(0,0), glVertex3f(v.x-S, v.y-S, v.z);
		glTexCoord2f(1,0), glVertex3f(v.x+S, v.y-S, v.z);
		glTexCoord2f(1,1), glVertex3f(v.x+S, v.y+S, v.z);
		glTexCoord2f(0,1), glVertex3f(v.x-S, v.y+S, v.z);
	}
	glEnd();
	glDepthMask(1);
}
void drawExplosions(Game& g)
{
	glColor3f(1,1,1);
	drawSingleType(&g.particles[0], g.particles.size(), EXPLOSION_P, explosionTex, .45);
}

void drawSmoke(Game& g)
{
	glColor3f(1,1,1);
	drawSingleType(&g.particles[0], g.particles.size(), SMOKE_P, smokeTex, .65);
}
