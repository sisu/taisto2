#include "Unit.hpp"
#include <cstdlib>

void moveBot(Unit& u)
{
	u.movey = 1;
	u.movex = (rand()%3)-1;
}
