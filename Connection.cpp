#include "Connection.hpp"
#include <unistd.h>
#include <iostream>
using namespace std;

bool Connection::read()
{
	if (cur<4) {
		int n = ::read(fd, buf+cur, 4-cur);
		if (n<0) return 0;
		cout<<"got data: "<<n<<'\n';
		cur += n;
		if (cur<4) return 0;
		need = *(int*)buf;
	}
	int n =::read(fd, buf+cur, need-(cur-4));
	if (n<0) return 0;
	cur += 4;
	if (cur<4+need) return 0;
	return 1;
	cur=0;
	return 1;
}

void Connection::write(const void* s, int n)
{
	::write(fd, &n, 4);
	::write(fd, s, n);
}
