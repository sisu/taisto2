#include "ClientInfo.hpp"
#include "Server.hpp"
#include "msg.hpp"
#include "DataReader.hpp"
#include <iostream>
using namespace std;

ClientInfo::ClientInfo(Server& s, int fd): server(s)
{
	conn.fd=fd;
}

void ClientInfo::handleMessages()
{
	while(conn.read()) {
		DataReader r(conn.buf+4);
		int type=r.readByte();
		switch(type) {
			default:
				cout<<"Warning: unknown message type "<<type<<'\n';
				break;
		}
	}
}

void ClientInfo::sendInit()
{
	char init=SRV_INIT;
	conn.write(&init, 1);
	cout<<"sent init\n";
}
