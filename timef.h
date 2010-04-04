#ifndef TIMEF_H
#define TIMEF_H

#include <SDL/SDL.h>
#include <cstdlib>
inline static float timef()
{
    int t = SDL_GetTicks();
    return t/1000.0;
}
inline static float randf()
{
    return rand()/float(RAND_MAX);
}

#endif
