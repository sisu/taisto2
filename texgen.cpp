#include<iostream>
#include<cstdlib>
#include<cmath>
#include<GL/gl.h>
#include<GL/glu.h>
#include <algorithm>
#include"texgen.hpp"
using namespace std;

const int NSIZE = 512;
double noise[NSIZE][NSIZE];

double rndf()
{
	return rand()/double(RAND_MAX);
}
void genNoise()
{
	for(int i=0; i<NSIZE; ++i)
		for(int j=0; j<NSIZE; ++j)
			noise[i][j] = rndf();
}
double interp(double x,double y,double a)
{
    double d = sin(a*M_PI/2);
    return d*x+(1-d)*y;
}

double smooth(double x, double y)
{
	int ix = int(x), iy = int(y);
	double fx = x-ix;
	double fy = y-iy;

	int ix2 = std::min(ix+1, NSIZE-1);
	int iy2 = std::min(iy+1, NSIZE-1);

	double n1 = noise[iy][ix];
	double n2 = noise[iy][ix2];
	double n3 = noise[iy2][ix];
	double n4 = noise[iy2][ix2];
    double xr1 =  interp(n4,n3,fx);
    double xr2 =  interp(n2,n1,fx);
    return interp(xr1,xr2,fy);

	return fx*fy*n4 + (1.0-fx)*fy*n3 + fx*(1.0-fy)*n2 + (1.0-fx)*(1.0-fy)*n1;
}
double turbulence(double x, double y, double size)
{
	double value = 0.0, asize = size;
	while(size >= 1.0) {
		value += smooth(x/size, y/size) * size;
		size /= 2;
	}
	return value / asize;
}

static Texture getAmmo()
{
    Texture ret;
    ret.w = 32;
    ret.h = 32;
    ret.pix = new unsigned[ret.w*ret.h];
    for(int x=0;x<ret.w;x++)
    {
        for(int y=0;y<ret.h;y++)
        {
            float z = (ret.w/2+0.5-x);
            z = z*z+(ret.h/2-y)*(ret.h/2-y);
            if(z!=0)
                z = 420000.0/(z*z);

            int iz = std::min(int(z),255);
            if(iz<5)iz=0;
            ret.pix[y*ret.w+x]=iz|iz<<24|(iz)<<16|iz<<8;
            //ret.pix[y*ret.w+x]=-1;//iz|iz<<24|(iz)<<16|iz<<8;
        }
    }
    unsigned tex;
    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_2D,tex);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the original
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    std::cout<<"mipmaps = "<<gluBuild2DMipmaps( GL_TEXTURE_2D, 4,ret.w,ret.h,
            GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, ret.pix )<<'\n';
    ret.glid = tex;
    glBindTexture(GL_TEXTURE_2D,0);
    return ret;
}
static Texture getSalama()
{
    Texture ret;
    ret.w = 64;
    ret.h = 64;
    ret.pix = new unsigned[ret.w*ret.h];
    for(int x=0;x<ret.w;x++)
    {
        for(int y=0;y<ret.h;y++)
        {
            float z = (ret.w/2+0.5-x);
            z = abs(z);
            if(z!=0)
                z = 200.0/(z);
            else z = 1000;
            z+=z*(0.1*(rand()%10));
            //int iz = std::min(int(z),700);
            int iz = z;
            iz=(iz+70)/2;
            if(iz<10)iz=0;
            ret.pix[y*ret.w+x]=(std::max(0,std::min(255,iz*2-80)))|std::min(255,iz/2)<<24|std::min(255,iz/2)<<16|std::min(255,int(sqrt(10000*iz)))<<8;
        }
    }
    unsigned tex;
    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_2D,tex);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the original
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    std::cout<<"mipmaps = "<<gluBuild2DMipmaps( GL_TEXTURE_2D, 4,ret.w,ret.h,
            GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, ret.pix )<<'\n';
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    ret.glid = tex;
    glBindTexture(GL_TEXTURE_2D,0);
    return ret;
}

GLuint genExplosionTex()
{
	const int TS=256;
	float texture[TS][TS][4];
	genNoise();

	for(int i=0; i<TS; ++i) {
		for(int j=0; j<TS; ++j) {
//			double x=2.*(j-TS/2)/TS;
//			double y=2.*(i-TS/2)/TS;
			double x=double(j)/TS, y=double(i)/TS;
			texture[i][j][0] = .6+.2*rndf();
			texture[i][j][1] = .3+.5*rndf();
			texture[i][j][2] = .2*rndf();
//			texture[i][j][3] = max(0.0, turbulence(x,y,64) + .5);
			double d = (x-.5)*(x-.5) + (y-.5)*(y-.5);
			double f = min(1., .05/d);
			const double B=.2;
			f = min(f, max(0.0, .5/(.5-B) - sqrt(d)/B));
//			f = min(f, min(min(x,1-x),min(y,1-y))/B);
			const double F=128;
			texture[i][j][3] = .4*f*(turbulence(F*x,F*y,16)-.1);
		}
	}
	GLuint t;
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the original
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    std::cout<<"mipmaps = "<<gluBuild2DMipmaps( GL_TEXTURE_2D, 4,TS,TS,
            GL_RGBA, GL_FLOAT, texture )<<'\n';
    glBindTexture(GL_TEXTURE_2D,0);
    return t;
}
GLuint genGroundTex()
{
	const int TS=256;
    static float texture[TS][TS][4];
	genNoise();

	for(int i=0; i<TS; ++i) {
		for(int j=0; j<TS; ++j) {
			double x=double(j)/TS, y=double(i)/TS;
            //if(x>0.5)x = 1-x;
            //if(y>0.5)y = 1-y;
            double s = turbulence(128*x,128*y,32);
            //s=0;
            int is =0;
            float z = (i/32+j/32)%2;
            int xx= i%32;
            int yy= j%32;
            const int bev = 10;
            int f = (i/32)*37+(j/32)*12;
            int foo = std::max(abs(16-xx),abs(16-yy));
            if(f%10==0)z=0;
#define FF  if(xx+yy<bev)z=0

            FF;
            xx = 31-xx;
            FF;
            yy = 31-yy;
            FF;
            xx = 31-xx;
            FF;
            if((int(s*20)%2))is=1;

            texture[i][j][0] = z*0.5+is*0.01;
            texture[i][j][1] = z*0.2+is*0.1;
            texture[i][j][2] = z*0.3+is*0.1;
			texture[i][j][3] =1;
		}
	}
	GLuint t;
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the original
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    std::cout<<"mipmaps = "<<gluBuild2DMipmaps( GL_TEXTURE_2D, 4,TS,TS,
            GL_RGBA, GL_FLOAT, texture )<<'\n';
    glBindTexture(GL_TEXTURE_2D,0);
    return t;
}
GLuint genBuildingTex()
{
	const int TS=64;
    static float texture[TS][TS][4];
	genNoise();

	for(int i=0; i<TS; ++i) {
		for(int j=0; j<TS; ++j) {
			double x=double(j)/TS, y=double(i)/TS;
            //if(x>0.5)x = 1-x;
            //if(y>0.5)y = 1-y;
            //double s = abs(turbulence(28*x,28*y,16));
            int s = i==0||j==0||i==TS-1||j==TS-1;// (i/32+j/32)%2;
            s=!s;
            texture[i][j][0] = s*0.0+(!s)*0.1;
            texture[i][j][1] = s*0.0+(!s)*0.4;
            texture[i][j][2] = s*0.0+(!s)*0.9;
			texture[i][j][3] =1;
		}
	}
	GLuint t;
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the original
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    std::cout<<"mipmaps = "<<gluBuild2DMipmaps( GL_TEXTURE_2D, 4,TS,TS,
            GL_RGBA, GL_FLOAT, texture )<<'\n';
    glBindTexture(GL_TEXTURE_2D,0);
    return t;
}
GLuint genHealthTex()
{
	const int TS=64;
    static float texture[TS][TS][4];
	genNoise();

	for(int i=0; i<TS; ++i) {
		for(int j=0; j<TS; ++j) {
			double x=double(j)/TS, y=double(i)/TS;
            texture[i][j][0] = 1;
            texture[i][j][1] = 0;
            texture[i][j][2] = 0;
            if((x<0.333 || x>=0.666) and ( y<1/3.0 or y>2/3.0)){
                texture[i][j][1] = 1;
                texture[i][j][2] = 1;
            }
			texture[i][j][3] =1;
		}
	}
	GLuint t;
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the original
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    std::cout<<"mipmaps = "<<gluBuild2DMipmaps( GL_TEXTURE_2D, 4,TS,TS,
            GL_RGBA, GL_FLOAT, texture )<<'\n';
    glBindTexture(GL_TEXTURE_2D,0);
    return t;
}



Texture ammo;
Texture salama;

unsigned explosionTex;
unsigned groundTex;
unsigned buildingTex;
unsigned healthTex;

void initTextures()
{
    ammo = getAmmo();
    salama = getSalama();
	explosionTex = genExplosionTex();
    groundTex = genGroundTex();
    buildingTex = genBuildingTex();
    healthTex = genHealthTex();
}
