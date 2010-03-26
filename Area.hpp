#ifndef AREA_HPP
#define AREA_HPP

struct Area {
	int w,h;
	int* a;

	bool blocked(int x, int y) const {
		return a[w*y+x]>0;
	}
};

#endif
