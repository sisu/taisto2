#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include "texgen.hpp"
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

	return fx*fy*n4 + (1.0-fx)*fy*n3 + fx*(1.0-fy)*n2 + (1.0-fx)*(1.0-fy)*n1;
}
double turbulence(double x, double y, double size)
{
	double value = 0.0, asize = size;
	while(size >= 1.0) {
		value += smooth(x/size, y/size) * size;
		size /= 2;
	}
	return .5 * value / asize;
}

const int TS=256;
float texture[TS][TS][4];
GLuint genExplisionTex()
{
	genNoise();

	for(int i=0; i<TS; ++i) {
		for(int j=0; j<TS; ++j) {
//			double x=2.*(j-TS/2)/TS;
//			double y=2.*(i-TS/2)/TS;
			double x=double(j)/TS, y=double(i)/TS;
			texture[i][j][0] = .6+.2*rndf();
			texture[i][j][1] = .6+.2*rndf();
			texture[i][j][2] = .2*rndf();
			texture[i][j][3] = turbulence(x,y,64);
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
            GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, texture );
    glBindTexture(GL_TEXTURE_2D,0);
    return t;
}
