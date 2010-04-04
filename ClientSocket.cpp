#include "ClientSocket.hpp"
#include "DataReader.hpp"
#include "Server.hpp"
#include "msg.hpp"
#include "Game.hpp"
#include "Unit.hpp"
#include "music.hpp"
#include "DataWriter.hpp"
#include <sys/types.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#else
#include <winsock.h>
#endif
#include <cstring>
#include <fcntl.h>
#include <iostream>
using namespace std;

ClientSocket::ClientSocket(Game& g): g(g)
{
}

bool ClientSocket::connect(const char* host,const char* nick)
{
	hostent* serv = gethostbyname(host);
	if (!serv) {
		cout<<"gethostbyname failed\n";
		exit(16);
	}
	sockaddr_in addr;
	#ifdef WIN32
	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	#else
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	#endif
		#ifdef WIN32
	if (sockfd==INVALID_SOCKET) {
		cout<<"binding socket failed "<<WSAGetLastError()<<endl;
		exit(15);
	}
		#endif
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
#ifdef WIN32
	addr.sin_addr.s_addr = ((struct in_addr *)(serv->h_addr))->s_addr;
//    memcpy(serv->h_addr, &addr.sin_addr.s_addr, serv->h_length);
#else
	bcopy(serv->h_addr, &addr.sin_addr.s_addr, serv->h_length);
#endif
	addr.sin_port = htons(SERVER_PORT);
	if (::connect(sockfd, (sockaddr*)&addr, sizeof(addr))<0) {
		perror("Connecting failed");
		#ifdef WIN32
		cout<<"connect error: "<<WSAGetLastError()<<'\n';
		#endif
		return 0;
	}
	#ifndef WIN32
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	#else
    unsigned long x=1;
    ioctlsocket(sockfd, FIONBIO, &x);
    char flag=1;
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag))<0) {
    	perror("setsockopt TCP_NODELAY");
    	exit(16);
    }
	#endif
	conn.fd = sockfd;

    DataWriter w;
    w.writeByte(CLI_NAME);
    char buf[33]={};
    strncpy(buf,nick,31);
    w.write(buf,32);
    conn.write(w);
	return 1;
}

void ClientSocket::handleMessages()
{
	while(conn.read()) {
		DataReader r(conn.buf+4);
		int type=r.readByte();
		cout<<"got message from server "<<type<<'\n';
		switch(type) {
			case SRV_INIT:
				readInit(r);
				break;
			case SRV_STATE:
				readState(r);
				break;
			case SRV_SHOOT:
				readBullet(r);
				break;
			case SRV_HIT:
				readHit(r);
				break;
			case SRV_LIGHTNING:
				readLightning(r);
				break;
			case SRV_BASE:
				g.curBase = r.readInt();
				if (g.curBase==g.area.bases.size()-2) g.defenceTime = DEFENCE_TIME;
				break;
            case SRV_ADDITEM:
                readAddItem(r);
				break;
            case SRV_DELITEM:
                readDestroyItem(r);
				break;
			case SRV_BCOUNT:
				readBCount(r);
				break;
			case SRV_ITEMS:
				readItems(r);
				break;
			case SRV_SPAWNTIME:
				g.nextSpawn = r.readFloat();
				break;
            case SRV_NAME:
                readName(r);
                break;
            case SRV_STATS:
                readStats(r);
                break;
            case SRV_DEAD:
                readDead(r);
                break;
			defaelt:
				cout<<"Unknown message "<<type<<'\n';
				break;
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
	r.read(a.a, 4*w*h);
	int b = r.readInt();
	a.bases.resize(b);
	r.read(&a.bases[0], 4*b);
	cout<<"got map "<<a.w<<' '<<a.h<<'\n';
}
void ClientSocket::readState(DataReader r)
{
	int n = r.readInt();
//	cout<<"reading "<<n<<" units\n";
	double d = g.player->d;
	int mx=g.player->movex, my=g.player->movey;
	g.units.resize(n);
	memcpy(&g.units[0], r.cur, n*sizeof(Unit));
	for(int i=0; i<n; ++i) if (g.units[i].type==0 && g.units[i].id==g.id) {
		Unit& u = g.units[i];
		u.d=d;
		u.movex = mx, u.movey = my;
		break;
	}
//	Unit& u = g.units[0]; cout<<"jee "<<u.loc<<'\n';
}
static double lastFlame=0;
void ClientSocket::readBullet(DataReader r)
{
    Bullet b;
    r.read(&b,sizeof(Bullet));
    double vol = distvol(length(g.player->loc-b.origin));;
	if (b.type==ROCKET) {
	}else if (b.type==SHOTGUN)
    {
		sounds.push_back(Sound(SHOTGUNSOUND, vol));
    }else if(b.type==BOUNCEGUN)
    {
		sounds.push_back(Sound(MACHINEGUNSOUND,vol));
    }else if(b.type==MACHINEGUN)//MACHINEGUN == FLAMEGUN
    {
        //if(-lastFlame+timef()>0.4){
            sounds.push_back(Sound(FLAMESOUND,vol));
            //lastFlame=timef();
        //}
    }
    g.bullets_map.insert(b);
	//g.bullets.push_back(*(Bullet*)r.cur);
	//g.bulletIndex[g.bullets.back().id] = g.bullets.size()-1;
}
#include "timef.h"
void ClientSocket::readHit(DataReader r)
{
	int id = r.readInt();
	float x=r.readFloat(), y=r.readFloat();
	g.destroyBullet(id,x,y);
}

void ClientSocket::readAddItem(DataReader r)
{
    Item it = *(Item*)r.cur;
    std::cout<<"received item "<<it.id<<"\n";
    g.items_map.insert(it);
}
void ClientSocket::readDestroyItem(DataReader r)
{
	int id = r.readInt();
    std::cout<<"readDestroyItem "<<id<<"\n";
    g.items_map.remove(id);
}

void ClientSocket::readLightning(DataReader r)
{
	int cnt = r.readInt();
	vector<int> ids((int*)r.cur, ((int*)r.cur)+cnt);
	g.lightnings.push_back(make_pair(0.0, ids));
//	cout<<"adding lightning "<<cnt<<'\n';
}
void ClientSocket::readItems(DataReader r)
{
	int n=r.readInt();
	Item it;
	for(int i=0; i<n; ++i) {
		r.read(&it, sizeof(it));
		g.items_map.insert(it);
	}
}
void ClientSocket::readBCount(DataReader r)
{
	r.read(g.bcnt, 8*4);
	while(g.weapon && !g.bcnt[g.weapon]) --g.weapon;
}
void ClientSocket::readName(DataReader r){
    int id = r.readInt();
    char buf[33];
    r.read(buf,32);
    if(g.names.size()<=id)g.names.resize(id+20);
    g.names[id] = buf;
    std::cout<<id<<" is "<<buf<<"\n";
}
void ClientSocket::readStats(DataReader r){
    int n = r.readInt();
    g.kills.clear();
    g.teamkills.clear();
    g.deaths.clear();

    g.kills.resize(2000,-1);
    g.teamkills.resize(2000,-1);
    g.deaths.resize(2000,-1);
    for(int i=0;i<n;i++)
    {
        int id = r.readInt();
        g.kills[id]=r.readInt();
        g.teamkills[id]=r.readInt();
        g.deaths[id]=r.readInt();
    }
}
void ClientSocket::readDead(DataReader r){
    int n = r.readInt();
    for(int i=0;i<n;i++)
    {
        Unit u;
        r.read(&u,sizeof(Unit));
        u.shootTime = timef();
        g.deadUnits.push_back(u);
    }
}

void ClientSocket::sendState()
{
	Unit* p = g.player;
	DataWriter w;
	w.writeByte(CLI_STATE);
	w.writeInt(p->movex);
	w.writeInt(p->movey);
	w.writeDouble(p->d);
	w.writeByte(p->shooting);
	w.writeInt(g.weapon);
	conn.write(w);
}
