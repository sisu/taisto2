#include "Game.hpp"
#include "physics.hpp"

Game::Game(): socket(*this)
{
	weapon=0;
}
void Game::updateNetwork()
{
	socket.sendState();
	socket.handleMessages();
}
void Game::updateState(double t)
{
	moveUnits(&units[0], units.size(), area, t);
	moveBullets(&bullets[0], bullets.size(), &units[0], units.size(), area, t);
}
