#ifndef AREA_HPP
#define AREA_HPP
#include<algorithm>
#include<iostream>
#include<stdlib.h>
#include<string>
#include<fstream>
#include <vector>
#include "Vector.hpp"

struct Area {
    private:
        Area(const Area& a);
        Area operator=(const Area&);
    public:

	int w,h;
	int* a;
	std::vector<int> bases;
    Area()
    {
        w=0;
        h=0;
        a=0;
    }
    void addRect(int x,int y,int w,int h,int v=1)
#define set setBlocked
    {
        for(int xx=x;xx<x+w;xx++)
        {
            for(int yy=y;yy<y+h;yy++)
            {
                set(xx,yy,v);
            }
        }
    }
#undef set
#define set(x,y,v) a[(x)+(w)*(y)]=v
    Area(int w_,int h_):w(w_),h(h_) {
        srand(time(0)+rand());
        
        a = new int[w*h];
        addRect(0,0,w,h,0);
        for(int i=0;i<h;i+=50)
        {
            bases.push_back(i);
        }
        std::cout<<"konstruktori!\n";

        const int caves = 2;
        for(int i=0;i<caves;i++)
        {
            int y=1;
            int x=(w/2)*((i+1)/float(caves));
            while(y<h-2)
            {
                int ox = x;
                switch(rand()%3){
                    case 0:
                    //case 2:
                        x++;
                        break;
                    //case 3:
                    case 1:
                        x--;
                        break;
                    default:
                        y++;
                };
                if(height(x,y)==-1)
                    x = ox;
                if(x<1)x=1;
                if(x>=w-1)x=w-2;
                set(x,y,-1);
            }
        }
        for(int b=0;b<h;b+=50)
        {

            for(int i=0;i<20;i++)
            {
                int ww = rand()%15+4;
                int x =  rand()%(w-ww);
                int y =  b+rand()%(50);
                //y+=10;
                int hh = rand()%3+4;
                for(int xx=0;xx<ww;xx++)
                {
                    for(int yy=0;yy<hh;yy++)
                    {
                        if(height(xx+x,yy+y)==-1){
                            i--;
                            goto endloop;
                        }
                    }
                }
                addRect(x,y,ww,hh,rand()%5+1);
endloop:;
            }
        }
        for(int i=0;i<(int)bases.size();i++)
        {
            addRect(0,bases[i]-5,w,10,0);
        }
        /*
        std::reverse(a,a+w*h);
        removeDeadEnds();
        std::reverse(a,a+w*h);
        removeDeadEnds();
        */
        for(int z = 0;z<0;z++)
        for(int x=0;x<w;x++)
        {
            for(int y=0;y<h;y++)
            {
                if(height(x,y)<=0)
                {
                    if(x<=1||y<=1){
                        continue;
                    }else {
                        //if(rand()%2)
                            set(x-1,y-1,0);
                        //if(rand()%2)
                            set(x-0,y-1,0);
                        //if(rand()%2)
                            set(x-1,y-0,0);
                    }
                }
            }
        }

        /*
        for(int x=0;x<w;x++)
        {
            for(int y=0;y<h;y++)
            {
                if(blocked(x,y))
                {
                    if(x<=1||y<=1){
                        continue;
                    }else {
                        //if(rand()%2)
                            set(x-1,y-1,height(x,y));
                        //if(rand()%2)
                            set(x-0,y-1,height(x,y));
                        //if(rand()%2)
                            set(x-1,y-0,height(x,y));
                    }
                }
            }
        }*/

        for(int x=0;x<w;x++)
        {
            for(int y=0;y<h;y++)
            {
                if(height(x,y)<0)
                    set(x,y,0);
            }
        }
#undef set 
        
    }
    Area(const char* fname)
    {
        std::ifstream fin(fname);
        fin>>h>>w;
        std::string buf;
        a = new int[w*h];
        int y = 0;
        while(std::getline(fin,buf))
        {
            for(int i=0;i<w;i++)
            {
                setBlocked(i,y,buf[i]!='.');
            }
            y++;
            if(y>=h)break;
        }
    }
    void setBlocked(int x,int y,int b=1) {
        if(x<0||y<0||x>=w||y>=h)return;
        a[x+y*w]=b;
    }

	bool blocked(int x, int y) const {
        if(x>=w || y>=h)return true;
        if(x<0 || y<0)return true;
		return a[w*y+x]>0;
	}
	int height(int x, int y) const {
        if(x>=w || y>=h)return 1;//+(((abs(x)^abs(y)))&7);
        if(x<0 || y<0)return 1;//+((abs(x)^abs(y))&7);
		//if (a[w*y+x]<0) return 0;
		return a[w*y+x];
	}
	Vec2 getSpawn(int base) {
		int y0 = bases.at(base);
		int x,y;
		do {
			x = rand()%w, y=y0 + rand()%6-3;
		} while(blocked(x,y));
		return Vec2(x+.5,y+.5);
	}
/*
    struct Union{
        int x;
        int y;
        Union* parent;
        int getY()
        {
            return find_root()->y;
        }
        Union* find_root()
        {
            if(parent==this)return this;
            return parent = parent->find_root();
        }
        void merge(Union* u)
        {
            Union* uroot = u->find_root();
            Union* root = this->find_root();
            if(uroot->y<root->y)
            {
                root->parent = uroot;
            }else
            {
                uroot->parent=root;
            }
        }
        Union(){
            parent = this;
            //miny = 10000000;
        }
    };

    void removeDeadEnds(){
        std::vector<std::vector<Union> > us(w,std::vector<Union> (h));
        for(int x=0;x<w;x++){
            for(int y=0;y<h;y++){
                us[x][y].x=x;
                us[x][y].y=y;
            }
        }
        for(int x=1;x<w;x++){
            for(int y=0;y<h;y++){
                if(!blocked(x,y)&&!blocked(x-1,y))
                    us[x][y].merge(&us[x-1][y]);
            }
        }
        for(int x=0;x<w;x++){
            for(int y=1;y<h;y++){
                if(!blocked(x,y)&&!blocked(x,y-1))
                    us[x][y].merge(&us[x][y-1]);
            }
        }
        for(int y=0;y<h;y++)
        {
            for(int x=0;x<w;x++)
            {
                if(us[x][y].getY()>1)
                {
                    Union* root = us[x][y].find_root();
                    int xx = root->x;
                    int yy = root->y;
                    //int best = 1000;
                    int found = 0;
                    for(int d = -1;d<=1;d+=2)
                    {
                        if(found)
                            break;
                        for(int x2=xx;x2>=0&&x2<w;x2+=d)
                        {
                            if(us[x2][yy].getY()<=1){
                                //best = abs(x2-xx);
                                found=true;
                                for(int ix=xx;ix!=x2+d;ix+=d)
                                {
                                    setBlocked(ix,yy,0);
                                    std::cout<<"removed "<<ix<<","<<yy<<"\n";
                                }
                                root->merge(&us[x2][yy]);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }*/
    std::vector<std::vector<int> > lowest;
    int rec(int x,int y)
    {
        if(blocked(x,y))return h+1;
        if(lowest[x][y]!=-1)return lowest[x][y];
        if(y<=1)return y;
        int m = y;
        for(int d=-1;d<=1;d+=2)
        {
            for(int x2=x;x2>=0&&x2<w;x2+=d)
            {
                if(blocked(x2,y-1)||blocked(x2,y))
                    break;
                m = std::min(m,rec(x2,y-1));
            }
        }
        return lowest[x][y]=m;
    }
    void removeDeadEnds(){
        lowest.resize(w,std::vector<int>(h,-1));
        for(int y=1;y<h;y++)
        {
            for(int x=0;x<w;x++)
            {
                if(blocked(x,y))continue;
                int my = rec(x,y);
                if(my>1){
                    int xx = x;
                    int yy = y;
                    while(rec(xx,yy)!=yy)
                    {
                        int left = rec(xx-1,yy);
                        int right = rec(xx+1,yy);
                        int down = rec(xx,yy+1);
                        if(left<std::min(down,right))
                            xx--;
                        else if(right<=std::min(down,left))
                            xx--;
                        else
                            yy--;
                        if(blocked(xx,yy))
                        {
                            yy++;
                            break;
                        }
                    }
                    bool found = false;
                    for(int d=-1;d<=1;d+=2)
                    {
                        if(found)
                            break;
                        for(int x2=xx;x2>=0&&x2<w;x2+=d)
                        {
                            //if(us[x2][yy].getY()<=1){
                            if(rec(x2,yy)<=1){
                                //best = abs(x2-xx);
                                found=true;
                                for(int ix=xx;ix!=x2+d*2;ix+=d)
                                {
                                    setBlocked(ix,yy,0);
                                }
                                break;
                            }
                        }
                    }
                }

            }
        }
    }
};

#endif
