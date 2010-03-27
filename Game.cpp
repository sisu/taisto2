#include "Game.hpp"

Game::Game(): socket(*this)
{
	weapon=0;
}
void Game::updateNetwork()
{
	socket.sendState();
	socket.handleMessages();
}
void Game::updateState()
{
}
