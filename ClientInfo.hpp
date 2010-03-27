#ifndef CLIENTINFO_HPP
#define CLIENTINFO_HPP

#include "Connection.hpp"
struct Server;
struct Unit;

struct ClientInfo {
	ClientInfo(Server& serv, int fd);

	void handleMessages();
	void sendInit();

	Server& server;
	Unit* u;
	Connection conn;
};

#endif
