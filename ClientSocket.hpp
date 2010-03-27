#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include "Connection.hpp"

struct Game;
struct DataReader;
struct ClientSocket {
	ClientSocket(Game& g);
	Game& g;
	bool connect(const char* host);
	void handleMessages();
	void readInit(DataReader r);
	void readState(DataReader r);
	void readBullet(DataReader r);
	void readHit(DataReader r);
	void sendState();

	Connection conn;
};

#endif
