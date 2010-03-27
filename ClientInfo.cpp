#include "ClientInfo.hpp"
#include "Server.hpp"
#include "msg.hpp"
#include "DataReader.hpp"
#include "DataWriter.hpp"
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
	DataWriter out;
	out.writeByte(SRV_INIT);
	id = server.nextID++;
	out.writeInt(id);
	conn.write(out.Buf, out.len());
	cout<<"sent init "<<out.len()<<'\n';;
}
