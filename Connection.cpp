#include "Connection.hpp"
#include <unistd.h>
#include <iostream>
#include <cassert>
using namespace std;

bool Connection::read()
{
	if (cur<4) {
		int n = ::read(fd, buf+cur, 4-cur);
		if (n<0) return 0;
//		cout<<"got data: "<<n<<'\n';
		cur += n;
		if (cur<4) return 0;
		need = *(int*)buf;
//		cout<<"need: "<<need<<'\n';
//		assert(need < 1<<10);
	}
//	cout<<"asd "<<cur<<' '<<need-(cur-4)<<'\n';
	int n =::read(fd, buf+cur, need-(cur-4));
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
//	cout<<"writing len "<<n<<'\n';
//	for(int i=0; i<n; ++i) cout<<hex<<(int)((char*)s)[i]<<' '; cout<<'\n';
	::write(fd, &n, 4);
	::write(fd, s, n);
}
