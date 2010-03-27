#ifndef GAME_HPP
#define GAME_HPP

#include "Unit.hpp"
#include "Area.hpp"
#include "ClientSocket.hpp"
#include "Bullet.hpp"
#include <vector>

struct Game {
	Game();
	Area area;
	std::vector<Unit> units;
	std::vector<Bullet> bullets;
	std::vector<Bullet> lastBullets;

	ClientSocket socket;

	void updateState(double t);
	void updateNetwork();
	int id;
	Unit* player;
	int weapon;

	int* bulletIndex;
};

#endif
