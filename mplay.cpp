#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include "SDL.h"
#include "mplay.hpp"
#include "padsynth.hpp"
#include "music.hpp"
using namespace std;

namespace {
#include "biisi.hpp"

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

const int NHARM=2;
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


double distord(double x)
{
	return 1-2/(1+exp(3*x));
}

int curPos=0;

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

void initBeats()
{
	beats[40] = (BeatT){.2, .5, .045*FREQ, .0012*FREQ};
	beats[36] = beats[40];
	beats[45] = (BeatT){.6, .2, .050*FREQ, .0012*FREQ};
	beats[57] = (BeatT){.9, .2, .055*FREQ, .0012*FREQ};
	beats[59] = (BeatT){.3, .2, .045*FREQ, .0012*FREQ};
}
}

void resetMusic()
{
	curPos = 0;
	for(int i=0; i<128; ++i) beatStart[i]=-1;
}
void initMusic()
{
	initHarms();
	initBeats();
	resetMusic();
}
void writeMusic(float* buf, int len)
{
	int ms = curPos * 10 / 441;
	int SAMPLES = len;
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
			if (ss<0 || curPos > ss+BLEN*FREQ) continue;
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

	curPos += len;
}
