#include "music.hpp"
#include "SDL.h"

extern bool playMusic;

int main()
{
	playMusic=1;
	initInstruments();
	initSoundSystem();
	SDL_PauseAudio(0);
	bool done=0;
	while(!done) {
		SDL_Delay(50);
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if (e.type==SDL_QUIT) done=1;
		}
	}
	SDL_PauseAudio(1);
	SDL_Quit();
}
