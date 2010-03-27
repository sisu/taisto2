#ifndef DAtAWRITER_HPP
#define DATAWRITER_HPP

#include <vector>
#include <cstring>
//#include <iostream>

extern char tmpBuf[];

struct DataWriter {
	DataWriter(): cur(Buf) {}
	char* cur;

	int len() const {
		return cur-Buf;
	}

#define F(name,type) void name(type x) { *(type*)cur = x; cur+=sizeof(type); }
	F(writeInt,int)
	F(writeByte,char)
	F(writeDouble,double)
	F(writeFloat,float)
#undef F
	void write(const void* s, int n) {
//		std::cout<<"writing "<<n<<' '<<len()<<'\n';
		memcpy(cur,s,n);
		cur+=n;
	}
	static char Buf[];
};

#endif
