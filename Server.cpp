#include "Server.hpp"
#include "physics.hpp"
#include "DataWriter.hpp"
#include "msg.hpp"
#include "Area.hpp"
#include "Bot.hpp"
#include "timef.h"
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
static const double shields[]={10,2,2,0.2,2,2};

Server::Server(): end(0), nextID(1),area(30,900)// area("field.in.1")
{
	//for(int i=2; i<area.h; i+=30) area.bases.push_back(i);
	clID = new int[1<<16];
	spawnTime = 0;
	curSpawn=0;
	botID=256;
	initSocket();
	genSpawnCounts();
	unitMove = 0;
}
Server::~Server()
{
	cout<<"destructing server\n";
	for(unsigned i=0; i<clients.size(); ++i) delete clients[i];
	shutdown(sockfd, SHUT_RDWR);
	close(sockfd);
}

void Server::loop()
{
	cout<<"starting server loop\n";
	double t=timef();
	while(!end) {
		double nt = SDL_GetTicks()/1e3;
		pollConnections();
		for(unsigned i=0; i<units.size(); ++i) {
			Unit& u = units[i];
			if (u.type!=0) continue;
			clients[clID[u.id]]->u = &u;
		}
		for(unsigned i=0; i<clients.size(); ++i) {
			ClientInfo& c=*clients[i];
			if (c.u) continue;
			c.spawnTime -= nt-t;
			if (c.spawnTime<0) {
				units.push_back(Unit(area.getSpawn(curSpawn), 0, c.id));
				c.u = &units.back();
			}
		}
		readInputs();
		updatePhysics(nt-t);
		updateBases();
		for(unsigned i=0; i<units.size(); ++i) {
			Unit& u = units[i];
			if (u.type!=0) continue;
			clients[clID[u.id]]->u = &u;
		}
		sendState();
		t=nt;
		SDL_Delay(15);
	}
}
static int bulletid = 0;
static int itemid = 0;
void Server::updatePhysics(double t)
{
	//spawnTime -= t;
	if (spawnTime <= 0) {
		//spawnTime = 1;

        //spawn items
	}

	spawnUnits(t);
	for(unsigned i=0; i<units.size(); ++i) 
        if (units[i].type!=0) moveBot(*this,units[i],area,units,botinfos[units[i].id]);

	unitMove += t;
	const double MOVE_STEP = .025;
	while(unitMove >= MOVE_STEP) {
		moveUnits(&units[0], units.size(), area, MOVE_STEP);
		unitMove -= MOVE_STEP;
	}

//	moveBullets(&bullets[0], bullets.size(), &units[0], units.size(), area, t);
	updateBullets(t);

	for(unsigned i=0; i<items_map.vec.size(); ++i) {
        items_map.vec[i].timeLeft-=t;
    }
	for(unsigned i=0; i<items_map.vec.size(); ++i) {
        if(items_map.vec[i].timeLeft<=0)
        {
            Item j = items_map.vec[i];
            DataWriter w;
            w.writeByte(SRV_DELITEM);
            w.writeInt(j.id);
            sendToAll(w);
            items_map.remove(j.id);
            break;
        }
    }
	for(unsigned i=0; i<units.size(); ++i) {
        if(units[i].type!=0)continue;
        Unit& u = units[i];
        if(u.health>=1.0)continue;
        for(unsigned j=0;j<items_map.vec.size();j++)
        {
            Item& i = items_map.vec[j];
            if(length2(u.loc-i.loc)<1)
            {
                std::cout<<"item used "<<i.id<<"\n";
                u.health+=0.25;
                u.health = std::min(1.0,u.health);
                DataWriter w;
                w.writeByte(SRV_DELITEM);
                w.writeInt(i.id);
                sendToAll(w);
                items_map.remove(i.id);
                break;
            }
        }
    }

	for(unsigned i=0; i<units.size(); ++i) {
		Unit& u = units[i];
		u.shootTime -= t;
		if (u.shooting && u.shootTime<=0) {
			int t = u.type>0 ? u.type-1 : clients[clID[u.id]]->weapon;
			u.shootTime = loadTimes[t];

			if (t==2) { // lightning
				DataWriter w;
				w.writeByte(SRV_LIGHTNING);
				w.writeInt(0);
				Vec2 v(cos(u.d),sin(u.d));
				int cnt=1;
				w.writeInt(u.id);
				for(unsigned j=0; j<units.size(); ++j) {
					if (j==i) continue;
					Unit& p =units[j];
					Vec2 v2 = p.loc-u.loc;
					double l = length(v2);
					if (l > LIGHTNING_RAD) continue;
					double a = acos(dot(v,v2)/l);
					if (a > LIGHTNING_ANGLE) continue;

					p.health -= damages[t] / shields[p.type];
					w.writeInt(p.id);
					++cnt;
				}
				*(int*)&w.datavec[1] = cnt;
				sendToAll(w);
			} else if (t==SHOTGUN)
            {
                for(int j=0;j<10;j++){
                    Bullet b = genBullet(t, u.loc, u.d, bulletid++);
                    bullets.push_back(b);

                    DataWriter w;
                    w.writeByte(SRV_SHOOT);
                    w.write(&b, sizeof(b));
                    sendToAll(w);
                }
            }else{
				Bullet b = genBullet(t, u.loc, u.d, bulletid++);
				bullets.push_back(b);

				DataWriter w;
				w.writeByte(SRV_SHOOT);
				w.write(&b, sizeof(b));
				sendToAll(w);
			}
		}
	}


	// delayed lightning damage
	for(unsigned i=0; i<units.size(); ++i) {
		damageUnit(i, 0);
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
	int yes=1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
		perror("setsockopt fail");
		exit(2);
	}
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
void Server::sendToAll(DataWriter& w)
{
	for(unsigned i=0; i<clients.size(); ++i)
		clients[i]->conn.write(w);
}

void Server::readInputs()
{
	for(unsigned i=0; i<clients.size(); ) {
		if (clients[i]->handleMessages()) ++i;
		else {
			Unit* u = clients[i]->u;
			if (u) {
				int x = u-&units[0];
				units[x] = units.back();
				units.pop_back();
			}

			delete clients[i];
			clients[i]=clients.back();
			clients.pop_back();
			clID[clients[i]->id] = i;
		}
	}
}

void Server::updateBullets(double t)
{
	for(unsigned i=0; i<bullets.size(); ++i) {
		Bullet& b = bullets[i];
		int h;
		if (!moveBullet(b, &units[0], units.size(), area, t, &h)) {
			if (h>=0) damageUnit(h, damages[b.type]);
			if (b.type==ROCKET || b.type==GRENADE) {
				double r = b.type==ROCKET ? EXPLOSION_SIZE : GRENADE_SIZE;
				double r2 = r*r;
				for(unsigned j=0; j<units.size(); ++j) {
					Unit& u = units[j];
					Vec2 d = u.loc-b.loc;
					if (length2(d) > r2) continue;
					unsigned s=units.size();
					damageUnit(j, damages[b.type]*(1 - length(d)/r));
					if (units.size()<s) --j;
				}
			}
//			cout<<"collision @ "<<c<<' '<<b.loc<<' '<<length(c-b.loc)<<'\n';
			DataWriter w;
			w.writeByte(SRV_HIT);
			w.writeInt(b.id);
			w.writeFloat(b.loc.x);
			w.writeFloat(b.loc.y);
			sendToAll(w);
			bullets[i] = bullets.back();
			bullets.pop_back();
			--i;
		}
	}
}
void Server::updateBases()
{
	int y2=area.bases[curSpawn+1];
	int c2=0;
	for(unsigned i=0; i<units.size(); ++i) {
		Unit& u = units[i];
//		cout<<"u "<<u.type<<'\n';
		int a=u.type==0?1:-1;
		if (abs(u.loc.y-y2)<3) c2+=a;
	}
//	cout<<"zxc "<<curSpawn<<' '<<c1<<' '<<c2<<'\n';
	if (c2>0) {
		++curSpawn;
		cout<<"updating base "<<curSpawn<<'\n';
		DataWriter w;
		w.writeByte(SRV_BASE);
		w.writeInt(curSpawn);
		sendToAll(w);
	}
}
void Server::damageUnit(int i, double d)
{
	Unit& u =units[i];
	u.health -= d/shields[u.type];
	if (u.health<0) {
		if (u.type==0) clients[clID[u.id]]->u=0, clients[clID[u.id]]->spawnTime=3;
		units[i] = units.back();
		units.pop_back();
	}
}
void Server::spawnUnits(double t)
{
	spawnTime -= t;
	if (spawnTime > 0) return;
    for(int i=0;i<10;i++)
    { 
        Vec2 s = area.getSpawn(curSpawn)+Vec2(randf()-0.5,randf()-0.5);;
        Item it;
        it.id = itemid;
        it.loc = s;
        it.timeLeft = 200.0f*(5+10*randf())/items_map.vec.size();
        it.type=ITEM_HEALTH;
        it.a = 360*randf();
        DataWriter w;
        w.writeByte(SRV_ADDITEM);
        w.write((void*)&it,sizeof(Item));
        itemid++;
        sendToAll(w);
        std::cout<<"add item "<<it.id<<"\n";
        items_map.insert(it);
    }

	cout<<"spawning bots\n";
	memset(enemyCounts,0,sizeof(enemyCounts));
    for(int i=0;i<(int)units.size();i++)
    {
        enemyCounts[units[i].type]++;
    }

	for(int i=1; i<20; ++i) {
		for(int k=curSpawn+1; k<curSpawn+3; ++k) {
			int kk = min(k, (int)area.bases.size()-1);
			for(int j=0; j<spawnCounts[i][kk]; ++j) {
                if(enemyCounts[i]>=3*spawnCounts[i][kk])
                    continue;
				Unit b(area.getSpawn(kk), i, botID++);
				units.push_back(b);
                enemyCounts[i]++;
				BotInformation* bi = new BotInformation;
				botinfos.resize(botID);
				botinfos.at(botID-1) = bi;
				cout<<"spawning "<<i<<' '<<kk<<'\n';
			}
		}
	}
	spawnTime = 20;
	cout<<"spawning done\n";
}

float firstBases[32] = {0,0,.0,.2,.4,.1};
int firstCounts[32] = {0,3,2,0,1,1};
int lastCounts[32] = {0,10,5,0,10,6};
void Server::genSpawnCounts()
{
	int n = area.bases.size();
	memset(spawnCounts,0,sizeof(spawnCounts));
	for(int i=0; i<20; ++i) {
		int f = n*firstBases[i];
		for(int j=f; j<n; ++j) {
			spawnCounts[i][j] = firstCounts[i] + (lastCounts[i]-firstCounts[i])*(j-f)/(n-1-f);
		}
	}
	cout<<"genSpawnCounts done\n";
}
