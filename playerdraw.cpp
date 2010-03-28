#include "jalka.c"
#include "vartalo.c"
#include "Model.hpp"
#include <GL/glu.h>
#include <cmath>
#include <GL/gl.h>

void drawPlayer(const Unit& u)
{
    glPushMatrix();
    glColor3f(0.7,1,0.8);
    glTranslatef(u.loc.x,u.loc.y,1);
    double a = u.d*180/M_PI;
    glPushMatrix();
    glRotatef(a+90+45,0,0,1);
    glRotatef(90,1,0,0);
    glScalef(0.5,0.5,0.5);
    draw_model(&vartalo_model);
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
        std::cout<<dm<<"\n";
        if(dm<90)
        {
            d-=180;
            dir=-1;
        }
        glRotatef(d,0,0,1);
        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);
        glScalef(0.5,0.5,0.5);
        float t = dir*timef()*length(Vec2(u.movex,u.movey))*4.5*2;
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
