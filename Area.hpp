#ifndef AREA_HPP
#define AREA_HPP
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
        for(int i=0;i<w;i++)
        {
            set(i,0,1);
            set(i,h-1,1);
        }
        for(int i=0;i<h;i++)
        {
            set(0,i,1);
            set(w-1,i,1);
        }
        for(int i=0;i<h/2*w/100.0;i++)
        {
            int x = rand()%(w);
            int y = rand()%(h-10);
            y+=10;
            int ww = rand()%10+5;
            int hh = rand()%10+5;
            addRect(x,y,ww,hh,rand()%5+1);
        }
        for(int x=0;x<w;x++)
        {
            for(int y=0;y<h;y++)
            {
                if(height(x,y)==0)
                {
                    if(x<=1||y<=1){
                        continue;
                    }else {
                        set(x-1,y-1,0);
                        set(x-0,y-1,0);
                        set(x-1,y-0,0);
                    }
                }
            }
        }
        for(int i=0;i<2;i++)
        {
            int y=1;
            int x=w/2;
            while(y<h-2)
            {
                int ox = x;
                switch(rand()%3){
                    case 0:
                        x++;
                        break;
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
		return a[w*y+x];
	}
	Vec2 getSpawn(int base) {
		int y0 = bases[base];
		int x,y;
		do {
			x = rand()%w, y=y0 + rand()%6-3;
		} while(blocked(x,y));
		return Vec2(x+.5,y+.5);
	}
};

#endif
