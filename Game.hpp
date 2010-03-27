#ifndef GAME_HPP
#define GAME_HPP

#include "Unit.hpp"
#include "Area.hpp"
#include "ClientSocket.hpp"
#include <vector>

struct Game {
	Game();
	Area area;
	std::vector<Unit> units;

	ClientSocket socket;

	void updateState();
	void updateNetwork();
	int id;
	Unit* player;
};

#endif
