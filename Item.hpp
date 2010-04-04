#ifndef ITEM_HPP
#define ITEM_HPP

#include "Vector.hpp"

const int ITEM_HEALTH=0;
const int ITEM_AMMON=1;
#pragma pack(push,1)
struct Item
{
    int type;
    Vec2 loc;
    int id;
    float timeLeft;
    float a;

	Item() {}
	Item(int t, Vec2 l, int i, float time, float a): type(t), loc(l), id(i), timeLeft(time), a(a) {}
};
#pragma pack(pop)



#endif
