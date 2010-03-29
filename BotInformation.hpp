#ifndef BOT_INFORMATION
#define BOT_INFORMATION

#include <vector>
#include "Vector.hpp"

struct BotInformation {
	int id; 
	int planTime;
	bool stayStill;
	std::vector<Vec2> plan;
};

#endif
