#ifndef GAME_HPP
#define GAME_HPP

#include "Unit.hpp"
#include "Area.hpp"
#include "ClientSocket.hpp"
#include <vector>

struct Game {
	Area area;
	std::vector<Unit> units;

	ClientSocket socket;

	void updateState();
	void updateNetwork();
};

#endif
