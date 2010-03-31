#include"HUD.hpp"
#include<GL/gl.h>
#include<GL/glu.h>
#include "LCD.hpp"

void draw(const char* s, double x, double y)
{
	glLineWidth(3);
	glColor3f(.5,2,0);
	drawString(s,x,y,.1);
	glLineWidth(1);
	glColor3f(1,.5,0);
	drawString(s,x,y,.1);
}

void drawHud(Game& g)
{
//	sprintf(buf, "a%.1f", g.nextSpawn);
//	drawString("asd",0,0,.1);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    char buf[100];
    sprintf(buf,"%i",(int)(100*g.player->health));
	draw(buf,.8,-.8);
#if 0
	// deprecated?
	sprintf(buf,"%.1f", g.nextSpawn);
	draw(buf,.75,-.65);
#endif

	for(int i=1; i<6; ++i) {
		sprintf(buf, "%d", g.bcnt[i]);
		drawString(buf, -.9+.3*i, -.95, .05);
	}
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
