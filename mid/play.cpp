#include "music.hpp"
#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include "SDL.h"
using namespace std;

const int FREQ = 44100;
const int SAMPLES = 256;

struct Envelope {
	float a,d,s,r;
};
Envelope envelopes[4] = {
	{.2,.2,.3,.1},
	{.05,.07,.4,.1},
	{.05,.05,.7,.05},
	{.1,.1,.4,.01},
};
float volume(const Envelope& e, float t, float len)
{
	if (t<0) return 0;
	if (t<e.a) return t/e.a;
	t -= e.a;
	if (t<e.d) return 1 + (e.s-1)*t/e.d;
	t -= e.d;
	len -= e.a + e.d;
	if (t<len) return e.s;
	t -= len;
	if (t<e.r) return e.s*(1-t/e.d);
	return 0;
}

int curNote[4];
float buf[SAMPLES];

const double inst[4][3] = {
	{.9,.4,.2},
	{0,.3,.2},
	{.7,.2,.1},
	{.3,.2,.1}
};
inline double wave(int pitch, double vol, double t, double fade) {
	const double freq = 440 * std::pow(2, pitch / 12.0);
	const double pi = M_PI;
	const double fade_time = .5;
	const double wave = std::sin(freq * t * 2 * pi) * (0.65 + 0.35 * std::sin(freq * t * 2 * pi * 2)) * std::min(1.0, pow((32 - pitch) / 60.0, 2)) * vol;
	if (fade < fade_time) return std::sin(fade / fade_time * pi / 2) * wave;
	fade = 1 - fade;
	if (fade < fade_time) return std::sin(fade / fade_time * pi / 2) * wave;
	return wave;
}
double sound(double t, int i)
{
	double s = sin(2*M_PI * t);
	double saw = 2*fmod(t,1)-1;
	double sqr = 2*(fmod(t,1)<.5)-1;
	return inst[i][0]*s + inst[i][1]*saw + inst[i][2]*sqr;
}
int baseNote[4] = {60,64,48,72};

int curPos=0;
void callback(void* udata, Uint8* stream, int len)
{
	(void)udata;
	Sint16* s = (Sint16*)stream;
	len >>= 1;

	memset(buf, 0, sizeof(buf));

	int ms = curPos * 10 / 441;
	for(int i=0; i<4; ++i) {
		if (i==3) continue;
		const Note* ns = notes[i];
		int& c = curNote[i];
		if (ms >= ns[c+1].start) {
			++c;
			cout<<ns[c].pitch<<'\n';
		}
		Note n = ns[c];
		double f = 440*exp2((n.pitch - baseNote[i])/12.);
		int ss = n.start * 441 / 10;
		ss = (ss+SAMPLES-1)/SAMPLES*SAMPLES;

		for(int j=0; j<len; ++j) {
			int kk = curPos + j;
			int k = kk - ss;
			double t = k / (double)FREQ;
//			double s = wave(n.pitch-baseNote[i], 1, t, min(t,1.));
			double s=sound(f*t, i);
//			if (i==1) s = rand()/(double)RAND_MAX;
			buf[j] += s * volume(envelopes[i], t, n.duration/1000.);
		}
	}

	for(int i=0; i<len; ++i) {
		s[i] = 10000 * buf[i];
	}

	curPos += len;
}

const int WTSIZE = 1<<16;

SDL_AudioSpec spec = {
	FREQ, // freq
	AUDIO_S16, // format
	1, // channels
	0, // silence
	SAMPLES, // samples
	0, // padding
	0, // size
	callback, // callback
	0 // userdata
};

int gcd(int a, int b)
{
	return b ? a : gcd(b, a%b);
}
int main()
{
//	for(int i=0; i<4; ++i) cout<<ncounts[i]<<'\n';
	for(int i=0; i<4; ++i) {
		const Note* ns = notes[i];
		int g=ns[1].start - ns[0].start;
		if (!g) g = ns[2].start - ns[0].start;
		for(int j=2; j<ncounts[i]; ++j) g = gcd(g, ns[j].start - ns[0].start);
		cout<<g<<'\n';
	}
//	return 0;
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER);
	SDL_OpenAudio(&spec, 0);
	SDL_PauseAudio(0);
	bool done=0;
	while(!done) {
		SDL_Delay(50);
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if (e.type==SDL_QUIT) done=1;
		}
	}
	SDL_PauseAudio(1);
	SDL_Quit();
}
