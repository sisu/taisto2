#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include "Unit.hpp"
#include "ClientInfo.hpp"
#include "Area.hpp"
#include "Bullet.hpp"

const int SERVER_PORT = 56125;

struct DataWriter;
struct Server {
	Server();
	~Server();
	void loop();
	void updatePhysics(double t);

	void initSocket();
	void pollConnections();
	void readInputs();
	void sendState();
	void sendToAll(const void* s, int n);
	void sendToAll(DataWriter w);
	void updateBullets(double t);
	void updateBases();
	void damageUnit(int i, double d);

	volatile bool end;

	std::vector<Unit> units;
	std::vector<ClientInfo*> clients;
	std::vector<Bullet> bullets;

	int sockfd;
	int nextID;

	Area area;
	int* clID;
	double spawnTime;

	int curSpawn;
};

#endif
