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

Server::Server(): end(0), nextID(1),area(40,300)// area("field.in.1")
{
	for(int i=2; i<area.h; i+=30) area.bases.push_back(i);
	clID = new int[1<<16];
	spawnTime = 0;
	curSpawn=0;
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
		updateBases();
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
		Unit b(area.getSpawn(curSpawn+1), 1, -1);
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

			Bullet b = genBullet(t, u.loc, u.d, bulletid++);
//			Vec2 v(cos(u.d),sin(u.d));
  //          Bullet b(u.loc+.1*v, 1000*v, t, bulletid++);
            bullets.push_back(b);

			DataWriter w;
			w.writeByte(SRV_SHOOT);
			w.write(&b, sizeof(b));
			sendToAll(w);
		}
	}

//	if (units.size()>1) {Unit& u = units[1]; cout<<"updated physics; "<<u.movex<<' '<<u.movey<<" ; "<<u.loc<<" ; "<<u.shooting<<' '<<u.id<<' '<<clID[u.id]<<'\n';}
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
		units.push_back(Unit(area.getSpawn(curSpawn), 0, cl->id));
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
	sendToAll(w);
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

static const double shields[]={10,2.8};
void Server::updateBullets(double t)
{
	for(unsigned i=0; i<bullets.size(); ++i) {
		Bullet& b = bullets[i];
		Vec2 l = b.loc + b.v*t;
		// FIXME: optimize
		Vec2 nv = normalize(b.v);
		double bdd2=1e50, bj=-1;
		for(unsigned j=0; j<units.size(); ++j) {
			Unit& u = units[j];
			Vec2 w = u.loc-b.loc;
			if (dot(w,nv)<0) continue;
			double d = fabs(cross(w, nv));
			if (d>.4) continue;
			double dd2 = length2(w) - d*d;
			if (dd2 < bdd2) bdd2=dd2, bj=j;
		}
//		if (bj>=0) cout<<"asd "<<bj<<' '<<bdd2<<' '<<sqrt(bdd2)<<'\n';

		double l2 = length2(b.v*t);
		if (bdd2<l2) l2=bdd2;
		else bj=-1;
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
			if (fabs(b.v.x)*yy > fabs(b.v.y)*xx) {
				c.y += dy * xx * ryx;
				c.x = iix;
				ix+=dx,iix+=dx;
			} else {
				c.x += dx * yy * rxy;
				c.y = iiy;
				iy+=dy, iiy+=dy;
			}
		}
		if (bj>=0 || hit) {
			if (!hit) {
				c=b.loc + normalize(b.v)*sqrt(l2);
				Unit& u =units[bj];
				u.health -= damages[b.type]/shields[u.type];
				if (u.health<0) {
					if (u.type==0) clients[clID[u.id]]->u=0;
					units[bj] = units.back();
					units.pop_back();
				}
			}
//			cout<<"collision @ "<<c<<' '<<b.loc<<' '<<length(c-b.loc)<<'\n';
			DataWriter w;
			w.writeByte(SRV_HIT);
			w.writeInt(b.id);
			w.writeFloat(c.x);
			w.writeFloat(c.y);
			sendToAll(w);
			bullets[i] = bullets.back();
			bullets.pop_back();
			--i;
		} else b.loc = l;
	}
}
void Server::updateBases()
{
	int y1=area.bases[curSpawn], y2=area.bases[curSpawn+1];
	int c1=0,c2=0;
	for(unsigned i=0; i<units.size(); ++i) {
		Unit& u = units[i];
//		cout<<"u "<<u.type<<'\n';
		int a=u.type==0?1:-1;
		if (abs(u.loc.y-y1)<3) c1+=a;
		if (abs(u.loc.y-y2)<3) c2+=a;
	}
//	cout<<"zxc "<<curSpawn<<' '<<c1<<' '<<c2<<'\n';
	if (curSpawn && c1<0) {
		--curSpawn;
		cout<<"lost base\n";
	} else if (c2>0) {
		++curSpawn;
		cout<<"updating base "<<curSpawn<<'\n';
	}
}
