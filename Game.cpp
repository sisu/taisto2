#include "Game.hpp"

Game::Game(): socket(*this)
{
}
void Game::updateNetwork()
{
	socket.handleMessages();
}
void Game::updateState()
{
}
