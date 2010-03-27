#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include "Unit.hpp"
#include "ClientInfo.hpp"

struct Server {
	void loop();

	void updatePhysics();

	bool end;

	std::vector<Unit> units;
	std::vector<ClientInfo> clients;
};

#endif
