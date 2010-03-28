#ifndef SALAMA_HPP
#define SALAMA_HPP
#include<cstdlib>
static float randf()
{
    return rand()/float(RAND_MAX);
}

void drawSalama(float x,float y,float tx,float ty);

#endif
