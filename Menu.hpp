#ifndef MENU_HPP
#define MENU_HPP

#include <vector>
#include <string>

enum ItemType {
	PICK,EXIT,STRING
};

struct MenuItem {
	const char* title;
	ItemType type;
	void(*func)();
	std::string* str;
};

struct Menu {
	Menu(): cur(0) {}
	void exec();
	bool handleInput();
	void draw();

	std::vector<MenuItem> items;
	int cur;
};

#endif
