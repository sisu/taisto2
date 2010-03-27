#include <SDL/SDL.h>
#include"Unit.hpp"
#include"Area.hpp"
#include"cube.hpp"
#include<iostream>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glext.h>
#include"ukko.c"
#include"physics.hpp"

float timef()
{
    int t = SDL_GetTicks();
    return t/1000.0;
}



bool keyboard[256];
int mouseX, mouseY;
bool mouse[4];
int screenW=1024, screenH=768;

int gameEnd = false;

Area area("field.in.1");
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
    player.d+=(mouseX-screenW/2)*.2*(timef()-lasttime);
    lasttime=timef();
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
    if(mouseX!=screenW/2){
        SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
        SDL_WarpMouse(screenW/2,screenH/2);
        SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
    }
}
void draw_model(Model* m)
{
#if 0 
    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3,GL_FLOAT,6,m->data);
    //glNormalPointer(GL_FLOAT,3,m->data+3);
    glDrawElements(GL_TRIANGLES,m->in,GL_UNSIGNED_SHORT,(GLvoid*)m->index);
#else
    glBegin(GL_TRIANGLES);
    for(int i=0;i<m->in;i++)
    {
        int j = m->index[i]*6;
        glColor3f(m->data[j+3],m->data[j+4],m->data[j+5]);
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

    glRotatef(player.d*180/M_PI+90,0,0,1);
    glTranslatef(-player.loc.x+0.5,-player.loc.y+0.5,0);
    //glTranslatef(-area.w/2,-area.h/2,0);

    for(int i=0;i<area.w;i++)
    {
        for(int j=0;j<area.h;j++)
        {

            if(area.blocked(i,j)){
                glPushMatrix();
                glTranslatef(i,j,0);
                glScalef(0.5,0.5,0.5);
                drawcube();
                glPopMatrix();
            }
        }
    }
    glPopMatrix();
}

void translateTo(float x,float y)
{
    glTranslatef(x*2-1,y*2-1,0);
}

void draw_player(float x,float y,float dir)
{
    glPushMatrix();
    glRotatef(90,1,0,0);
    glRotatef(180,0,1,0);
    translateTo(x,y);
    glScalef(0.2,0.2,0.2);
    draw_model(&ukko_model);
    glPopMatrix();
}

double spin = 0;
void draw(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0,0,-10);
    glRotatef(-45,1,0,0);
    draw_area();
    draw_player(0.5,0.5,playerdir);
    /*
       glBegin(GL_TRIANGLES);
       glVertex3d(0,0,0);
       glVertex3d(1,0,0);
       glVertex3d(1,1,0);
       glEnd();*/
}

void mainLoop()
{
    player.loc.x = 5;
    player.loc.y = 3;

    int r  = 0;
    double lasttime = 0;
    while(!gameEnd) {
        double t = timef();
        double dt=t-lasttime;
        lasttime=t;
        r++;
        if(r%104==0){
            std::cout<<"dir = "<<player.d<<"\n";
            std::cout<<"dir = "<<dt<<"\n";
        }
        readInput();
        handleInput();
        draw();
        moveUnits(&player,1,area,dt);
        SDL_GL_SwapBuffers();
        SDL_Delay(10);
    }

}
void setPerspective()
{
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_GREATER);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45,4.0/3.0,0.01,1000);
    glMatrixMode(GL_MODELVIEW);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    SDL_SetVideoMode(screenW,screenH,0,SDL_OPENGL);

    setPerspective();
    mainLoop();

    SDL_Quit();
}
