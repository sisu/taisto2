#ifndef MENU_HPP
#define MENU_HPP

#include <vector>

struct MenuItem {
	int type;
	void(*func)();
};

struct Menu {
	std::vector<MenuItem> items;

	void exec();
};

#endif
