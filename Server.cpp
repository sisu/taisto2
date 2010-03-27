#include "Server.hpp"
#include "physics.hpp"
#include "DataWriter.hpp"
#include "msg.hpp"
#include "Area.hpp"
#include "Bot.hpp"
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "SDL.h"
using namespace std;

Server::Server(): end(0), nextID(1), area("field.in.1")
{
	clID = new int[1<<16];
	spawnTime = 0;
	initSocket();
}

void Server::loop()
{
	cout<<"starting server loop\n";
	double t=SDL_GetTicks()/1e3;
	while(!end) {
		double nt = SDL_GetTicks()/1e3;
		pollConnections();
		for(unsigned i=0; i<units.size(); ++i) {
			Unit& u = units[i];
			if (u.type!=0) continue;
			clients[clID[u.id]]->u = &u;
		}
		readInputs();
		updatePhysics(nt-t);
		sendState();
		t=nt;
		SDL_Delay(15);
	}
}
static int bulletid = 0;
void Server::updatePhysics(double t)
{
	spawnTime -= t;
	if (spawnTime <= 0) {
		Unit b(Vec2(7,5), 1, -1);
		units.push_back(b);
		spawnTime = 5;
	}
	for(unsigned i=0; i<units.size(); ++i) if (units[i].type!=0) moveBot(units[i]);
	moveUnits(&units[0], units.size(), area, t);
//	moveBullets(&bullets[0], bullets.size(), &units[0], units.size(), area, t);
	updateBullets(t);

	for(unsigned i=0; i<units.size(); ++i) {
		Unit& u = units[i];
		u.shootTime -= t;
		if (u.shooting && u.shootTime<=0) {
			int t = u.type>0 ? u.type-1 : clients[clID[u.id]]->weapon;
			u.shootTime = loadTimes[t];
			Vec2 v(cos(u.d),sin(u.d));
            Bullet b(u.loc, 1000*v, t, bulletid++);
            bullets.push_back(b);

			DataWriter w;
			w.writeByte(SRV_SHOOT);
			w.write(&b, sizeof(b));
			sendToAll(w);
		}
	}

	if (units.size()>1) {Unit& u = units[1]; cout<<"updated physics; "<<u.movex<<' '<<u.movey<<" ; "<<u.loc<<" ; "<<u.shooting<<' '<<u.id<<' '<<clID[u.id]<<'\n';}
}

void Server::initSocket()
{
	struct sockaddr_in serv_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(SERVER_PORT);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
		exit(1);
	}
	listen(sockfd,5);
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
}

void Server::pollConnections()
{
	while(true) {
		sockaddr_in cli_addr;
		int clilen=0;
		int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
		if (newsockfd < 0) break;
		cout<<"Newsockfd: "<<newsockfd<<endl;

#if 0
		const int BSIZE = 255;
		char buffer[BSIZE];

		// check if interested
		int n = read(newsockfd,buffer,BSIZE); 

		if(n < 0) {
			cout<<"CONTINUING"<<endl;
			continue;		
		}
#endif

		cout<<"Adding socket"<<endl;

		ClientInfo* cl = new ClientInfo(*this, newsockfd);
		clients.push_back(cl);
		cl->sendInit();
		units.push_back(Unit(area.getSpawn(), 0, cl->id));
		clID[cl->id] = clients.size()-1;
//		sockets[sockets_used] = newsockfd;
//		++sockets_used;

		fcntl(newsockfd, F_SETFL, O_NONBLOCK);


//		const char* message = "Connection established";
//		n = write(newsockfd,message,strlen(message));
	}
}
void Server::sendState()
{
	DataWriter w;
	w.writeByte(SRV_STATE);
	w.writeInt(units.size());
	w.write(&units[0], units.size() * sizeof(Unit));

#if 0
    w.writeInt(bullets.size());
	w.write(&bullets[0], bullets.size() * sizeof(Bullet));
#endif
	sendToAll(w.Buf, w.len());
}
void Server::sendToAll(const void* s, int n)
{
	for(unsigned i=0; i<clients.size(); ++i)
		clients[i]->conn.write(s,n);
}
void Server::sendToAll(DataWriter w)
{
	for(unsigned i=0; i<clients.size(); ++i)
		clients[i]->conn.write(w);
}

void Server::readInputs()
{
	for(unsigned i=0; i<clients.size(); ++i)
		clients[i]->handleMessages();
}

void Server::updateBullets(double t)
{
	for(unsigned i=0; i<bullets.size(); ++i) {
		Bullet& b = bullets[i];
		Vec2 l = b.loc + b.v*t;
		// FIXME: optimize
		for(unsigned j=0; j<units.size(); ++j) {
//			Unit& u = units[j];
		}

		double l2 = length2(l-b.loc);
		Vec2 c = b.loc;
		int dx=b.v.x>0?1:-1, dy=b.v.y>0?1:-1;
		int ix=c.x, iy=c.y;
		int iix=dx>0?ix+1:ix, iiy=dy>0?iy+1:iy;
		double ryx = fabs(b.v.y/b.v.x), rxy = fabs(b.v.x/b.v.y);
		bool hit=0;
		while(length2(c-b.loc) < l2) {
			if (area.blocked(ix,iy)) {
				hit=1;
				break;
			}
			double xx = fabs(iix-c.x);
			double yy = fabs(iiy-c.y);
			if (b.v.x*yy < b.v.y*xx) {
				c.y += dy * xx * ryx;
				c.x = iix;
				ix+=dx,iix+=dx;
			} else {
				c.x += dx * xx * rxy;
				c.y = iiy;
				iy+=dy, iiy+=dy;
			}
		}
		if (hit) {
			DataWriter w;
			w.writeByte(SRV_HIT);
			w.writeInt(b.id);
			sendToAll(w);
			bullets[i] = bullets.back();
			bullets.pop_back();
			--i;
		} else b.loc = l;
	}
}
