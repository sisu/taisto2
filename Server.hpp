#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include "Unit.hpp"
#include "ClientInfo.hpp"
#include "Area.hpp"
#include "Bullet.hpp"

const int SERVER_PORT = 56125;

struct Server {
	Server();
	void loop();
	void updatePhysics(double t);

	void initSocket();
	void pollConnections();
	void readInputs();
	void sendState();
	void sendToAll(const void* s, int n);

	bool end;

	std::vector<Unit> units;
	std::vector<ClientInfo*> clients;
	std::vector<Bullet> bullets;

	int sockfd;
	int nextID;

	Area area;
	int* clID;
};

#endif
