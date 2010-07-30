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
double sound(double t, int i)
{
	double s = sin(2*M_PI * t);
	double saw = 2*fmod(t,1)-1;
	double sqr = 2*(fmod(t,1)<.5)-1;
	return inst[i][0]*s + inst[i][1]*saw + inst[i][2]*sqr;
}
int baseNote[4] = {60,72,48,72};

const int CSMAX = 1<<18;
double csbufs[4][CSMAX];
int cssize[4];
int cscur[4];

double rndf()
{
	return rand()/(double)RAND_MAX;
}
double sinc(double x)
{
    return x == 0.0 ? 1.0 : sin(x) / x;
}
void makeLowpass(int n, double f, double *coeffs)
{
    for (int i = 0; i <= n; ++i)
    {
        coeffs[i] = 2 * f * sinc(M_PI * f * (2 * i - n));
    }
    return;
}
const int CSCNUM = 16;
double cscoeffs[4][CSCNUM];

void initCS(int c, double f)
{
	int smp = FREQ / f;
	cssize[c] = smp;
	for(int i=0; i<smp; ++i) {
		csbufs[c][i] = 2*rndf()-1;
//		csbufs[c][i] = ;
	}
	cscur[c] = 0;
	makeLowpass(CSCNUM, 1, cscoeffs[c]);
}
double distord(double x)
{
	return 1-2/(1+exp(3*x));
}
double cstmp[CSMAX];
void filterCS(int c)
{
#if 1
	double a=0;
	double z = .2;
	for(int i=0; i<cssize[c]; ++i) {
		double b = a;
		a = csbufs[c][i];
		csbufs[c][i] = (1-z)*a + z*b;
//		csbufs[c][i] = distord(csbufs[c][i]);
	}
#else
	memcpy(cstmp, csbufs[c], c * cssize[c]);
	for(int i=0; i<cssize[c]; ++i) {
		int kl = min(i+1, CSCNUM/2)-1;
		int kh = min(cssize[c]-1, CSCNUM/2)-1;
		double r=0;
		for(int j=-kl; j<=kh; ++j) {
			r += cstmp[i+j] * cscoeffs[c][CSCNUM/2+j];
//			cout<<cscoeffs[c][CSCNUM/2+j]<<'\n';
		}
		r = cstmp[i];
//		cout<<'\n';
		csbufs[c][i] = r;
	}
#endif
}

int curPos=0;
void callback(void* udata, Uint8* stream, int len)
{
	(void)udata;
	Sint16* s = (Sint16*)stream;
	len >>= 1;

	memset(buf, 0, sizeof(buf));

	int ms = curPos * 10 / 441;
	for(int i=0; i<4; ++i) {
		if (i!=1) continue;
		const Note* ns = notes[i];
		int& c = curNote[i];
		bool init=0;
		if (ms >= ns[c+1].start) {
			++c;
			cout<<ns[c].pitch<<'\n';
			init=1;
		}
		Note n = ns[c];
		double f = 440*exp2((n.pitch - baseNote[i])/12.);

		if (init) initCS(i, f);

		int ss = n.start * 441 / 10;
		ss = (ss+SAMPLES-1)/SAMPLES*SAMPLES;

		for(int j=0; j<len; ++j) {
			int kk = curPos + j;
			int k = kk - ss;
			double t = k / (double)FREQ;
#if 0
//			double s = wave(n.pitch-baseNote[i], 1, t, min(t,1.));
			double s=sound(f*t, i);
#else
			double s = csbufs[i][cscur[i]++];
			if (cscur[i]>=cssize[i]) {
				cscur[i]=0;
				filterCS(i);
			}
#endif
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
