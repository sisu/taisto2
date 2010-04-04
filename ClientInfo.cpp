#include "ClientInfo.hpp"
#include "Server.hpp"
#include "msg.hpp"
#include "DataReader.hpp"
#include "DataWriter.hpp"
#include <iostream>
#include <errno.h>
#include <unistd.h>
//#include <sys/socket.h>
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
//	shutdown(conn.fd, SHUT_RDWR);
	close(conn.fd);
}

bool ClientInfo::handleMessages()
{
	while(conn.read()) {
		DataReader r(conn.buf+4);
		int type=r.readByte();
		cout<<"got message from client "<<type<<'\n';
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

volatile bool haxAllWeapons = true;//false;

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
    std::cout<<"nextid = "<<server.nextID<<"\n";
    for(int i=0;i<=server.nextID;i++)
    {
        std::cout<<"server.name["<<i<<"] == "<<server.name[i]<<"\n";
        if(server.name[i].size()){
            DataWriter w;
            w.writeByte(SRV_NAME);
            w.writeInt(i);
            char buf[33]={};
            memcpy(buf,server.name[i].c_str(),server.name[i].size());
            w.write(buf,32);
            conn.write(w);
            std::cout<<"sending name "<<buf<<" to "<<server.name[id]<<"\n";
        }
    }
	conn.flush();
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
    if(haxAllWeapons)
    {
        for(int i=0;i<8;i++)
            bcnt[i]=1000;
    }
    /*
       bcnt[ROCKET]=10000;
       bcnt[RAILGUN]=10000;
       <<<<<<< HEAD:ClientInfo.cpp
       bcnt[LIGHTNING]=10000;
       =======
       bcnt[GRENADE]=10000;
       >>>>>>> d614ab52da10e704625e7ba7e0d02d845a71c35d:ClientInfo.cpp
       */
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
    std::cout<<buf<<" == "<<server.name[id]<<" ("<<id<<") connected\n";
}
