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
	MenuItem& i = items[cur];
	if (i.type==STRING) {
		string& str = *i.str;
		if (press('\b') && !str.empty()) str.erase(str.end()-1);
		for(int i=32; i<123; ++i) {
			if (press(i)) str.push_back(i);
		}
	} else if (i.type==LIST) {
		if (press(SDLK_LEFT)) i.cur=(i.cur+i.lst.size()-1)%i.lst.size();
		if (press(SDLK_RIGHT)) i.cur=(i.cur+1)%i.lst.size();
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
//	setProjection();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glLineWidth(1);
	int ii=0;
	for(unsigned i=0; i<items.size(); ++i, ++ii) {
		double x=-.8, y=.7-2.5*size*ii;
		if ((int)i==cur) {
			glColor3f(.8,.5,1);
			glLineWidth(3);
			drawString(items[i].title, x, y, size);
			glLineWidth(1);
		}
		glColor3f(1,.3,1);
		drawString(items[i].title, x, y, size);

		MenuItem& c = items[i];
		if (c.type==STRING) drawString(c.str->c_str(), x+6*size, y, size);
		else if (c.type==LIST) {
			int xx=0;
			for(unsigned j=0; j<c.lst.size(); ++j) {
				y = .7-2.5*size*ii;
				const string& s = c.lst[j];
				double x0=-.3;
				if (x0 + .6*size*(xx+s.size()) >= .95) {
					++ii;
					xx=0;
					--j;
					continue;
				}
				if ((int)j==c.cur) drawString(s.c_str(), x0+.6*size*xx, y, size, .2, 1, .2, .5, 1, .5);
				else drawString(s.c_str(), x0+.6*size*xx, y, size, .2, 1, .2);
				xx += s.size()+1;
			}
		}
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
