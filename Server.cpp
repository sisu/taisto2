#include "Server.hpp"

void Server::loop()
{
	while(!end) {
		updatePhysics();
	}
}

void Server::updatePhysics()
{
}
