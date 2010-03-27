#ifndef CLIENTINFO_HPP
#define CLIENTINFO_HPP

#include "Connection.hpp"
struct Server;
struct Unit;
struct DataReader;

struct ClientInfo {
	ClientInfo(Server& serv, int fd);

	void handleMessages();
	void sendInit();
	void readState(DataReader r);

	Server& server;
	Unit* u;
	Connection conn;
	int id;
};

#endif
