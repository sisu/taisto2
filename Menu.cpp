#include "Menu.hpp"
#include "input.hpp"
#include "LCD.hpp"
#include <GL/gl.h>
#include "SDL.h"
#include <cstring>
#include <iostream>
using namespace std;

bool prevkb[512];

bool press(int c)
{
	return !prevkb[c] && keyboard[c];
}

bool Menu::handleInput()
{
	if (press(27)) return 0;
	if (press(SDLK_UP)) cur=(cur+items.size()-1)%items.size();
	if (press(SDLK_DOWN)) cur=(cur+1)%items.size();
	if (press('\r')) {
		MenuItem& m = items[cur];
		if (m.type==EXIT) return 0;
		if (m.type==PICK) m.func();
	}
	if (items[cur].type==STRING) {
		string& str = *items[cur].str;
		if (press('\b') && !str.empty()) str.erase(str.end()-1);
		for(int i=32; i<123; ++i) {
			if (press(i)) str.push_back(i);
		}
	}
	return 1;
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
	glMatrixMode(GL_VIEWPORT);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
//	setProjection();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glLineWidth(1);
	for(unsigned i=0; i<items.size(); ++i) {
		double x=-.5, y=.5-.3*i;
		if ((int)i==cur) {
			glColor3f(.8,.5,1);
			glLineWidth(3);
			drawString(items[i].title, x, y, .1);
			glLineWidth(1);
		}
		glColor3f(1,.3,1);
		drawString(items[i].title, x, y, .1);

		if (items[i].type==STRING) drawString(items[i].str->c_str(), x+.6, y, .1);
	}
}

void Menu::exec()
{
	while(1) {
		memcpy(prevkb,keyboard,512);
		readInput();
		if (!handleInput()) break;
		draw();

		SDL_GL_SwapBuffers();
		SDL_Delay(50);
	}
}
