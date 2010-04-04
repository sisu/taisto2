#include "music.hpp"
#include<SDL/SDL.h>
#include<iostream>


int main(int argc, char* argv[])
{
    SDL_Init( SDL_INIT_AUDIO | SDL_INIT_TIMER);
    initMusic();
	SDL_PauseAudio(0);
    //sounds.push_back(Sound(EXPLOSION, .5));
    //sounds.push_back(Sound(SHOTGUNSOUND, .5));
    //sounds.push_back(Sound(MACHINEGUNSOUND, .5));
    //sounds.push_back(Sound(FLAMESOUND, .5));
	sounds.push_back(Sound(ELECTROSOUND, .5));
	bool done=0;
    int time = 0;
	while(!done) {
        time++;
        std::cout<<time*50<<"\n";
        if(time*50>500)
        {
            return 0;
        }
		SDL_Delay(50);
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if (e.type==SDL_QUIT) done=1;
		}
	}
	SDL_PauseAudio(1);

    SDL_Delay(1000*5);
}
