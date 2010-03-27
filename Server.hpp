#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include "Unit.hpp"
#include "ClientInfo.hpp"
#include "Area.hpp"

const int SERVER_PORT = 56125;

struct Server {
	Server();
	void loop();
	void updatePhysics();

	void initSocket();
	void pollConnections();
	void readInputs();
	void sendState();
	void sendToAll(const void* s, int n);

	bool end;

	std::vector<Unit> units;
	std::vector<ClientInfo*> clients;

	int sockfd;
	int nextID;

	Area area;
};

#endif
