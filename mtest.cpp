#include "music.hpp"
#include "mplay.hpp"
#include "SDL.h"

extern bool playMusic;
extern bool playSounds;
bool inGame=1;

int main(int argc, char* argv[])
{
#if 0
	if (argc>1) {
		double s = atof(argv[1]);
		curPos = int(FREQ * s) / SAMPLES * SAMPLES;
		int ms = 1000*curPos/FREQ;
		for(int i=0; i<3; ++i) {
			while(notes[i][curNote[i]+1].start < ms) ++curNote[i];
			cout<<"start note "<<i<<' '<<curNote[i]<<' '<<notes[i][curNote[i]].start<<'\n';
		}
		while(notes2337[curNote[3]+1].start < ms) ++curNote[3];
	}
#endif
	playMusic=1;
	playSounds=0;
	initMusic();
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
