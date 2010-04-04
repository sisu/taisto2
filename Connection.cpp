#include "Connection.hpp"
#include "DataWriter.hpp"
#include <sys/types.h>
#include <fcntl.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <winsock.h>
#endif
#include <unistd.h>
#include <iostream>
#include <cassert>
using namespace std;

#ifdef WIN32
int get(int fd, void* buf, int n)
{
	int r = recv(fd, (char*)buf, n, 0);
	if (r<0) {
		int rr = WSAGetLastError();
		if (rr==WSAEWOULDBLOCK) errno=EAGAIN;
		else cout<<"rect error: "<<rr<<endl;
	} else cout<<"got data "<<r<<'\n';
	return r;
}
#else
int get(int fd, void* buf, int n)
{
	return ::read(fd, buf, n);
}
#endif

bool Connection::read()
{
    assert(cur<1<<20);
	if (cur<4) {
		int n = get(fd, buf+cur, 4-cur);
		if (n<0) return 0;
//		cout<<"got data: "<<n<<'\n';
		cur += n;
		if (cur<4) return 0;
		need = *(int*)buf;
//		cout<<"need: "<<need<<'\n';
//		assert(need < 1<<10);
	}
//	cout<<"asd "<<cur<<' '<<need-(cur-4)<<'\n';
	int n =get(fd, buf+cur, need-(cur-4));
	if (n<0) return 0;
//	cout<<"real data: "<<n<<'\n';
//	for(int i=0; i<n; ++i) cout<<hex<<(int)((char*)buf)[cur+i]<<' ';cout<<'\n';
	cur += n;
	if (cur<4+need) return 0;
	cur=0;
	return 1;
}

void Connection::write(const void* s, int n)
{
#if 1
	char* np = (char*)&n;
	obuf.insert(obuf.end(), np, np+4);
	char* ss = (char*)s;
	obuf.insert(obuf.end(), ss, ss+n);
#else
	::write(fd, &n, 4);
	::write(fd, s, n);
#endif
}
void Connection::write(DataWriter& w)
{
	write(w.data(), w.len());
}
void Connection::flush()
{
	if (obuf.size()) {
		cout<<"sending "<<obuf.size()<<" bytes\n";
		#ifdef WIN32
		int r = send(fd, &obuf[0], obuf.size(), 0);
		if (r != obuf.size()) {
			cout<<"failed send "<<WSAGetLastError()<<endl;
		}
		#else
		::write(fd, &obuf[0], obuf.size());
		#endif
		obuf.clear();
	}
}
