#ifndef IDMAP_HPP
#define IDMAP_HPP
#include<iostream>
#include<cassert>
#include<vector>

template<typename T>
struct IDMap
{
    int curid;
    unsigned int i2i[0xffff+1];
    std::vector<T> vec;
    void insert(T t){
        t.id&=0xffff;
        vec.push_back(t);
        i2i[t.id]=vec.size()-1;
    }
    T errT;
    T& getById(int i)
    {
        unsigned int x = i2i[i&0xffff];
        if(x>=vec.size())
        {
            errT.id=-1;
            return errT;
        }
        return vec[i2i[i&0xffff]];
    }
    bool remove(int id)
    {
        id&=0xffff;
        unsigned int j = i2i[id];
        assert(j!=0xffffffff);
        if(vec[j].id!=id)
        {
            assert(false);
        }
        if(vec.size()<=j){
            std::cout<<"not found: "<<id<<"\n";
            return 0;
        }
        vec[j]=vec.back();
        i2i[vec[j].id]=j;
        vec.pop_back();
        i2i[id]=-1;
        return 1;
    }
	int size() const {
		return vec.size();
	}
	const T& operator[](int i) const {
		return vec[i];
	}
	T& operator[](int i) {
		return vec[i];
	}
};

#endif
