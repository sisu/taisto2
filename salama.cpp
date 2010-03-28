#include<iostream>
#include<cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include "salama.hpp"
#include "texgen.hpp"

static void rec(float* xs,float* ys,int& p,float x,float y,float tx,float ty,int d)
{
    srand(y*d+tx*p);
    if(d==0)return;
    float len = sqrt((x-tx)*(x-tx)+(y-ty)*(y-ty));
    if(len<5)return;
    float mx = (x+tx)/2.0;
    float my = (y+ty)/2.0;
    mx+=(randf()-0.5)*len*0.1;
    my+=(randf()-0.5)*len*0.1;
    rec(xs,ys,p,x,y,mx,my,d-1);
    xs[p]=mx;
    ys[p]=my;
    p++;
    rec(xs,ys,p,mx,my,tx,ty,d-1);
}


void drawSalama(float x,float y,float tx,float ty)
{
    glPushMatrix();
//    glScalef(0.5,0.5,0.5);
    float xs[128]={};
    float ys[128]={};
    int p;
    xs[0]=x;
    ys[0]=y;
    p=1;

    rec(xs,ys,p,x,y,tx,ty,5);
    xs[p]=tx;
    ys[p]=ty;
    p++;
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    glLineWidth(10);
    glDepthMask(0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnable (GL_BLEND); 
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);  
    glBindTexture(GL_TEXTURE_2D,salama.glid);
    float nx[128]={};
    float ny[128]={};
    for(int i=0;i<p-1;i++)
    {
        float xx = xs[i+1]-xs[i];
        float yy = ys[i+1]-ys[i];
        float nyy = -xx;
        float nxx = yy;
        float len = sqrt(nxx*nxx+nyy*nyy);
        nxx/=len;
        nyy/=len;
        nx[i]=nxx;
        ny[i]=nyy;
    }
    glBegin(GL_QUADS);
    for(int i=0;i<p-1;i++)
    {

        float nx1 = nx[i]+(i?nx[i-1]:0);
        float ny1 = ny[i]+(i?ny[i-1]:0);

        float nx2 = nx[i]+(i!=p-2?nx[i+1]:0);
        float ny2 = ny[i]+(i!=p-2?ny[i+1]:0);

        /*
        nx1*=0.5;
        ny1*=0.5;
        nx2*=0.5;
        ny2*=0.5;*/
        glTexCoord2f(0,0);
        glVertex3f(xs[i]+nx1,ys[i]+ny1,1);
        glTexCoord2f(1,0);
        glVertex3f(xs[i]-nx1,ys[i]-ny1,1);
        glTexCoord2f(1,1);
        glVertex3f(xs[i+1]-nx2,ys[i+1]-ny2,1);
        glTexCoord2f(0,1);
        glVertex3f(xs[i+1]+nx2,ys[i+1]+ny2,1);
    }

    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glEnable(GL_COLOR);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glDepthMask(1);

}
