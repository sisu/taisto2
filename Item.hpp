#ifndef ITEM_HPP
#define ITEM_HPP

#include "Vector.hpp"

const int ITEM_HEALTH=0;
const int ITEM_AMMON=1;
struct Item
{
    int type;
    Vec2 loc;
    int id;
    float a;
};



#endif
