#ifndef BOT_INFORMATION
#define BOT_INFORMATION

#include <vector>
#include "Vector.hpp"
#include "timef.h"

struct BotInformation {
	int id; 
	int planTime;
	bool stayStill;
    bool enemyFound;
	std::vector<Vec2> plan;

	BotInformation() {
		id = planTime = stayStill = 0;
        //planTime = randf();
        //plan.push_back(Vec2(0,0));
	}
};

#endif
