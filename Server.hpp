#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include "Unit.hpp"
#include <string>
#include "ClientInfo.hpp"
#include "Item.hpp"
#include "IDMap.hpp"
#include "Area.hpp"
#include "Bullet.hpp"
#include "BotInformation.hpp"

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
	void sendStats();t
	void sendToAll(const void* s, int n);
	void sendToAll(DataWriter& w);
	void updateBullets(double t);
	void updateBases();
	void damageUnit(int i, double d);
	void spawnUnits(double t);
	void genSpawnCounts();
	void spawnClient(ClientInfo& c);
    void sendStats();

	volatile bool end;

	std::vector<Unit> units;
	std::vector<ClientInfo*> clients;
	std::vector<Bullet> bullets;
    IDMap<Item> items_map;
	std::vector<BotInformation*> botinfos;

	int sockfd;
	int nextID;

	Area area;
	int* clID;
	double spawnTime;

	int curSpawn;
	int botID;

	int spawnCounts[32][64]; // [type][base]
	int itemSpawns[32][64];
    int enemyCounts[32];
	double unitMove;
    double flowSpawnTime;
    int stats[320][3];//kills - teamkills - deaths
    std::string name[320];
};

#endif
