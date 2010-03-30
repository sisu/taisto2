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

    char buf[100];
    sprintf(buf,"%i",(int)(100*g.player->health));
	draw(buf,.8,-.8);
	sprintf(buf,"%.1f", g.nextSpawn);
	draw(buf,.75,-.65);
#if 0
    glLineWidth(3);
    glColor3f(0.5,0.2,0);
    drawString(buf,0.8,-0.8,0.1);
    glLineWidth(1);
    glColor3f(1,0.5,0);
    drawString(buf,0.8,-0.8,0.1);

#endif
}
