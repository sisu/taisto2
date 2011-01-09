#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <vector>

void initSoundSystem();
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
	float vol;
	int pos;

	Sound(){}
	Sound(SoundType t, float v): type(t), vol(v), pos(0) {}
};

extern std::vector<Sound> sounds;

extern float distvol(float d);

const int SAMPLES = 512;


#endif
