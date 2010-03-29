
#include<cassert>
#include "salama.hpp"
#include <SDL/SDL.h>
#include"Unit.hpp"
#include "HUD.hpp"
#include"texgen.hpp"
#include"Area.hpp"
#include"cube.hpp"
#include<iostream>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glext.h>
//#include"ukko.c"
//#include"ukko_walk1.c"
//#include"ukko_walk2.c"
//#include"ase.c"
#include"rocket.c"
#include"physics.hpp"
#include "Server.hpp"
#include "Game.hpp"
#include"timef.h"
#include "explosion.hpp"
#include "LCD.hpp"
#include "input.hpp"
#include "Menu.hpp"
#include "playerdraw.cpp"
using namespace std;

Game game;

int screenW=1024, screenH=768;
string curHost("127.0.0.1");

//double ay=0;

//Area area(100,10000);//("field.in.1");
Area& area=game.area;
Unit player;
float playerdir;
int mouseState;
int weapon=0;
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
static int sign(double x)
{
	return x<0?-1:x>0?1:0;
}
void drawWall(double x0, double x1, double y0, double y1, float z0, float z1)
{
	Vec3 c = cross(Vec3(x1-x0,y1-y0,0), Vec3(0,0,z1-z0));
//	cout<<"asd "<<x0<<' '<<x1<<' '<<y0<<' '<<y1<<' '<<z0<<' '<<z1<<" ; "<<c.x<<' '<<c.y<<'\n';
	glNormal3f(sign(c.x),sign(c.y),0);
	glTexCoord2f(0,z0), glVertex3f(x0,y0,z0);
	glTexCoord2f(1,z0), glVertex3f(x1,y1,z0);
	glTexCoord2f(1,z1), glVertex3f(x1,y1,z1);
	glTexCoord2f(0,z1), glVertex3f(x0,y0,z1);
}
int fixH(int h)
{
	if (h<=0) return 0;
	return 2+2*h;
}
void draw_area()
{
    glPushMatrix();
//    glScalef(0.5,0.5,0.5);
    //glTranslatef(-player.loc.x,-player.loc.y,0.0);

    
//    glTranslatef(-player.loc.x+0.5,-player.loc.y+0.5,0);
    //glTranslatef(-area.w/2,-area.h/2,0);

    int w = 50;
    int h = 50;
    int sx = player.loc.x-w/2;
    int sy = player.loc.y-h/2;
    glColor4f(1,1,1,1);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,buildingTex);
#if 0
    glTranslatef(0.5,0.5,0);
>>>>>>> 1be66e84bad984a84a0352060ca8a20dba25394e:main.cpp
    for(int i=sx;i<sx+w;i++)
    {
        for(int j=sy;j<sy+h;j++)
        {
            if(area.blocked(i,j)){
                glPushMatrix();
                glTranslatef(i,j,0);
                glScalef(0.51,0.51,2+2*area.height(i,j));
            
                drawcube(4+4*area.height(i,j));
                glPopMatrix();
            }
        }
    }
#else
	glBegin(GL_QUADS);
	for(int i=sx; i<sx+w; ++i) {
		for(int j=sy; j<sy+h; ++j) {
			int h=area.height(i,j);
			if (area.blocked(i,j)) {
				int hh=fixH(h);
				glNormal3f(0,0,1);
				glTexCoord2f(0,0), glVertex3f(i,j,hh);
				glTexCoord2f(1,0), glVertex3f(i+1,j,hh);
				glTexCoord2f(1,1), glVertex3f(i+1,j+1,hh);
				glTexCoord2f(0,1), glVertex3f(i,j+1,hh);
			}
			if (h!=area.height(i-1,j)) drawWall(i,i,j,j+1,fixH(h),fixH(area.height(i-1,j)));
			if (h!=area.height(i,j-1)) drawWall(i,i+1,j,j,fixH(h),fixH(area.height(i,j-1)));
		}
	}
	glEnd();
    glTranslatef(0.5,0.5,0);
#endif

    

    //ground
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,groundTex);
    //glRotatef(45,0,0,1);
    glBegin(GL_QUADS);
		glNormal3f(0,0,1);
        glTexCoord2f(0,0);
        glVertex3f(-1000,-1000,0);
        glTexCoord2f(200,0);
        glVertex3f(1000,-1000,0);
        glTexCoord2f(200,200);
        glVertex3f(1000,1000,0);
        glTexCoord2f(0,200);
        glVertex3f(-1000,1000,0);
    glEnd();
    //glRotatef(-45,0,0,1);
    glTranslatef(0,0.5,0.1);
    glDisable(GL_TEXTURE_2D);
    for(unsigned i=0;i<game.area.bases.size();i++){
        //glEnable(GL_TEXTURE_2D);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);

        if(game.curBase >= (int)i)
            glColor4f(0.0,0.7,0.1,0.5);
        else{
            glColor4f(0.8,0.0,0.1,0.5);
        }

        //glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,groundTex);
        glBegin(GL_QUADS);{
            glNormal3f(0,0,1);
            glTexCoord2f(0,0);
            glVertex3f(-1000,game.area.bases[i]-5,0);
            glTexCoord2f(200,0);
            glVertex3f(1000,game.area.bases[i]-5,0);
            glTexCoord2f(200,200);
            glVertex3f(1000,game.area.bases[i]+5,0);
            glTexCoord2f(0,200);
            glVertex3f(-1000,game.area.bases[i]+5,0);
        }
        glEnd();
        glDisable(GL_BLEND);
    }glDisable(GL_TEXTURE_2D);
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

//<<<<<<< HEAD:main.cpp
    glColor3f(0.5,0.5,0);

//=======
    glColor3f(0.3,0.9,0.3);
    glScalef(0.1,0.1,0.1);
    glRotatef(timef()*200,0,0,1);
//>>>>>>> 1c0bce074761328992930dcf72559e3f144de933:main.cpp
    draw_model(&raketti_model);
    
    double d=2*M_PI*rand()/RAND_MAX;
    double v=4;
//	game.eparts.push_back(ExplosionP(loc,-bu.v+0.2*Vec2(randf(),randf())));
    game.eparts.push_back(ExplosionP(Vec3(loc,1),Vec3(-bu.v,0)+0.2*Vec3(2*randf()-1,2*randf()-1,2*randf()-1)));
    
    glPopMatrix();
}

void draw_item(Item it)
{
    Vec2 loc = it.loc;
    glPushMatrix();
    glTranslatef(loc.x,loc.y,0);
    glScalef(0.25,0.25,0.25);
    glRotatef(it.a,0,0,1);
    glColor3f(1,1,1);
    //glBindTexture(GL_TEXTURE_2D,ammo.glid);
    drawcube();
    glPopMatrix();
}

void draw_bullet(Bullet bu,float scale=1)
{
    Vec2 loc = bu.loc;
    glPushMatrix();
//    glScalef(0.5,0.5,0.5);
    //glTranslatef(-player.loc.x,-player.loc.y,0.0);
    glDepthMask(0);
    glDisable(GL_LIGHTING);
    //glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable (GL_BLEND); 
    //glDisable (GL_BLEND); 
    glBindTexture(GL_TEXTURE_2D,ammo.glid);
    
//    glTranslatef(-player.loc.x+0.5,-player.loc.y+0.5,0);
//    glTranslatef(loc.x-0.5,loc.y-0.5,0);
    glTranslatef(loc.x,loc.y,0);

    //glScalef(0.5,0.5,0.5);
    //glTranslatef(-area.w/2,-area.h/2,0);
    //glColor4f(0.2,0.8,0.2,0.1);
    float a = atan2(bu.v.y,bu.v.x);
    float v = length(bu.v)*4.5;
    float z = length(bu.loc-bu.origin);//*2.0;
    //std::cout<<z<<"\n";
    z = std::max(0.0f,z-2);
    v = std::min(z,v);
    glRotatef(a*180/M_PI+90,0,0,1);
    glScalef(scale,scale,scale);
    float part=1;
    if(bu.hitt-0.01>=0)
    {
        part = ( 0.05 ) / ((timef()-bu.hitt));
        part*=part;
        part*=2;
        if(part>1)part=1;
    }
    glBegin(GL_QUADS);
        
        //glColor4f((part),part,part,(part));
        glNormal3f(-1,0,0);
		glTexCoord2f(0,0);
        glVertex3f(-1,-1,1);
		glTexCoord2f(0,1);
        glVertex3f(-1,1,1);
        //float xz = part*(v-1)/v;

		glTexCoord2f(1,1);
        glVertex3f(1,1,1);
		glTexCoord2f(0,1);
        glVertex3f(1,-1,1);
#if 0

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

#endif

    glEnd();
    glPopMatrix();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glDepthMask(1);
}

/*
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
}*/

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
void setPerspective()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(45,4.0/3.0,0.01,1000);
    glEnable(GL_MULTISAMPLE);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_LINE_SMOOTH);

    glMatrixMode(GL_MODELVIEW);
}

double spin = 0;
void draw(){
    setPerspective();
	setLights();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_NORMALIZE);
    glLoadIdentity();
    drawHud(game);
    glTranslatef(0,0,-30);
    //glTranslatef(0,0,-35);
	//glRotatef(-45,1,0,0);
	glTranslatef(-player.loc.x, -player.loc.y, 0);
    glColor3f(1,1,1);
    

    draw_area();
	for(unsigned i=0; i<game.units.size(); ++i) {
		Unit& u=game.units[i];
        drawPlayer(u);
		//draw_player(u.loc.x,u.loc.y,u.d,sin(u.d)*u.movey+cos(u.d)*u.movex);
	}
	for(unsigned i=0; i<game.items.size(); ++i) {
		Item& u=game.items[i];
        draw_item(u);
		//draw_player(u.loc.x,u.loc.y,u.d,sin(u.d)*u.movey+cos(u.d)*u.movex);
	}
	for(unsigned i=0; i<game.bullets.size(); ++i) {
        Bullet b = game.bullets[i];
		if (b.type==0){
            glColor4f(1.5,0.6,0.0,0.5);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE);
            draw_bullet(b);
        }
        else if(b.type==1) draw_rocket(b);
        else if(b.type==SHOTGUN){
            glColor4f(0.5,0.7,1,0.5);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE);
            draw_bullet(b,.5);
        }
	}

    for(int l=0;l<game.lightnings.size();l++)
    {
        std::vector<Vec2> enemies;
        for(int i=1;i<game.lightnings[l].second.size();i++)
        {
            int j = game.lightnings[l].second[i];

            enemies.push_back(game.units[game.unitIndex[j]].loc);
        }
        drawSalama(game,game.units[game.unitIndex[game.lightnings[l].second[0]]].loc,&enemies[0],
                enemies.size());
    }

#if 0 
	for(unsigned i=0; i<game.lastBullets.size(); ++i) {
        Bullet b = game.lastBullets[i];
		if (b.type!=0) continue;
        draw_bullet(b);

        Vec2 target= b.loc;
        //std::vector<Vecc2
        /*
        drawSalama( b.origin.x,
                    b.origin.y,
                    b.loc.x,
                    b.loc.y);
                    */
//        drawSalama( b.origin.x, b.origin.y, b.loc.x, b.loc.y);
	}
#endif

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
//	bool res = game.socket.connect("127.0.0.1");
	bool res = game.socket.connect(curHost.c_str());
	cout<<"connect res "<<res<<'\n';
	if (!res) {
		return;
	}
    glClear(GL_ACCUM_BUFFER_BIT);
    while(1) {
        double t = timef();
        double dt=t-lasttime;
        lasttime=t;
        r++;
        if(r%104==0){
			cout<<player.loc<<'\n';
//            std::cout<<"dir = "<<player.d<<"\n";
//            std::cout<<"dir = "<<dt<<"\n";
			//std::cout<<"pl "<<player.loc<<'\n';
        }
        readInput();
		if (keyboard[27]) break;
        handleInput();
		game.weapon = weapon;
		game.player = &player;
		game.updateNetwork();
		game.updateState(dt);
		for(unsigned i=0; i<game.units.size(); ++i)
            if (game.units[i].type==0 && game.units[i].id==game.id)
                player=game.units[i];
        draw();
		int err = glGetError();
		if (err) {
			cout<<"GL ERROR: "<<gluErrorString(err)<<'\n';
		}
        SDL_GL_SwapBuffers();
        SDL_Delay(15);

#if 0
		if (game.area.w && game.area.w!=area.w) {
			Area& a=game.area;
			area.w=a.w, area.h=a.h, area.a=a.a;
		}
#endif
    }
	cout<<"returning from main loop\n";
}

Server* server;
int startServer(void*)
{
	server = new Server;
	server->loop();
	delete server;
	server=0;
	return 0;
}
void hostGame()
{
	SDL_CreateThread(startServer, 0);
	SDL_Delay(50);
	mainLoop();
	cout<<"setting server end\n";
	server->end=1;
	SDL_Delay(10);
}
void joinGame()
{
	Menu m;
	MenuItem host={"hostname",STRING};
	host.str = &curHost;
	m.items.push_back(host);
	MenuItem conn={"connect",PICK};
	conn.func = mainLoop;
	m.items.push_back(conn);
	m.exec();
}
Menu createMainMenu()
{
	Menu m;
	MenuItem host={"host game",PICK};
	host.func = hostGame;
	m.items.push_back(host);
	MenuItem join={"join game",PICK};
	join.func = joinGame;
	m.items.push_back(join);
	m.items.push_back((MenuItem){"quit", EXIT});
	return m;
}

int main(int argc, char* argv[])
{
    srand(time(0));
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	atexit(SDL_Quit);
	if (argc>1) {
		Server s;
		s.loop();
		return 0;
	}
    SDL_SetVideoMode(screenW,screenH,0,SDL_OPENGL);
    initTextures();
	initLCD();

#if 0
    mainLoop();
#else
	Menu m = createMainMenu();
	m.exec();
#endif
}
