#include"HUD.hpp"
#include<GL/gl.h>
#include<GL/glu.h>
#include "LCD.hpp"


void drawHud(Game& g)
{
    char buf[100];
    sprintf(buf,"%i",(int)(100*g.player->health));
    glLineWidth(3);
    glColor3f(0.5,0.2,0);
    drawString(buf,0.8,-0.8,0.1);
    glLineWidth(1);
    glDisable(GL_DEPTH_TEST);
    glColor3f(1,0.5,0);
    drawString(buf,0.8,-0.8,0.1);
    glEnable(GL_DEPTH_TEST);
}
