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
	memset(bcnt,0,sizeof(bcnt));
//	for(int i=0; i<8; ++i) bcnt[i]=1e8;
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
            case CLI_NAME:
                sendName(r);
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
	w.writeInt(a.bases.size());
	w.write(&a.bases[0], 4*a.bases.size());
	conn.write(w);
	cout<<"sent init "<<w.len()<<'\n';;
    
	{
		DataWriter w;
		w.writeByte(SRV_ITEMS);
		w.writeInt(server.items_map.size());
		for(int i=0; i<server.items_map.size(); ++i) w.write(&server.items_map[i], sizeof(Item));
		conn.write(w);
	}
	{
		DataWriter w;
		w.writeByte(SRV_BASE);
		w.writeInt(server.curSpawn);
		conn.write(w);
	}
	{
		DataWriter w;
		w.writeByte(SRV_SPAWNTIME);
		w.writeFloat(server.spawnTime);
		conn.write(w);
	}
	sendBCounts();
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
void ClientInfo::sendBCounts()
{
	DataWriter w;
	w.writeByte(SRV_BCOUNT);
	w.write(bcnt, 8*4);
	conn.write(w);
}
void ClientInfo::sendName(DataReader r)
{
    DataWriter w;
    //int id = r.readInt();
    char buf[32]={};
    r.read(buf,32);
    server.name[id]=buf;
    w.writeByte(SRV_NAME);
    w.writeInt(id);
    w.write(buf,32);
    server.sendToAll(w);
    std::cout<<buf<<" connected\n";
}
