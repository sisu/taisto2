#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <vector>

struct DataWriter;
struct Connection {
	Connection(): buf(new char[1<<20]), cur(0), need(0) {}
	char* buf;
	int cur;
	int need;
	int fd;
	std::vector<char> obuf;

	bool read();
	void write(const void* s, int n);
	void write(DataWriter& w);
	void flush();

private:
	Connection(const Connection&);
};

#endif
