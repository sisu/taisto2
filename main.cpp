
#include<cassert>
#include "salama.hpp"
#include <SDL/SDL.h>
#include"Unit.hpp"
#include"texgen.hpp"
#include"Area.hpp"
#include"cube.hpp"
#include<iostream>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glext.h>
#include"ukko.c"
#include"ukko_walk1.c"
#include"ukko_walk2.c"
#include"ase.c"
#include"rocket.c"
#include"physics.hpp"
#include "Server.hpp"
#include "Game.hpp"
#include"timef.h"
#include "explosion.hpp"
#include "LCD.hpp"
using namespace std;


Game game;

bool keyboard[256];
int mouseX, mouseY;
bool mouse[4];
int screenW=1024, screenH=768;

//double ay=0;
int gameEnd = false;

Area area(100,10000);//("field.in.1");
Unit player;
float playerdir;
int mouseState;
int weapon=0;
void readInput()
{
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if (e.type==SDL_QUIT) gameEnd=1;
    }

    SDL_PumpEvents();
    Uint8* keys = SDL_GetKeyState(0);
    for(int i=0; i<256; ++i) 
        keyboard[i]=keys[i];



    if(keys[SDLK_ESCAPE])gameEnd=true;
    int mstate = SDL_GetMouseState(&mouseX, &mouseY);
    for(int i=0; i<3; ++i) mouse[i] = mstate & SDL_BUTTON(1+i);
}
void handleInput()
{
    static float lasttime = 0;
    double t= timef();
    player.d=-atan2(mouseY-screenH/2.0,mouseX-screenW/2.0);
    lasttime=t;
    player.movex=0;
    player.movey=0;
    if(keyboard[SDLK_w])
        player.movey++;
    if(keyboard[SDLK_s])
        player.movey--;
    if(keyboard[SDLK_a])
        player.movex--;
    if(keyboard[SDLK_d])
        player.movex++;

	for(int i=1; i<10; ++i) if (keyboard['0'+i]) weapon=i-1;
	
	player.shooting = mouse[0];
}
void draw_interp_model(Model* m1,Model* m2,float x)
{
    assert(m1->vn==m2->vn);
    assert(m1->in==m2->in);
	static float data2[1<<16];
    for(int i=0;i<m1->vn*6;i++)
    {
        data2[i]=(1-x)*m1->data[i]+x*m2->data[i];
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    //glEnable(GL_TEXTURE_2D);
//glBindTexture(GL_TEXTURE_2D,ammo.glid);


    glVertexPointer(3,GL_FLOAT,6*sizeof(float),data2);
    glNormalPointer(GL_FLOAT,6*sizeof(float),data2+3);
    glDrawElements(GL_TRIANGLES,m1->in,GL_UNSIGNED_SHORT,(GLvoid*)m1->index);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
void draw_model(Model* m)
{
#if 1
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    //glEnable(GL_TEXTURE_2D);
//glBindTexture(GL_TEXTURE_2D,ammo.glid);


    glVertexPointer(3,GL_FLOAT,6*sizeof(float),m->data);
    glNormalPointer(GL_FLOAT,6*sizeof(float),m->data+3);
    glDrawElements(GL_TRIANGLES,m->in,GL_UNSIGNED_SHORT,(GLvoid*)m->index);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
#else
    glBegin(GL_TRIANGLES);
    for(int i=0;i<m->in;i++)
    {
        int j = m->index[i]*6;
//        glColor3f(m->data[j+3],m->data[j+4],m->data[j+5]);
        glNormal3f(m->data[j+3],m->data[j+4],m->data[j+5]);
        glVertex3f(m->data[j+0],m->data[j+1],m->data[j+2]);
        //std::cout<<m->data[i+0]<<" "<<m->data[i+1]<<" "<<m->data[i+2]<<"\n";;
    }
    glEnd();
#endif
}
void draw_area()
{
    glPushMatrix();
//    glScalef(0.5,0.5,0.5);
    //glTranslatef(-player.loc.x,-player.loc.y,0.0);

    
//    glTranslatef(-player.loc.x+0.5,-player.loc.y+0.5,0);
    glTranslatef(0.5,0.5,0);
    //glTranslatef(-area.w/2,-area.h/2,0);

    int w = 50;
    int h = 50;
    int sx = player.loc.x-w/2;
    int sy = player.loc.y-h/2;
    for(int i=sx;i<sx+w;i++)
    {
        for(int j=sy;j<sy+h;j++)
        {

            if(area.blocked(i,j)){
                glPushMatrix();
                glTranslatef(i,j,0);
                glScalef(0.51,0.51,2+2*area.height(i,j));
            
                drawcube();
                glPopMatrix();
            }
        }
    }
    glPopMatrix();
}
void draw_rocket(Bullet bu){
    Vec2 loc = bu.loc;
    
    glPushMatrix();
    
    glTranslatef(loc.x,loc.y,1);

    float a = atan2(bu.v.y,bu.v.x);
    glRotatef(a*180/M_PI+90,0,0,1);
    glRotatef(timef()*100,0,1,0);
    glRotatef(90,1,0,0);

    glColor3f(0.5,0.5,0);
    glScalef(0.05,0.05,0.05);

    draw_model(&raketti_model);
    
    glPopMatrix();
}

void draw_bullet(Bullet bu)
{
    Vec2 loc = bu.loc;
    glPushMatrix();
//    glScalef(0.5,0.5,0.5);
    //glTranslatef(-player.loc.x,-player.loc.y,0.0);
    glDepthMask(0);
    glDisable(GL_COLOR);
    glDisable(GL_LIGHTING);
    //glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnable (GL_BLEND); 
    glBlendFunc (GL_SRC_ALPHA, GL_ONE);
    glBindTexture(GL_TEXTURE_2D,ammo.glid);
    
//    glTranslatef(-player.loc.x+0.5,-player.loc.y+0.5,0);
//    glTranslatef(loc.x-0.5,loc.y-0.5,0);
    glTranslatef(loc.x,loc.y,0);

    glScalef(0.5,0.5,0.5);
    //glTranslatef(-area.w/2,-area.h/2,0);
    //glColor4f(0.2,0.8,0.2,0.1);
    float a = atan2(bu.v.y,bu.v.x);
    glColor3f(1,1,1);
    float v = length(bu.v)*4.5;
    float z = length(bu.loc-bu.origin)*2.0;
    //std::cout<<z<<"\n";
    z = std::max(0.0f,z-2);
    v = std::min(z,v);
    glRotatef(a*180/M_PI+90,0,0,1);
    glScalef(0.1,1,1);
    float part=1;
    if(bu.hitt-0.01>=0)
    {
        part = ( 0.05 ) / ((timef()-bu.hitt));
        part*=part;
        part*=2;
        if(part>1)part=1;
    }
    glBegin(GL_QUADS);
        
        glColor4f((part),part,part,(part));
        glNormal3f(1,0,0);
		glTexCoord2f(0,0);
        glVertex3f(-1,-1,1);
		glTexCoord2f(1,0);
        glVertex3f(-1,1,1);
        //float xz = part*(v-1)/v;
        glVertex3f(1,-1,1);
		glTexCoord2f(1,0.5);
        glVertex3f(1,1,1);
		glTexCoord2f(0,0.5);

		glTexCoord2f(0,0.5);
        glVertex3f(-1,1+-1,1);
		glTexCoord2f(1,0.5);
        glVertex3f(1,1+-1,1);
        
		glTexCoord2f(1,0.5);
        glVertex3f(1,1+v-1,1);
		glTexCoord2f(0,0.5);
        glVertex3f(-1,1+v-1,1);



		glTexCoord2f(0,0.5);
        glVertex3f(-1,1+v-1,1);
		glTexCoord2f(1,0.5);
        glVertex3f(1,1+v-1,1);
        glColor4f(0,0,0,0);
		glTexCoord2f(1,0.5);
        glVertex3f(1,1+v,1);
		glTexCoord2f(0,0.5);
        glVertex3f(-1,1+v,1);


    glEnd();
    glPopMatrix();
    glEnable(GL_COLOR);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glDepthMask(1);
}

void draw_player(float x,float y,float dir,float movey = 0)
{
//	x-=player.loc.x, y-=player.loc.y;
    glPushMatrix();
	glTranslatef(x,y,0);
    glRotatef(dir*180/M_PI-90,0,0,1);
    glRotatef(90,1,0,0);
    glRotatef(180,0,1,0);
    glScalef(0.15,0.1,0.15);
    glTranslatef(0,4.5,0);
    glEnable(GL_NORMALIZE);
    glColor3f(0.1,0.4,0.3);
    draw_interp_model(&ukko_walk1_model,&ukko_walk2_model,movey*sin(10*timef())*0.5+0.5);
    glTranslatef(-0.8,0.5,0.4);
    draw_model(&ase_model);
    glPopMatrix();
}

void setLights()
{
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float pos[] = {.1,.1,1,0};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHT1);
	float pos2[] = {.2,-.3,.8,0};
	glLightfv(GL_LIGHT1, GL_POSITION, pos2);
}

double spin = 0;
void draw(){
	setLights();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_NORMALIZE);
    glLoadIdentity();
    glTranslatef(0,0,-20);
	//glRotatef(-45,1,0,0);
	glTranslatef(-player.loc.x, -player.loc.y, 0);
    glColor3f(1,1,1);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,groundTex);
    glBegin(GL_QUADS);
		glNormal3f(0,0,1);
        glTexCoord2f(0,0);
        glVertex3f(-1009,-1000,0);
        glTexCoord2f(60,0);
        glVertex3f(1000,-1000,0);
        glTexCoord2f(60,60);
        glVertex3f(1000,1000,0);
        glTexCoord2f(0,60);
        glVertex3f(-1000,1000,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    draw_area();
	for(unsigned i=0; i<game.units.size(); ++i) {
		Unit& u=game.units[i];
		draw_player(u.loc.x,u.loc.y,u.d,sin(u.d)*u.movey+cos(u.d)*u.movex);
	}
	for(unsigned i=0; i<game.bullets.size(); ++i) {
        Bullet b = game.bullets[i];
		if (b.type==0) draw_bullet(b);
        else if(b.type==1) draw_rocket(b);
	}
	for(unsigned i=0; i<game.lastBullets.size(); ++i) {
        Bullet b = game.lastBullets[i];
		if (b.type!=0) continue;
        draw_bullet(b);

        Vec2 target= b.loc;
        drawSalama(game,b.origin,&target,1);
        /*
        drawSalama( b.origin.x,
                    b.origin.y,
                    b.loc.x,
                    b.loc.y);
                    */
//        drawSalama( b.origin.x, b.origin.y, b.loc.x, b.loc.y);
	}

	for(unsigned i=0; i<game.lastBullets.size(); ++i) {
        Bullet b = game.lastBullets[i];
        if(b.hitt<timef()-5)
        {
            game.lastBullets[i]=game.lastBullets.back();
            game.lastBullets.pop_back();
            i--;
        }
	}
	drawExplosions(game);

    /*
       glBegin(GL_TRIANGLES);
       glVertex3d(0,0,0);
       glVertex3d(1,0,0);
       glVertex3d(1,1,0);
       glEnd();*/

//	drawString("abcdefg", 0, 0, .2);
}

void mainLoop()
{
	player.loc = Vec2(.5,.5);
    std::cout<<player.loc.x<<" "<<player.loc.y<<"\n";

    int r  = 0;
    double lasttime = 0;
	bool res = game.socket.connect("127.0.0.1");
	cout<<"connect res "<<res<<'\n';
    while(!gameEnd) {
        double t = timef();
        double dt=t-lasttime;
        lasttime=t;
        r++;
        if(r%104==0){
            //std::cout<<player.loc.x<<" "<<player.loc.y<<"\n";
//            std::cout<<"dir = "<<player.d<<"\n";
//            std::cout<<"dir = "<<dt<<"\n";
			//std::cout<<"pl "<<player.loc<<'\n';
        }
        readInput();
        handleInput();
		game.weapon = weapon;
		game.player = &player;
		game.updateNetwork();
		game.updateState(dt);
		for(unsigned i=0; i<game.units.size(); ++i)
            if (game.units[i].type==0 && game.units[i].id==game.id)
                player=game.units[i];
        draw();
        SDL_GL_SwapBuffers();
        SDL_Delay(10);

		if (game.area.w && game.area.w!=area.w) {
			Area& a=game.area;
			area.w=a.w, area.h=a.h, area.a=a.a;
		}
    }

}
void setPerspective()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45,4.0/3.0,0.01,1000);
    glMatrixMode(GL_MODELVIEW);

    glClear(GL_ACCUM_BUFFER_BIT);
}

int main(int argc, char* argv[])
{
    srand(time(0));
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	if (argc>1) {
		Server s;
		s.loop();
		return 0;
	}
    SDL_SetVideoMode(screenW,screenH,0,SDL_OPENGL);
    initTextures();
	initLCD();

    setPerspective();
    mainLoop();

    SDL_Quit();
}
