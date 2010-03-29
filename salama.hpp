#ifndef SALAMA_HPP
#define SALAMA_HPP
#include<cstdlib>
#include"Game.hpp"
/*
static inline float randf()
{
    return rand()/float(RAND_MAX);
}*/

void drawSalama(Game& game,Vec2 orig,Vec2* targets,int n);
void drawSalama(float x,float y,float tx,float ty);

#endif
