#ifndef AREA_HPP
#define AREA_HPP
#include<string>
#include<fstream>

struct Area {
    private:
        Area(const Area& a);
        Area operator=(const Area&);
    public:

	int w,h;
	int* a;
    Area()
    {
        w=0;
        h=0;
        a=0;
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
        a[x+y*w]=b;
    }

	bool blocked(int x, int y) const {
        if(x>=w || y>=h)return true;
        if(x<0 || y<0)return true;
		return a[w*y+x]>0;
	}
};

#endif
