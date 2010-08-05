#include "music.hpp"
#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include "SDL.h"
using namespace std;

const int FREQ = 44100;
const int SAMPLES = 256;

struct Envelope {
	float a,d,s,r;
};
Envelope envelopes[4] = {
	{.05,.07,.4,.1},
	{.05,.07,.4,.1},
	{.05,.05,.7,.05},
	{.02,.04,.2,.01},
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
double rndf()
{
	return rand()/(double)RAND_MAX;
}

const int NHARM=16;
double harms[4][3][NHARM];
double vibrf[4] = {.01,.01,.02,0};
double sound2(double t, int c)
{
	double r=0;
	for(int i=0; i<NHARM; ++i) {
		double tt = t * (1+i);
		double s = sin(2*M_PI * tt);
		double saw = 2*fmod(tt,1)-1;
//		double sqr = (fmod(tt,1)<.5)-.5;
		double sqr = (fmod(tt + vibrf[c]*sin(t/2),1)<.5)-.5;
//		sqr = 4*fabs(fmod(tt, 1)-.5)-1;
		r += harms[c][0][i]*s + harms[c][1][i]*saw + harms[c][2][i]*sqr;
	}
	return r;
}
int baseNote[4] = {60,72,60,72};

const double BLEN = .2;
Envelope beatEnv = {.02,.03,.4,.05};
struct BeatT {
	double bfac, nfac;
	double sfreq;
	double slowdown;

	double gen(double t) {
//		cout<<"gen beat "<<sfreq<<'\n';
		double b = bfac * sin(2*M_PI*sfreq*t / (1+t*slowdown));
		double n = nfac * (rndf()-.5);
		return (b+n) * volume(beatEnv, t, BLEN);
	}
	bool end(double t) {
		return t > BLEN;
	}
};
BeatT beats[128];
int beatStart[128];

struct C64F {
	double d1,d2,g1,g2,ampl;
	double x1,x2,y1,y2;

	void genf(double freq, double res) {
		// from e32frodo
#define CALC_RESONANCE_LP(f) (227.755\
		- 1.7635 * f\
		- 0.0176385 * f * f\
		+ 0.00333484 * f * f * f\
		- 9.05683E-6 * f * f * f * f)

#define CALC_RESONANCE_HP(f) (366.374\
		- 14.0052 * f\
		+ 0.603212 * f * f\
		- 0.000880196 * f * f * f)
		double f = CALC_RESONANCE_LP(freq);
		double a = f / (FREQ>>1);
		a = min(a, .99);
		a = max(a, .01);

		g2 = 0.55 + 1.2 * a * a - 1.2 * a + res * 0.0133333333;
		g1 = -2.0 * sqrt(g2) * cos(M_PI * a);

		// Increase resonance if LP/HP combined with BP
		//		if (f_type == FILT_LPBP || f_type == FILT_HPBP)
		g2 += 0.1;

		// Stabilize filter
		if (fabs(g1) >= g2 + 1.0) {
			if (g1 > 0.0)
				g1 = g2 + 0.99;
			else
				g1 = -(g2 + 0.99);
		}

		d1 = 2; d2 = 1;
		ampl = .25 * (1 + g1 + g2);
		cout<<"c64f init: "<<d1<<' '<<d2<<' '<<g1<<' '<<g2<<' '<<ampl<<'\n';
	}
	double apply(double x) {
		x *= ampl;
		double y = x + d1*x1 + d2*x2 - g1*y1 - g2*y2;
		x2=x1; x1=x;
		y2=y1; y1=y;
		return y;
	}
};
C64F post64;

double distord(double x)
{
	return 1-2/(1+exp(3*x));
}
ofstream ndump("m.dat");

int curPos=0;
void callback(void* udata, Uint8* stream, int len)
{
	(void)udata;
	Sint16* s = (Sint16*)stream;
	len >>= 1;

	memset(buf, 0, sizeof(buf));

	int ms = curPos * 10 / 441;
	for(int i=0; i<3; ++i) {
		const Note* ns = notes[i];
		int& c = curNote[i];
		bool init=0;
		while(c<ncounts[i] && ms >= ns[c+1].start) {
			++c;
			cout<<double(curPos)/FREQ<<' '<<curPos<<' '<<i<<' '<<ns[c].pitch<<'\n';
			init=1;
		}
		Note n = ns[c];
		double f = 440*exp2((n.pitch - baseNote[i])/12.);

		int ss = (long long)n.start * 441 / 10;
		ss = (ss+SAMPLES-1)/SAMPLES*SAMPLES;

		for(int j=0; j<len; ++j) {
			int kk = curPos + j;
			int k = kk - ss;
#if 0
			if (init && j==0) {
				if (k!=0) cout<<"k "<<k<<' '<<kk<<' '<<ss<<' '<<i<<'\n';
				assert(k==0);
			}
#endif
			double t = k / (double)FREQ;
			//			double s = wave(n.pitch-baseNote[i], 1, t, min(t,1.));
			double s;
			s=sound2(f*t, i);
			//			if (i==1) s = rand()/(double)RAND_MAX;
			//			s = wtable[int(k * f / 220)];
			buf[j] += .4 * s * volume(envelopes[i], t, n.duration/1000.);
		}
	}

	// handle beats
	{
		int& c = curNote[3];
		const Note* ns = notes[3];
		while(c<ncounts[3] && ms>=ns[c+1].start) {
			++c;
			Note n = ns[c];
			beatStart[n.pitch] = curPos;
			cout<<"starting beat "<<n.pitch<<'\n';
		}
		for(int i=0; i<128; ++i) {
			int ss = beatStart[i];
			if (curPos > ss+BLEN*FREQ) continue;
			BeatT b = beats[i];
//			cout<<"lol "<<i<<'\n';
			for(int j=0; j<len; ++j) {
				int kk = curPos + j;
				int k = kk - ss;
				double t = k / (double)FREQ;
				double s = b.gen(t);
				buf[j] += .5 * s;
			}
		}
	}

	for(int i=0; i<len; ++i) s[i] = post64.apply(s[i]);

	for(int i=0; i<len; ++i) {
		s[i] = 10000 * buf[i];
	}
	ndump.write((char*)s, 2*len);

	curPos += len;
}

void initHarms()
{
#if 0
	harms[1][1][0] = .3;
	harms[1][2][0] = .2;
#endif
#if 0
	harms[2][0][0] = .7;
	harms[2][1][0] = .2;
#endif
	harms[2][2][0] = .4;
	harms[2][2][1] = .2;
	harms[1][1][0] = .7;

	harms[0][2][0] = 1.5;
#if 0
	harms[0][2][1] = .3;
	harms[0][1][0] = .4;
	harms[0][1][1] = .2;
#endif

//	harms[2][2][1] = .3;
//	harms[2][2][2] = .1;
#if 0
	for(int i=0; i<NHARM; ++i) {
		harms[2][2][i] = 1./(1+i);
		harms[2][2][i] = max(0., 1-.2*i);
//		harms[2][2][i] = 1./(1+i*i);
//		harms[2][1][i] = 1./(1+i);
//		harms[2][1][i] = max(0., 1-.2*i);
//		harms[2][1][i] = 1./(1+i*i);
	}
#endif
}

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
void initFilt()
{
	post64.genf(220, 5);
}
void initBeats()
{
	beats[40] = (BeatT){.2, .5, .045*FREQ, .0012*FREQ};
	beats[36] = beats[40];
	beats[45] = (BeatT){.6, .2, .050*FREQ, .0012*FREQ};
	beats[57] = (BeatT){.9, .2, .055*FREQ, .0012*FREQ};
	beats[59] = (BeatT){.3, .2, .045*FREQ, .0012*FREQ};
}

int gcd(int a, int b)
{
	return b ? a : gcd(b, a%b);
}
int main(int argc, char* argv[])
{
	for(int i=0; i<4; ++i) curNote[i]=-1;
	if (argc>1) {
		double s = atof(argv[1]);
		curPos = int(FREQ * s) / SAMPLES * SAMPLES;
		int ms = 1000*curPos/FREQ;
		for(int i=0; i<4; ++i) {
			while(notes[i][curNote[i]+1].start < ms) ++curNote[i];
			cout<<"start note "<<i<<' '<<curNote[i]<<' '<<notes[i][curNote[i]].start<<'\n';
		}
	}
	//	for(int i=0; i<4; ++i) cout<<ncounts[i]<<'\n';
	for(int i=0; i<4; ++i) {
		const Note* ns = notes[i];
		int g=ns[1].start - ns[0].start;
		if (!g) g = ns[2].start - ns[0].start;
		for(int j=2; j<ncounts[i]; ++j) g = gcd(g, ns[j].start - ns[0].start);
		cout<<g<<'\n';
	}
	initHarms();
	initFilt();
	initBeats();
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
