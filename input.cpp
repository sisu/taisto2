#include "SDL.h"
#include <cstdlib>
bool keyboard[512];
int mouseX, mouseY;
bool mouse[4];

void readInput()
{
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if (e.type==SDL_QUIT) exit(0);
    }

    SDL_PumpEvents();
    Uint8* keys = SDL_GetKeyState(0);
    for(int i=0; i<300; ++i)
        keyboard[i]=keys[i];

    int mstate = SDL_GetMouseState(&mouseX, &mouseY);
    for(int i=0; i<3; ++i) mouse[i] = mstate & SDL_BUTTON(1+i);
}
