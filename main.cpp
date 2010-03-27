
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
#include"physics.hpp"
#include "Server.hpp"
#include "Game.hpp"
using namespace std;

float timef()
{
    int t = SDL_GetTicks();
    return t/1000.0;
}

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
	
	player.shooting = mouse[0];
}
void draw_model(Model* m)
{
#if 0
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glColor3d(1,0,0);
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
    glScalef(0.5,0.5,0.5);
    //glTranslatef(-player.loc.x,-player.loc.y,0.0);

    
    glTranslatef(-player.loc.x+0.5,-player.loc.y+0.5,0);
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
    glColor3f(0.2,0.2,0.2);
    glBegin(GL_QUADS);
		glNormal3f(0,0,1);
        glVertex3f(-1009,-1000,0);
        glVertex3f(1000,-1000,0);
        glVertex3f(1000,1000,0);
        glVertex3f(-1000,1000,0);
    glEnd();
    glPopMatrix();
}
void draw_bullet(Vec2 loc)
{
    glPushMatrix();
    glScalef(0.5,0.5,0.5);
    //glTranslatef(-player.loc.x,-player.loc.y,0.0);

    
    glTranslatef(-player.loc.x+0.5,-player.loc.y+0.5,0);
    glTranslatef(loc.x-0.5,loc.y-0.5,0);
    glScalef(0.5,0.5,0.5);
    //glTranslatef(-area.w/2,-area.h/2,0);
    glColor4f(0.2,0.8,0.2,0.1);
    glBegin(GL_QUADS);
		glNormal3f(0,0,1);
        glVertex3f(-1,-1,1);
        glVertex3f(1,-1,1);
        glVertex3f(1,1,1);
        glVertex3f(-1,1,1);
    glEnd();
    glPopMatrix();

}

void translateTo(float x,float y)
{
//    glTranslatef(x*2-1,y*2-1,0);
    glTranslatef(.5*x,.5*y,0);
//    glTranslatef(y*2-1,x*2-1,0);
}

void draw_player(float x,float y,float dir)
{
	x-=player.loc.x, y-=player.loc.y;
    glPushMatrix();
    translateTo(x,y);
    glRotatef(dir*180/M_PI-90,0,0,1);
    glRotatef(90,1,0,0);
    glRotatef(180,0,1,0);
    glScalef(0.15,0.1,0.15);
    glTranslatef(0,3.5,0);
    draw_model(&ukko_model);
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
    glLoadIdentity();
    glTranslatef(0,0,-20);
//    glRotatef(-45+ay*40,1,0,0);
    draw_area();
//    draw_player(0.5,0.5,playerdir);
//    draw_player(player.loc.x,player.loc.y,playerdir);
	for(unsigned i=0; i<game.units.size(); ++i) {
		Unit& u=game.units[i];
		draw_player(u.loc.x,u.loc.y,u.d);
	}
    if(rand()%100==0)
        std::cout<<"bullets.size() = "<<game.bullets.size()<<"\n";
	for(unsigned i=0; i<game.bullets.size(); ++i) {
        Bullet b = game.bullets[i];
        draw_bullet(b.loc);
	}
    /*
       glBegin(GL_TRIANGLES);
       glVertex3d(0,0,0);
       glVertex3d(1,0,0);
       glVertex3d(1,1,0);
       glEnd();*/
}

void mainLoop()
{
//    player.loc.x = 5;
//    player.loc.y = 3;
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
            std::cout<<player.loc.x<<" "<<player.loc.y<<"\n";
//            std::cout<<"dir = "<<player.d<<"\n";
//            std::cout<<"dir = "<<dt<<"\n";
			std::cout<<"pl "<<player.loc<<'\n';
        }
        readInput();
        handleInput();
		game.player = &player;
		game.updateNetwork();
		game.updateState(dt);
		for(unsigned i=0; i<game.units.size(); ++i)
            if (game.units[i].type==0 && game.units[i].id==game.id)
                player=game.units[i];
        draw();
#if 0
        double d = player.d;
        player.d=-M_PI/2;
        moveUnits(&player,1,area,dt);
        player.d = d;
#endif
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
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45,4.0/3.0,0.01,1000);
    glMatrixMode(GL_MODELVIEW);
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

    setPerspective();
    mainLoop();

    SDL_Quit();
}
