#include "jalka.c"
#include "vartalo.c"
#include "Model.hpp"
#include <GL/glu.h>
#include <cmath>
#include <GL/gl.h>
#include <iostream>
using namespace std;

void drawPlayer_real(const Unit& u)
{
    glPushMatrix();
    double a = u.d*180/M_PI;
    glPushMatrix();
    glTranslatef(0,0,0.5);
    glRotatef(a+90-45,0,0,1);
    glRotatef(90,1,0,0);
    glScalef(0.4,0.4,0.4);
    draw_model(&vartalo_model,GL_TRIANGLES);
    glPopMatrix();
    if(u.movex==0 && u.movey==0)
    {
        glRotatef(a-90,0,0,1);
        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);
        glTranslatef(1,-2,0);
        draw_model(&jalka_model);
        glTranslatef(-2,0,0);
        draw_model(&jalka_model);
    }else
    {
        float d = atan2(-u.movex,u.movey)*180/M_PI;
        int dir = 1;
        d-=90;
        float dm = std::min(fmod(360+a-d,360),fmod(360+d-a,360));
        d+=90;
        if(u.type==0)
        if(dm<90)
        {
            d-=180;
            dir=-1;
        }
        glRotatef(d,0,0,1);
        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);
        float t = dir*timef()*length(Vec2(u.movex,u.movey))*4.5*4;
        float h = sin(t);
        float x = cos(t);

        glPushMatrix();
        glTranslatef(1,-2,0);
        glTranslatef(0,std::max(0.0f,x)/2,h/2);
        draw_model(&jalka_model);
        glPopMatrix();
        h = sin(t+180);
        x = cos(t+180);

        glPushMatrix();
        glTranslatef(-1,-2,0);
        glTranslatef(0,std::max(0.0f,x)/2,h/2);
        draw_model(&jalka_model);
        glPopMatrix();
    }

    glPopMatrix();
}
static void quad(double w,double h)
{
    glPushMatrix();
    glScalef(w,h,0);
    glBegin(GL_QUADS);
        glVertex3f(-1,-1,1);
        glVertex3f(-1,1,1);
        glVertex3f(1,1,1);
        glVertex3f(1,-1,1);
    glEnd();
    glPopMatrix();

}
float playerColors[][3] = {
	{.4,.8,.6},
	{0.8,.3,.0},
	{.2,.2,1},
	{.8,.3,.8},
	{.1,1,1},
	{.3,.3,.3},
	{1,1,0},
	{1,1,1}
};
void drawPlayer(const Unit& u,bool healthbar=1){

    //drawPlayer_real(u);
    //return;
    //

    //

    glPushMatrix();
    glTranslatef(u.loc.x,u.loc.y,1);

    //helabaari
    if(healthbar){
        glPushMatrix();
        glTranslatef(1.0,0.0,0);
        glColor3f(0,0,0);
        glRotatef(90,0,0,1);
        quad(1,0.1);

        glColor3f(1,0,0);
        glTranslatef(0,0,0.1);
        quad(1*u.health,0.05);
        glPopMatrix();
    }

    glColor3f(0,0,0);

    glPolygonMode(GL_BACK,GL_LINE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);
    extern int screenW;
    glLineWidth(screenW/500);
    drawPlayer_real(u);


    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

#if 0
    if(u.type==0)
        glColor3f(0.4,0.8,0.6);
    else if(u.type==1)
        glColor3f(0.8,0.3,0.0);
    else if(u.type==2){
        //std::cout<<"rakettimies\n";
        glColor3f(0.5,0.5,0.5);
    }else if(u.type==4)
    {
        glColor3f(0.5,0.5,1);
    } else if (u.type==5) glColor3f(.8,.3,.8);
#else
	if (u.type<0 || u.type>7) {
		cout<<"UNKNOWN BOT TYPE "<<u.type<<'\n';
		glColor3f(1,1,1);
	} else
	glColor3fv(playerColors[u.type]);
#endif

    drawPlayer_real(u);
    glDepthFunc(GL_LESS);
    glPopMatrix();
}


void drawDeadPlayer(Unit u,float timeDead)
{

    //std::cout<<"draw\n";
    u.movex = u.movey=0;
    glPushMatrix();
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);
    //glDisable(GL_DEPTH_TEST);
    glColor4f(playerColors[u.type][0],playerColors[u.type][1],playerColors[u.type][2],5-timeDead);

    timeDead*=4;
    if(timeDead>1)
        timeDead=1;
    glTranslatef(u.loc.x,u.loc.y,1);
    {
        glPushMatrix();

        double a = u.d*180/M_PI;
        glPushMatrix();
        glRotatef(u.lastShotAngle*180/M_PI+90,0,0,1);
        glTranslatef(0,-2*timeDead,0);
        glRotatef(timeDead*90,1,0,0);
        glRotatef(-u.lastShotAngle*180/M_PI-90,0,0,1);
        //glRotatef(-timeDead*90,1,0,0);
        glTranslatef(0,0,0.5);
        glRotatef(a+90,0,0,1);
        glRotatef(-45,0,0,1);
        glRotatef(90,1,0,0);
        glScalef(0.4,0.4,0.4);
        draw_model(&vartalo_model,GL_TRIANGLES);
        glPopMatrix();
        if(u.movex==0 && u.movey==0)
        {
            glRotatef(a-90,0,0,1);
            glRotatef(90,1,0,0);
            //glTranslatef(0,0,-1.5*timeDead);
            glRotatef(180,0,1,0);
            glScalef(0.5,0.5,0.5);
            glTranslatef(1,-2,0);
            draw_model(&jalka_model);
            glTranslatef(-2,0,0);
            draw_model(&jalka_model);
        }else
        {
            float d = atan2(-u.movex,u.movey)*180/M_PI;
            int dir = 1;
            d-=90;
            float dm = std::min(fmod(360+a-d,360),fmod(360+d-a,360));
            d+=90;
            if(u.type==0)
                if(dm<90)
                {
                    d-=180;
                    dir=-1;
                }
            glRotatef(d,0,0,1);
            glRotatef(90,1,0,0);
            //glTranslatef(0,0,-1.5*timeDead);
            glRotatef(180,0,1,0);
            glScalef(0.5,0.5,0.5);
            float t = dir*timef()*length(Vec2(u.movex,u.movey))*4.5*4;
            float h = sin(t);
            float x = cos(t);

            glPushMatrix();
            glTranslatef(1,-2,0);
            glTranslatef(0,std::max(0.0f,x)/2,h/2);
            draw_model(&jalka_model);
            glPopMatrix();
            h = sin(t+180);
            x = cos(t+180);

            glPushMatrix();
            glTranslatef(-1,-2,0);
            glTranslatef(0,std::max(0.0f,x)/2,h/2);
            draw_model(&jalka_model);
            glPopMatrix();
        }

        glPopMatrix();
    }
    glPopMatrix();
}
