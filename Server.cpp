#include "Server.hpp"
#include "physics.hpp"
#include "DataWriter.hpp"
#include "msg.hpp"
#include "Area.hpp"
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
			if (u.type==0) {
				unsigned k;
				for(k=0; k<clients.size(); ++k) if (clients[k]->id==u.id) {
					clients[k]->u = &u;
					break;
				}
			}
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
	moveUnits(&units[0], units.size(), area, t);
	moveBullets(&bullets[0], bullets.size(), &units[0], units.size(), area, t);

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
			sendToAll(w.Buf, w.len());
		}
	}

	if (!units.empty()) {
		Unit& u = units[0]; cout<<"updated physics; "<<u.movex<<' '<<u.movey<<" ; "<<u.loc<<" ; "<<u.shooting<<'\n';
	}
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
		clID[cl->id] = units.size()-1;
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

void Server::readInputs()
{
	for(unsigned i=0; i<clients.size(); ++i)
		clients[i]->handleMessages();
}
