#ifndef CLIENTINFO_HPP
#define CLIENTINFO_HPP

#include "Connection.hpp"
struct Server;
struct Unit;
struct DataReader;

struct ClientInfo {
	ClientInfo(Server& serv, int fd);
	~ClientInfo();

	bool handleMessages();
	void sendInit();
	void readState(DataReader r);
	void sendBCounts();
    void sendName(DataReader r);

	Server& server;
	Unit* u;
	Connection conn;
	int id;
	int weapon;
	double spawnTime;

	int bcnt[8];
};

#endif
