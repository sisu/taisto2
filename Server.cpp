#include "Server.hpp"
#include "physics.hpp"
#include "DataWriter.hpp"
#include "msg.hpp"
#include "Area.hpp"
#include "Bot.hpp"
#include "timef.h"
#include <sys/types.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#else
#include <winsock.h>
#endif
#include <fcntl.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "SDL.h"

using namespace std;
//static const double buffer[1000];
static const double shields[]={20,4,3,0.4,4,4,4,4,4,4};
const int packSizes[] = {0,15,100,50,30,5,10};

Server::Server(): end(0),nextID(1),area(30,900)// area("field.in.1")
{
	//for(int i=2; i<area.h; i+=30) area.bases.push_back(i);
	clID = new int[1<<16];
    unitID.resize(1<<16);
	spawnTime = 0;
	curSpawn=0;
	botID=256;
	initSocket();
	genSpawnCounts();
	unitMove = 0;
	defenceTime=0;
	win=0;
	flowSpawnTime=0;
}
Server::~Server()
{
//	cout<<"destructing server\n";
	for(unsigned i=0; i<clients.size(); ++i)
        delete clients[i];
#ifndef WIN32
	shutdown(sockfd, SHUT_RDWR);
#endif
	close(sockfd);
}

static double total_time=0;
void Server::loop()
{
//	cout<<"starting server loop "<<timef()<<'\n';;
	double t=timef();
    for(int i=0;i<30;i++){
        //spawnUnits(1);
        updatePhysics(1);
    }
//	cout<<"really starting server loop "<<timef()<<'\n';
	while(!end) {
		double nt = SDL_GetTicks()/1e3;
		if (nt <= t) {
			SDL_Delay(10);
			continue;
		}
		for(unsigned i=0; i<units.size(); ++i) units[i].id &= 0xffff;
        //if(nt>15)end=1;
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
				memset(c.bcnt,0,sizeof(c.bcnt));
				c.sendBCounts();
				spawnClient(c);
			}
		}
		readInputs();
		updatePhysics(nt-t);

		if (curSpawn==area.bases.size()-2 && defenceTime<=0) {
			// voitto
			win=1;
			for(unsigned i=0; i<units.size(); ) {
				if (units[i].type==0) ++i;
				else units[i]=units.back(),units.pop_back();
			}
		}

		updateBases();
		for(unsigned i=0; i<units.size(); ++i) {
			Unit& u = units[i];
			if (u.type!=0) continue;
			clients[clID[u.id]]->u = &u;
		}
		sendState();
        sendStats();
		t=nt;
		for(unsigned i=0; i<clients.size(); ++i) {
			clients[i]->conn.flush();
		}
		SDL_Delay(15);
	}
}

void find_affected(const vector<vector<int> >& graph,
		vector<bool>& used, vector<int>& hits, int ind) {

	used[ind] = true;
	hits.push_back(ind);
	for(int i = 0;i<graph[ind].size(); ++i) {
		if(!used[graph[ind][i]])
			find_affected(graph,used,hits,graph[ind][i]);
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
		moveUnits(&units[0], units.size(), area, MOVE_STEP, pc);
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
//        if(u.health>=1.0)continue;
        for(unsigned j=0;j<items_map.vec.size();j++)
        {
            Item& i = items_map.vec[j];
			if (i.type==0 && u.health>=1) continue;
            if(length2(u.loc-i.loc)<1)
            {
//                std::cout<<"item used "<<i.id<<' '<<i.type<<"\n";
				if (i.type==0) u.health = min(1.0,u.health+.25);
				else clients[clID[u.id]]->bcnt[i.type] += packSizes[i.type], clients[clID[u.id]]->sendBCounts();
                DataWriter w;
                w.writeByte(SRV_DELITEM);
                w.writeInt(i.id);
                sendToAll(w);
                items_map.remove(i.id);
            }
        }
    }

	for(unsigned i=0; i<units.size(); ++i) {
		Unit& u = units[i];
		u.shootTime -= t;
		if (u.shooting && u.shootTime<=0) {
			int t = u.type>0 ? u.type-1 : clients[clID[u.id]]->weapon;
			if (u.type==0) {
				ClientInfo& c = *clients[clID[u.id]];
				if (c.weapon) {
					if (!c.bcnt[t]) continue;
					c.bcnt[t]--;
					DataWriter w;
					c.sendBCounts();
				}
			}
			u.shootTime = loadTimes[t];

			if (t==LIGHTNING) { // lightning
				DataWriter w;
				w.writeByte(SRV_LIGHTNING);
				w.writeInt(0);
				Vec2 v(cos(u.d),sin(u.d));
				int cnt=1;
				w.writeInt(u.id);

				vector<int> aff;
				for(unsigned j=0; j<units.size(); ++j) {
					if (j==i) continue;
					Unit& p =units[j];
					Vec2 v2 = p.loc-u.loc;
					double l = length(v2);
					if (l > LIGHTNING_RAD) continue;
					double a = acos(dot(v,v2)/l);
					if (a > LIGHTNING_ANGLE) continue;

					aff.push_back(j);

					/*
					p.health -= damages[t] / shields[p.type];
					w.writeInt(p.id);
					++cnt;
					*/
				}

				aff.push_back(i); // add the source

				// generate graph

				vector<vector<int> > graph(units.size());

				for(int a = 0; a < aff.size(); ++a) {
					for(int b = a + 1; b < aff.size(); ++b) {
						bool ok = false;
                        int ia = aff[a];
                        int ib = aff[b];
						wallHitPoint(units[ia].loc, units[ib].loc, area, &ok);

						if(!ok) {
							graph[ia].push_back(ib);
							graph[ib].push_back(ia);
						}
					}
				}

				vector<bool> used(units.size());
				vector<int> hits;

				find_affected(graph,used,hits,i);

				for(int a = 0; a < hits.size(); ++a) {
					if(hits[a] != i) {
						Unit& p =units[hits[a]];
                        Bullet b = genBullet(LIGHTNING,Vec2(0,0),randf()*3.14,-1);
                        damageUnit(hits[a],damages[t],u.id,b);
						//p.health -= damages[t];
						w.writeInt(p.id);
						++cnt;
					}
				}

				*(int*)&w.datavec[1] = cnt;
				sendToAll(w);
			} else if (t==SHOTGUN)
            {
                int sn = 5;
                for(int j=0;j<sn;j++){
                    Bullet b = genBullet(t, u.loc, u.d, bulletid++);
                    b.shooter = u.id;
                    double d = u.d;
                    d+=(sn/2-j)*0.1/(sn/5.0);
                    Vec2 v(cos(d),sin(d));
                    v*=30;
                    b.v=v;
                    bullets.push_back(b);

                    DataWriter w;
                    w.writeByte(SRV_SHOOT);
                    w.write(&b, sizeof(b));
                    sendToAll(w);
                }
            }else{
				Bullet b = genBullet(t, u.loc, u.d, bulletid++);
                b.shooter = u.id;
				bullets.push_back(b);

				DataWriter w;
				w.writeByte(SRV_SHOOT);
				w.write(&b, sizeof(b));
				sendToAll(w);
			}
		}
	}

	defenceTime -= t;

	// delayed lightning damage
	for(unsigned i=0; i<units.size(); ++i) {
		//damageUnit(i, 0,2000);
        if(units[i].health<=0){
            units[i] = units.back();
            units.pop_back();
            if(units[i].type ==0)
            {
                Unit u = units[i];
                clients[clID[u.id]]->u=0, clients[clID[u.id]]->spawnTime=std::min(clients[clID[u.id]]->spawnTime,3.0);
            }
            i--;
        }
	}

//	if (units.size()>1) {Unit& u = units[1]; cout<<"updated physics; "<<u.movex<<' '<<u.movey<<" ; "<<u.loc<<" ; "<<u.shooting<<' '<<u.id<<' '<<clID[u.id]<<'\n';}
}

void Server::initSocket()
{
	struct sockaddr_in serv_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

//  bzero((char *) &serv_addr, sizeof(serv_addr));
    memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(SERVER_PORT);
#ifndef WIN32
	int yes=1;
#else
    char yes=1;
#endif
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
		perror("setsockopt fail");
		exit(3);
	}
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on binding");
		exit(1);
	}
	if (listen(sockfd,5)) {
		perror("Error on calling listen");
		exit(5);
	}
#ifndef WIN32
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
#else
    unsigned long x=1;
    ioctlsocket(sockfd, FIONBIO, &x);
    char flag=1;
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag))<0) {
    	perror("setsockopt TCP_NODELAY");
    	exit(17);
    }
#endif
}

void Server::pollConnections()
{
	while(true) {
		sockaddr_in cli_addr;
		int clilen=sizeof cli_addr;
#ifndef WIN32
		int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
#else
		int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
#endif
		if (newsockfd < 0) break;
//		cout<<"Newsockfd: "<<newsockfd<<endl;
//		cout<<"Adding socket"<<endl;

		ClientInfo* cl = new ClientInfo(*this, newsockfd);
		clients.push_back(cl);
		cl->sendInit();
		clID[cl->id] = clients.size()-1;
		spawnClient(*cl);
//		sockets[sockets_used] = newsockfd;
//		++sockets_used;

#ifndef WIN32
		fcntl(newsockfd, F_SETFL, O_NONBLOCK);
#else
    unsigned long x=1;
    ioctlsocket(sockfd, FIONBIO, &x);
#endif

//		const char* message = "Connection established";
//		n = write(newsockfd,message,strlen(message));
	}
}
void Server::spawnClient(ClientInfo& c)
{
	bool fail=0;
	for(unsigned i=0; i<units.size(); ++i) {
		Unit& u = units[i];
		if (u.type!=0 && abs(area.bases[curSpawn]-u.loc.y)<BASE_SIZE) fail=1;
	}
	int b = max(0, curSpawn-fail);
	units.push_back(Unit(area.getSpawn(b), 0, c.id));
	c.u = &units.back();
	unitID[c.id] = units.size()-1;
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

void Server::sendToAll(const void* s,int n)
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
			cout<<"dropping client "<<i<<'\n';
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
void Server::sendStats(){
    DataWriter w;
    w.writeByte(SRV_STATS);
	if (kills.size()<nextID+2) {
		kills.resize(nextID+2);
		teamkills.resize(nextID+2);
		deaths.resize(nextID+2);
	}
    int n = clients.size();//kills.size();
    w.writeInt(n);
    for(int i=0;i<n;i++)
    {
        int id = clients[i]->id;
        w.writeInt(id);
        w.writeInt(kills[id]);
        w.writeInt(teamkills[id]);
        w.writeInt(deaths[id]);
    }
    sendToAll(w);
}

void Server::updateBullets(double t)
{
	for(unsigned i=0; i<bullets.size(); ++i) {
		Bullet& b = bullets[i];
		int h;
		if(b.type != RAILGUN) {
			if (!moveBullet(b, &units[0], units.size(), area, t, &h, pc)) {
				if (b.type==ROCKET || b.type==GRENADE) {
					b.loc -= normalize(b.v) * .05;
					double r = b.type==ROCKET ? EXPLOSION_SIZE : GRENADE_SIZE;
					double r2 = r*r;
					for(unsigned j=0; j<units.size(); ++j) {
						Unit& u = units[j];
						Vec2 d = u.loc-b.loc;
						if (length2(d) > r2) continue;
						bool fail;
						wallHitPoint(b.loc, u.loc, area, &fail);
						if (fail) continue;
						unsigned s=units.size();
						damageUnit(j, damages[b.type]*(1 - length(d)/r),b.shooter,b);
						if (units.size()<s) --j;
					}
				} else if (h>=0) damageUnit(h, damages[b.type],b.shooter,b);
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
		} else {
			bool ok;
			vector<int> hits = moveRail(b, &units[0], units.size(), area, t, &ok);
            for(unsigned j = 0; j < hits.size(); ++j) {
                //units[hits[j]].health = -1;
                damageUnit(hits[j],10,b.shooter,b);
            }

			if(ok) {
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
	for(unsigned i=0; i<units.size(); ++i) {
		//damageUnit(i, 0,2000);
        if(units[i].health<=0 || isnan(units[i].health)){
            if(isnan(units[i].health))
            {
//                std::cout<<"unit "<<units[i].id<<" health is nan\n";
                Unit u = units[i];
                clients[clID[u.id]]->u=0, clients[clID[u.id]]->spawnTime=3;
            }
            if(units[i].type == 0)
            {
                Unit u = units[i];
                clients[clID[u.id]]->u=0, clients[clID[u.id]]->spawnTime=std::min(clients[clID[u.id]]->spawnTime,3.0);
            }
            units[i] = units.back();
            units.pop_back();
            i--;
        }
	}
}
void Server::updateBases()
{
	int y1=area.bases[curSpawn],y2=area.bases[curSpawn+1];
	int c2=0,c1=0;
	int curPl=0;
	for(unsigned i=0; i<units.size(); ++i) {
		Unit& u = units[i];
//		cout<<"u "<<u.type<<'\n';
		int a=u.type==0?1:-1;
		if (abs(u.loc.y-y2)<BASE_SIZE) c2+=a;
		if (abs(u.loc.y-y1)<BASE_SIZE) c1+=a;
		if (u.type==0 && u.loc.y >= y1) ++curPl;
	}
//	cout<<"zxc "<<curSpawn<<' '<<c1<<' '<<c2<<'\n';
	int sp=curSpawn;
	if (c2>0) {
		++curSpawn;
//		cout<<"updating base "<<curSpawn<<'\n';
	}
	else if (curSpawn && c1<-2 && curPl==0) {
		--curSpawn;
	}
	if (curSpawn!=sp) {
		DataWriter w;
		w.writeByte(SRV_BASE);
		w.writeInt(curSpawn);
		sendToAll(w);
        spawnTime = 0;

		if (curSpawn == (int)area.bases.size()-2) {
			defenceTime = DEFENCE_TIME;
		}
	}
}
static const double ultimate_foo = -32;
void Server::damageUnit(int i, double d,int shooter,Bullet b)
{
	Unit& u =units[i];
	double lol = 1;

	if(clients.size() == 1) {
		lol = 0.4;
	} else if(clients.size() == 2) {
		lol = 0.75;
	}
	if (u.type==0) lol=1;

	kills.resize(256);
	deaths.resize(256);
	teamkills.resize(256);

	u.health -= d/(lol * shields[u.type]);
    assert(!isnan(u.health));
    if(u.health < ultimate_foo+2)
        return;
	if (u.health<0) {
        u.health = ultimate_foo;
        DataWriter w;
        u.lastShotAngle = atan2(b.v.y,b.v.x);
        if(b.type==GRENADE || b.type==ROCKET)
        {
            u.lastShotAngle = atan2(u.loc.y-b.loc.y,u.loc.x-b.loc.x);
        }
        w.writeByte(SRV_DEAD);
        w.writeInt(1);// one unit. hieman outo
        w.write(&u,sizeof(Unit));
        sendToAll(w);
        if(u.type>=1)
        {
            if(shooter < 256 && shooter>=0)
                kills[shooter]++;
        }
		if (u.type==0){
            clients[clID[u.id]]->u=0, clients[clID[u.id]]->spawnTime=3;
            //if(clients[clID[u.id]]->
            if(shooter < 256 && shooter>=0)
                teamkills[shooter]++;
            deaths[u.id]++;
        }else if (randf() < .25) {
			float t = 200. * (50+10*randf())/(1+items_map.vec.size());
			Item it(u.type-1, u.loc, itemid++, t, u.d);
			items_map.insert(it);

			DataWriter w;
			w.writeByte(SRV_ADDITEM);
			w.write(&it,sizeof(it));
			sendToAll(w);
		}
		//units[i] = units.back();
		//units.pop_back();
	}
}
void Server::spawnUnits(double t)
{
	double f = max(1.0,sqrt(clients.size()));
	f *= 2;
    flowSpawnTime-=t;
	memset(enemyCounts,0,sizeof(enemyCounts));
    for(int i=0;i<(int)units.size();i++)
    {
        enemyCounts[units[i].type]++;
    }
	botinfos.resize(1<<16);
    if(!win && flowSpawnTime<0){
        for(int i=0;i<curSpawn+1;i++)
        for(int k=curSpawn-1; k<curSpawn+3; ++k) {
			if (k!=curSpawn+2 && (curSpawn<(int)area.bases.size()-2 || k!=curSpawn-1)) continue;
            int kk = min(k, (int)area.bases.size()-1);
            int tot = 0;
            for(int i=0;i<20;i++)
            {
                tot+=f*spawnCounts[i][kk];
            }
            int r = rand()%tot;
            int t=0;
            int s=0;
            for(t=0;s<r && t<8;t++)s+=f*spawnCounts[t][kk];
            t--;
			if (t<0) continue;
            if(enemyCounts[t]>=f*3*spawnCounts[t][kk])
                continue;
            Unit b(area.getSpawn(kk), t, botID++);
			if (botID==1<<16) botID=256;
            units.push_back(b);
			delete botinfos[botID-1];
            botinfos[botID-1] = new BotInformation;
        }
        flowSpawnTime=1;
        //std::cout<<units.size()<<" units\n";
        //std::cout<<items_map.vec.size()<<" items\n";
    }
	spawnTime -= t;
	if (spawnTime > 0) return;
	spawnTime = 20;

	vector<Item> added;
	for(int i=0; i<20; ++i) {
		for(int j=0; j<f*itemSpawns[i][curSpawn]; ++j) {
			Vec2 s = area.getSpawn(curSpawn) + Vec2(randf()-.5,randf()-.5);
			float t = 200. * (50+10*randf())/items_map.vec.size();
			Item it(i, s, itemid++, t, 360*randf());
			items_map.insert(it);
			added.push_back(it);
		}
	}
	DataWriter w;
	w.writeByte(SRV_ITEMS);
	w.writeInt(added.size());
	w.write(&added[0], added.size()*sizeof(Item));
	sendToAll(w);

//	cout<<"spawning done\n";

	{
		DataWriter w;
		w.writeByte(SRV_SPAWNTIME);
		w.writeFloat(spawnTime);
		sendToAll(w);
	}
}

int firstBases[32] = {0,1,2,3,0,4,15,16};
int firstCounts[32] = {0,3,1,1,0,2,1,0};
int lastCounts[32] = {0,7,7,4,0,3,1,0};
int fItemBases[32] = {0,1,2,4,3,5,6};
int fItemCounts[32] = {5,4,6,4,3,2,4};
int lItemCounts[32] = {8,10,12,8,6,6,8};
void Server::genSpawnCounts()
{
	int n = area.bases.size();
	memset(spawnCounts,0,sizeof(spawnCounts));
	for(int i=0; i<20; ++i) {
		int f = firstBases[i];
		for(int j=f; j<n; ++j) {
			spawnCounts[i][j] = firstCounts[i] + (lastCounts[i]-firstCounts[i])*(j-f)/(n-1-f);
		}
	}
	memset(itemSpawns,0,sizeof(itemSpawns));
	for(int i=0; i<20; ++i) {
		int f = fItemBases[i];
		for(int j=f; j<n; ++j) {
			itemSpawns[i][j] = fItemCounts[i] + (lItemCounts[i]-fItemCounts[i])*(j-f)/(n-1-f);
		}
	}
//	cout<<"genSpawnCounts done\n";
}
