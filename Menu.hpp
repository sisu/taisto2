#ifndef MENU_HPP
#define MENU_HPP

#include <vector>

struct MenuItem {
	const char* title;
	int type;
	void(*func)();
};

struct Menu {
	Menu(): cur(0) {}
	void exec();
	void handleInput();
	void draw();

	std::vector<MenuItem> items;
	int cur;
};

#endif
