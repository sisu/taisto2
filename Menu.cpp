#include "Menu.hpp"
#include "input.hpp"
#include "LCD.hpp"
#include <GL/gl.h>
#include "SDL.h"
#include <cstring>
#include <iostream>
using namespace std;

bool prevkb[256];

bool press(int c)
{
	return !prevkb[c] && keyboard[c];
}

void Menu::handleInput()
{
	if (press('w')) cur=(cur+items.size()-1)%items.size();
	if (press('s')) cur=(cur+1)%items.size();
}

#if 0
static void setProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
}
#endif

void Menu::draw()
{
//	setProjection();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	
	glLineWidth(1);
	for(unsigned i=0; i<items.size(); ++i) {
		double x=0, y=.5-.3*i;
		if ((int)i==cur) {
			glColor3f(.8,.5,1);
			glLineWidth(3);
			drawString(items[i].title, x, y, .1);
			glLineWidth(1);
		}
		glColor3f(1,.3,1);
		drawString(items[i].title, x, y, .1);
	}
}

void Menu::exec()
{
	while(1) {
		memcpy(prevkb,keyboard,256);
		if (!readInput()) break;
		handleInput();
		draw();

		SDL_GL_SwapBuffers();
	}
}
