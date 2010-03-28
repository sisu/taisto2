#include "Unit.hpp"
#include <cstdlib>
#include <cmath>

void moveBot(Unit& u)
{
	u.movey = -1;
	u.movex = (rand()%3)-1;
	u.shooting = 0;
	u.d = M_PI + M_PI*rand()/RAND_MAX;
}
