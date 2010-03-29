#include "ClientInfo.hpp"
#include "Server.hpp"
#include "msg.hpp"
#include "DataReader.hpp"
#include "DataWriter.hpp"
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <cassert>
using namespace std;

ClientInfo::ClientInfo(Server& s, int fd): server(s), weapon(0)
{
	conn.fd=fd;
	u=0;
	spawnTime=0;
}
ClientInfo::~ClientInfo()
{
	delete[] conn.buf;
	shutdown(conn.fd, SHUT_RDWR);
	close(conn.fd);
}

bool ClientInfo::handleMessages()
{
	while(conn.read()) {
		DataReader r(conn.buf+4);
		int type=r.readByte();
//		cout<<"got message "<<type<<'\n';
		switch(type) {
			case CLI_STATE:
				readState(r);
				break;
			default:
				cout<<"Warning: unknown message type "<<type<<'\n';
                assert(false);
				break;
		}
	}
	if (errno!=EAGAIN) return 0;
	return 1;
}

void ClientInfo::sendInit()
{
	DataWriter w;
	w.writeByte(SRV_INIT);
	id = server.nextID++;
	w.writeInt(id);
	Area& a = server.area;
	w.writeInt(a.w);
	w.writeInt(a.h);
	w.write(a.a, 4*a.w*a.h);
	conn.write(w);
	cout<<"sent init "<<w.len()<<'\n';;
}
void ClientInfo::readState(DataReader r)
{
	if (u) {
		u->movex = r.readInt();
		u->movey = r.readInt();
		u->d = r.readDouble();
		u->shooting = r.readByte();
	} else r.cur+=4+4+8+1;
	weapon = r.readInt();
}
