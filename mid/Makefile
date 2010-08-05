all: play
SRC=play.cpp ../padsynth.cpp

play: $(SRC)
	g++ $(SRC) -Wall -O2 -std=c++0x `sdl-config --cflags --libs` -o $@
