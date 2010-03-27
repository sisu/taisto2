#ifndef TIMEF_H
#define TIMEF_H

#include <SDL/SDL.h>
static float timef()
{
    int t = SDL_GetTicks();
    return t/1000.0;
}

#endif
