#include "ClientSocket.hpp"
#include "DataReader.hpp"
#include "Server.hpp"
#include "msg.hpp"
#include "Game.hpp"
#include "Unit.hpp"
#include "DataWriter.hpp"
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
//		cout<<"got message "<<type<<'\n';
		switch(type) {
			case SRV_INIT:
				readInit(r);
				break;
			case SRV_STATE:
				readState(r);
			default: break;
		}
	}
}

void ClientSocket::readInit(DataReader r)
{
	g.id = r.readInt();
	cout<<"got ID "<<g.id<<'\n';
	int w=r.readInt(), h=r.readInt();
	Area& a=g.area;
	a.w=w,a.h=h;
	a.a = new int[w*h];
	memcpy(a.a, r.cur, 4*w*h);
}
void ClientSocket::readState(DataReader r)
{
	int n = r.readInt();
//	cout<<"reading "<<n<<" units\n";
	double d = g.player->d;
	g.units.resize(n);
	memcpy(&g.units[0], r.cur, n*sizeof(Unit));
	for(int i=0; i<n; ++i) if (g.units[i].type==0 && g.units[i].id==g.id) g.units[i].d=d;

//	Unit& u = g.units[0]; cout<<"jee "<<u.loc<<'\n';
}

void ClientSocket::sendState()
{
	Unit* p = g.player;
	DataWriter w;
	w.writeByte(CLI_STATE);
	w.writeInt(p->movex);
	w.writeInt(p->movey);
	w.writeDouble(p->d);
	conn.write(w.Buf,w.len());
}
