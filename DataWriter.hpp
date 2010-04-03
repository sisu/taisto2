#ifndef DATAWRITER_HPP
#define DATAWRITER_HPP
#include<cassert>

#include "Vector.hpp"
#include <vector>
#include <cstring>
//#include <iostream>

extern char tmpBuf[];

struct DataWriter {
    std::vector<char> datavec;

	int len() const {
        return datavec.size();
	}
    char* data(){
        return &datavec[0];
    }

//#define F(name,type) void name(type x) {char buf[100]; *(type*)buf = x;datavec.insert(datavec.end(),buf,buf+sizeof(type)); }
#define F(name,type) void name(type x) {write((void*)(&x),sizeof(type));}
	F(writeInt,int)
	F(writeByte,char)
	F(writeDouble,double)
	F(writeFloat,float)
    F(writeVec2,Vec2)
#undef F
	void write(const void* s, int n) {
//		std::cout<<"writing "<<n<<' '<<len()<<'\n';
        const char* cs = (char*)s;
        datavec.insert(datavec.end(),cs,cs+n);
	}

	DataWriter(){
    }
    ~DataWriter()
    {
    }
};

#endif
