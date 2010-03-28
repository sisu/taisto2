#include<iostream>
#include<cstdlib>
#include "timef.h"
#include "Game.hpp"
#include<cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "Vector.hpp"

#include "salama.hpp"
#include "texgen.hpp"

static void rec(float* xs,float* ys,int& p,float x,float y,float tx,float ty,int d)
{
    //srand(y*d+tx*p);
    if(d==0)return;
    float len = sqrt((x-tx)*(x-tx)+(y-ty)*(y-ty));
    if(len<2)return;
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
static float randff(int z)
{
    unsigned  x = z*321253+z*z*32423+z*z*z*5123;
    x = x*222253+x*z*52423+z*x*z*8123;
    x = (x<<8) + (x>>8) + z*z*28983;
    return x/(double((unsigned)-1));
}
static float fuaa(float f)
{
    int r = f;
    float z = f-r;
    return randff(r+1)*z+randff(r)*(1-z);
}

static float curtime;
static Vec2 randpoint(int x,int y)
{
    return Vec2(x+fuaa(curtime+x*5),y+fuaa(curtime+123+y*3));
}

void drawSalamaStrip(Vec2* v,int p)
{
    glPushMatrix();

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
        float xx = v[i+1].x-v[i].x;
        float yy = v[i+1].y-v[i].y;
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
        glVertex3f(v[i].x+nx1,v[i].y+ny1,1);
        glTexCoord2f(1,0);
        glVertex3f(v[i].x-nx1,v[i].y-ny1,1);
        glTexCoord2f(1,1);
        glVertex3f(v[i+1].x-nx2,v[i+1].y-ny2,1);
        glTexCoord2f(0,1);
        glVertex3f(v[i+1].x+nx2,v[i+1].y+ny2,1);
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

#include<queue>

struct pqnode{
    float len;
    int x;
    int y;
    int px,py;
    bool operator<(const pqnode& p)const
    {
        return len>p.len;
    }
};

short prevx[300][3000];
short prevy[300][3000];
void drawSalama(Game& game,Vec2 orig,Vec2* targets,int n)
{
    curtime = timef();
    memset(prevx,-1,sizeof(prevx));
    memset(prevy,-1,sizeof(prevy));
    std::priority_queue<pqnode> pq;
    pqnode start;
    start.x = orig.x;
    start.y = orig.y;
    start.px = -100;
    start.py = -100;
    start.len = 0;
    pq.push(start);
    bool salamaed[100]={};
    bool path_found = 0;
    while(pq.size())
    {
        pqnode p = pq.top();
        pq.pop();
        if(prevx[p.x][p.y]!=-1)continue;
        prevx[p.x][p.y]=p.px;
        prevy[p.x][p.y]=p.py;
        for(int i=0;i<n;i++)
        {
            if(salamaed[i])continue;
            int tx = targets[i].x;
            int ty = targets[i].y;
            if(tx==p.x && ty==p.y)
            {
                std::vector<Vec2> strip;
                strip.push_back(targets[i]);
                int x,y;
                x = p.x;
                y = p.y;
                while(x!=-100)
                {
                    strip.push_back(randpoint(x,y));
                    int x2 = prevx[x][y];
                    y = prevy[x][y];
                    x = x2;
                }
                strip.push_back(orig);
                drawSalamaStrip(&strip[0],strip.size());
                return;
            }
        }
        
        static const int dx[]= {0,1,1,1,0,-1,-1,-1};
        static const int dy[]= {1,1,0,-1,-1,-1,0,1};
        for(int i=0;i<8;i++)
        {
            int nx = p.x+dx[i];
            int ny = p.y+dy[i];
            if(game.area.blocked(nx,ny))continue;
            if(nx<0||nx>=300)continue;
            if(ny<0||ny>=3000)continue;
            pqnode ne ;
            ne.x=nx;
            ne.y=ny;
            ne.px = p.x;
            ne.py = p.y;
            ne.len = p.len+length2((randpoint(p.x,p.y)-randpoint(nx,ny)));
            pq.push(ne);
        }
    }
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

        float xx = xs[i+1]-xs[i];
        float yy = ys[i+1]-ys[i];
        float len = sqrt(xx*xx+yy*yy);

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
        glTexCoord2f(1,len);
        glVertex3f(xs[i+1]-nx2,ys[i+1]-ny2,1);
        glTexCoord2f(0,len);
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
