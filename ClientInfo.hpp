#ifndef CLIENTINFO_HPP
#define CLIENTINFO_HPP

struct Server;

struct ClientInfo {
	ClientInfo(Server& serv, int fd);

	Server& server;
	int fd;
};

#endif
