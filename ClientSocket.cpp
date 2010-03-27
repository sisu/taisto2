#include "ClientSocket.hpp"
#include "Server.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <netdb.h>

bool ClientSocket::connect(const char* host)
{
	hostent* serv = gethostbyname(host);
	sockaddr_in addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	bcopy((char*)serv->h_addr, (char*)addr.sin_addr.s_addr, serv->h_length);
	addr.sin_port = htons(SERVER_PORT);
	if (::connect(sockfd, (sockaddr*)&addr, sizeof(addr))<0) return 0;
	return 1;
}
