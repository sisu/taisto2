#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

struct Game;
struct ClientSocket {
	Game& g;
	bool connect(const char* host);

	int sockfd;
};

#endif
