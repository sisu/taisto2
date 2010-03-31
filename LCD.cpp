#include <GL/gl.h>
#include <iostream>
using namespace std;

static short acontains[] = {379,941,579,909,595,83,867,378,132,840,10372,578,3402,9546,843,123,9035,8315,819,133,842,9480,12618,15360,3200,6657,843,264,633,825,314,819,883,265,891,827};
static char llocs[][4] = {
	{0,2,2,2},
	{0,2,0,1},
	{1,2,1,1},
	{2,2,2,1},
	{0,1,1,1},
	{1,1,2,1},
	{0,1,0,0},
	{1,1,1,0},
	{2,1,2,0},
	{0,0,2,0},
	{0,2,1,1},
	{1,1,2,2},
	{0,0,1,1},
	{1,1,2,0}
};

int contains[256];
void printChar(int c, float x0, float y0, float w, float h)
{
	for(int i=0; i<14; ++i) {
		if (c & (1<<i)) {
//			cout<<"jee "<<i<<'\n';
			glVertex2f(x0+w*llocs[i][0], y0+h*llocs[i][1]);
			glVertex2f(x0+w*llocs[i][2], y0+h*llocs[i][3]);
		}
	}
}

void initLCD()
{
	for(int i=0; i<='z'-'a'; ++i) contains['a'+i]=acontains[i];
	for(int i=0; i<10; ++i) contains['0'+i]=acontains['z'-'a'+1+i];
	contains['-'] = 16+32;
}

void drawString(const char* str, double x, double y, double s)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	//glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	//glLoadIdentity();
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_CULL_FACE);
	glDepthMask(0);

	glBegin(GL_LINES);
	for(int i=0; str[i]; ++i) printChar(contains[(unsigned char)str[i]], x+i*.6*s, y, .25*s, .5*s);
	glEnd();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(1);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
void drawString(const char* str, double x, double y, double s, double r, double g, double b)
{
	glColor3f(r,g,b);
	drawString(str,x,y,s);
}
void drawString(const char* str, double x, double y, double s, double r, double g, double b, double r2, double g2, double b2)
{
	glLineWidth(3);
	glColor3f(r,g,b);
	drawString(str,x,y,s);
	glLineWidth(1);
	glColor3f(r2,g2,b2);
	drawString(str,x,y,s);
}
