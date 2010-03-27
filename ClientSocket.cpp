#include "ClientSocket.hpp"
#include "DataReader.hpp"
#include "Server.hpp"
#include "msg.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <netdb.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

ClientSocket::ClientSocket(Game& g): g(g)
{
}

bool ClientSocket::connect(const char* host)
{
	hostent* serv = gethostbyname(host);
	sockaddr_in addr;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	bcopy(serv->h_addr, &addr.sin_addr.s_addr, serv->h_length);
	addr.sin_port = htons(SERVER_PORT);
	if (::connect(sockfd, (sockaddr*)&addr, sizeof(addr))<0) return 0;
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	conn.fd = sockfd;

	return 1;
}

void ClientSocket::handleMessages()
{
	while(conn.read()) {
		DataReader r(conn.buf+4);
		int type=r.readByte();
		cout<<"got message "<<type<<'\n';
		switch(type) {
			case SRV_INIT:
				readInit(r);
				break;
			default: break;
		}
	}
}

void ClientSocket::readInit(DataReader r)
{
	cout<<"reading init\n";
}
