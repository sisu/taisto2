all: play
SRC=play.cpp

play: $(SRC) music.hpp
	g++ $(SRC) -Wall -O2 `sdl-config --cflags --libs` -o $@

music.hpp: gen
	./gen > $@

gen: gen.cpp
	g++ $< -o $@ -Wall -O2
