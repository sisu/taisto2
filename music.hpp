#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <vector>

void initSoundSystem();
void initInstruments();
void initSounds();

enum SoundType {
	EXPLOSION,
    SHOTGUNSOUND,
    MACHINEGUNSOUND,
    FLAMESOUND,
	ELECTROSOUND,
	RAILSOUND,
	NSOUNDS
};
struct Sound {
	SoundType type;
	double vol;
	int pos;

	Sound(){}
	Sound(SoundType t, double v): type(t), vol(v), pos(0) {}
};

extern std::vector<Sound> sounds;

extern double distvol(double d);

const int SAMPLES = 256;


#endif
