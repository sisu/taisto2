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
extern  float playerColors[][3];
void drawHud(Game& g,bool stats)
{
//	sprintf(buf, "a%.1f", g.nextSpawn);
//	drawString("asd",0,0,.1);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    char buf[100];
    sprintf(buf,"%i",(int)(ceil(100*g.player->health)));
	draw(buf,.8,-.8);
#if 0
	// deprecated?
	sprintf(buf,"%.1f", g.nextSpawn);
	draw(buf,.75,-.65);
#endif

    double nameh = -0.8;
    double s = 0.05;
    double ss = s*5;
    glLineWidth(0.000001);
    drawString("jam",-0.9,nameh,s);
    drawString("shotgun",-0.9+ss*1,nameh,s);
    drawString("uzi",-0.9+ss*2,nameh,s);
    drawString("electro",-0.9+ss*3,nameh,s);
    drawString("rocket",-0.9+ss*4,nameh,s);
    drawString("grenade",-0.9+ss*5,nameh,s);
    drawString("railgun",-0.9+ss*6,nameh,s);
	drawString("inf", -.9, -.95, s);


    
	for(int i=1; i<7; ++i) {
		sprintf(buf, "%d", g.bcnt[i]);
		drawString(buf, -.9+ss*i, -.95, s);
	}

    if(stats)
    {
        //std::cout<<"STATS:\n";
        double ss = 0.05;
        drawString("STATS:",-0.9,-0.9,ss);
        int c = 1;
        for(int i=0;i<g.units.size();i++)
        {
            if(g.units[i].type==0){
                int id = g.units[i].id;
                char buf[128];
                //sprintf(buf,"%-32s %8i %8i %8i",g.names[id].c_str(),g.kills[id],g.teamkills[id],g.deaths[id]);
                //drawString(buf,-0.9,0.9-ss*c,ss);
                //std::cout<<g.names[id]<<" "<<g.kills[id]<<" "<<g.teamkills[id]<<" "<<g.deaths[id]<<"\n";
                c++;
            }
        }
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

        glColor4f(0.2,0.2,0.2,0.7);
        glPushMatrix();
        double h = c*ss;
        glTranslatef(0,0.95-h/2,0);
        quad(0.95,h/2+0.5*ss);
        glDisable(GL_BLEND);
        glPopMatrix();
        c=1;
        glColor3f(2,2,2);
        for(int i=0;i<g.units.size();i++)
        {
            if(g.units[i].type==0){
                if(g.units[i].id == g.player->id){
                    glColor3f(2,2,2);
                }else
                {
                    glColor3f(1,1,0);
                }

                int id = g.units[i].id;
                char buf[128];
                sprintf(buf,"%-32s %8i %8i %8i",g.names[id].c_str(),g.kills[id],g.teamkills[id],g.deaths[id]);
                drawString(buf,-0.9,0.9-ss*c,ss*0.90);
                //std::cout<<g.names[id]<<" "<<g.kills[id]<<" "<<g.teamkills[id]<<" "<<g.deaths[id]<<"\n";
                c++;
            }
        }

    }

	if (g.curBase==(int)g.area.bases.size()-2) {
		char buf[128];
		sprintf(buf, "defence time %.1f", g.defenceTime);
		glColor3f(1,0,0);
		drawString(buf, -.8, .8, .1);
	}

    int w = g.weapon;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(playerColors[w+1][0],playerColors[w+1][1],playerColors[w+1][2],0.5);
    glTranslatef(-0.9+ss*w+ss/2-ss/8,nameh-ss/2,0);
    quad(ss/2,ss);
    glDisable(GL_BLEND);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);

}
