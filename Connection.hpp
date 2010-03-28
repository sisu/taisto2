#ifndef CONNECTION_HPP
#define CONNECTION_HPP

struct DataWriter;
struct Connection {
	Connection(): buf(new char[1<<22]), cur(0), need(0) {}
	char* buf;
	int cur;
	int need;
	int fd;

	bool read();
	void write(const void* s, int n);
	void write(DataWriter w);

private:
	Connection(const Connection&);
};

#endif
