#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include "Connection.hpp"

struct Game;
struct DataReader;
struct ClientSocket {
	ClientSocket(Game& g);
	Game& g;
	bool connect(const char* host,const char* nick);
	void handleMessages();
	void readInit(DataReader r);
	void readState(DataReader r);
	void readBullet(DataReader r);
	void readHit(DataReader r);
	void readLightning(DataReader r);
    void readAddItem(DataReader r);
    void readDestroyItem(DataReader r);
	void readItems(DataReader r);
	void readBCount(DataReader r);
    void readName(DataReader r);
    void readDead(DataReader r);

    void readStats(DataReader r);
	void sendState();
	Connection conn;
};

#endif
