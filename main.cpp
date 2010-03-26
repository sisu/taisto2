#include "Game.hpp"
#include "SDL.h"

bool gameEnd;

bool keyboard[256];
int mouseX, mouseY;
bool mouse[4];
int screenW, screenH;

void readInput()
{
	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		if (e.type==SDL_QUIT) gameEnd=1;
	}

	SDL_PumpEvents();
	Uint8* keys = SDL_GetKeyState(0);
	for(int i=0; i<256; ++i) keyboard[i]=keys[i];

	int mstate = SDL_GetMouseState(&mouseX, &mouseY);
	for(int i=0; i<3; ++i) mouse[i] = mstate & SDL_BUTTON(1+i);
}

void mainLoop()
{
	Game game;
	game.socket.connect("127.0.0.1");

	while(!gameEnd) {
		readInput();
		game.updateNetwork();
		game.updateState();
		SDL_GL_SwapBuffers();
	}
}

int main()
{
	screenW=1024, screenH=768;
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
	SDL_SetVideoMode(screenW,screenH,0,SDL_OPENGL);

	mainLoop();

	SDL_Quit();
}
