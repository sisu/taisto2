#include "Game.hpp"

Game::Game(): socket(*this)
{
}
void Game::updateNetwork()
{
	socket.sendState();
	socket.handleMessages();
}
void Game::updateState()
{
}
