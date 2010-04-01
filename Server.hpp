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
const double DEFENCE_TIME = 60;

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
	void sendToAll(DataWriter& w);
	void updateBullets(double t);
	void updateBases();
	void damageUnit(int i, double d,int shooter,Bullet b);
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
	double defenceTime;

	int curSpawn;
	int botID;
	bool win;

	int spawnCounts[32][64]; // [type][base]
	int itemSpawns[32][64];
    int enemyCounts[32];
	double unitMove;
    double flowSpawnTime;
    std::vector<int> kills;
    std::vector<int> deaths;
    std::vector<int> teamkills;
    std::string name[320];
    std::vector<int> unitID;
};

#endif
