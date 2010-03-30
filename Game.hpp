#ifndef GAME_HPP
#define GAME_HPP

#include "Item.hpp"
#include "IDMap.hpp"
#include "Unit.hpp"
#include "Area.hpp"
#include "ClientSocket.hpp"
#include "Bullet.hpp"
#include "Particle.hpp"
#include <vector>
#include <utility>
struct Game {
	Game();
	Area area;
	std::vector<Unit> units;
#if 0
	std::vector<Bullet> bullets;
	int* bulletIndex;
#else 
    IDMap<Bullet> bullets_map;
#endif
    IDMap<Item> items_map;
    std::vector<Item>& items;
	std::vector<Bullet>& bullets;


	std::vector<Bullet> lastBullets;
	std::vector<Particle> particles;
	std::vector<std::pair<double, std::vector<int> > > lightnings;

	ClientSocket socket;

	void updateState(double t);
	void updateNetwork();
	void destroyBullet(int id, double x, double y);
	int id;
	Unit* player;
	int weapon;


    std::vector<int> unitIndex;
	int curBase;
};

#endif
