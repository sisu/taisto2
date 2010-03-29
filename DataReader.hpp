#ifndef DAtAREADER_HPP
#define DATAREADER_HPP

#include <cstring>
#include "Vector.hpp"
struct DataReader {
	DataReader(void* c): cur((char*)c) { }
	char* cur;

#define F(name,type) type name() { type r = *(type*)cur; cur+=sizeof(type); return r; }
    F(readVec2,Vec2)
	F(readInt,int)
	F(readByte,char)
	F(readDouble,double)
	F(readFloat,float)
#undef F
	void read(void* out, int n) {
		memcpy(out, cur, n);
		cur += n;
	}
};

#endif
