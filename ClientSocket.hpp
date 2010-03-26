#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

struct Game;
struct ClientSocket {
	Game& g;
	void connect(const char* host);
};

#endif
