#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include "SDL.h"
#include "music.hpp"
#include "timef.h"
using namespace std;


vector<Sound> sounds;
bool playMusic=0, playSounds=1;
namespace {

static int ilog2(int n)
{
	int i;
	for(i=0; n>>i; ++i);
	return i;
}

//unsigned short bitrev[1<<16];
static unsigned revbits(unsigned k)
{
//	return (bitrev[k&0xffff]<<16)|bitrev[k>>16];
	int r=0;
	for(int i=0; i<32; ++i) r|=((k>>i)&1)<<(31-i);
	return r;
}

#define COMPLEX_MULT(r, i, a, b, c, d) r=(a)*(c)-(b)*(d); i=(a)*(d)+(b)*(c)
static void calc_fft(float* rdst, float* idst, const float* rsrc, const float* isrc, int n, float z0)
{
	int rb = 33-ilog2(n);

	// bit reverse copy stuff
	for(int i=0; i<n; ++i) {
		int r = revbits(i)>>rb;
//		printf("%d -> %d ; %f %f\n", i, r, rsrc[i], isrc[i]);
		rdst[r] = rsrc[i];
		idst[r] = isrc[i];
	}
//	printf("wqe: %f\n", rdst[0]);

	for(int m=2; m<=n; m<<=1) {
		float z = z0/m;
		float wmr = cos(z), wmi = sin(z);
		int m2=m/2;
		for(int k=0; k<n; k+=m) {
			float wr=1, wi=0;
			for(int j=k; j<k+m2; ++j) {
				float ur = rdst[j], ui = idst[j];
				int h = j + m2;
				float tr,ti;
				COMPLEX_MULT(tr,ti,wr,wi,rdst[h],idst[h]);

//				printf("jee %f %f (%d %d)\n", ur, ui, j, h);

				rdst[j] = ur+tr;
				rdst[h] = ur-tr;
				idst[j] = ui+ti;
				idst[h] = ui-ti;

				float tmpr=wr, tmpi=wi;
				COMPLEX_MULT(wr,wi,tmpr,tmpi,wmr,wmi);
			}
		}
	}
}
#undef COMPLEX_MULT

void fft(float* rdst, float* idst, const float* rsrc, const float* isrc, int n)
{
	calc_fft(rdst,idst,rsrc,isrc,n,2*M_PI);
}
void ifft(float* rdst, float* idst, float* rsrc, float* isrc, int n)
{
	for(int i=0; i<n/2; ++i) rsrc[n-1-i]=rsrc[i], isrc[n-1-i]=-isrc[i];
	calc_fft(rdst,idst,rsrc,isrc,n,-2*M_PI);
	float r = 1.f/n;
	for(int i=0; i<n; ++i) {
		rdst[i] *= r;
		idst[i] *= r;
	}
}
void normalize(float* a, int n)
{
	float b=0;
#if 1
	for(int i=0; i<n; ++i) b=max(b,fabs(a[i]));
#else
	for(int i=0; i<n; ++i) b+=a[i]*a[i]; b = sqrt(b);
#endif
	for(int i=0; i<n; ++i) a[i]/=b;
}
void normalize2(float* a, int n)
{
	float s=0;
	for(int i=0; i<n; ++i) s+=fabs(a[i]);
	s /= n;
	for(int i=0; i<n; ++i) a[i] /= 2*s;
}

const int WTS=1<<16;
const int FREQ = 44100;

float sintable[WTS], costable[WTS], nulltable[WTS];

void genWT(float* out, const float* amps, const float* freqs, int n, float fundfreq, float fundbw, float bwscale)
{
    memset(sintable,0,sizeof(sintable));
    memset(costable,0,sizeof(sintable));
	float bw0 = fundfreq*(exp2(fundbw/1200)-1);
	for(int i=0; i<n; ++i) {
		float bwz = bw0*pow(freqs[i], bwscale);
		float bwi = .5*bwz/FREQ;
		float fi = fundfreq*freqs[i]/FREQ;
		for(int j=0; j<WTS/2; ++j) {
			float fx = (float)j/WTS-fi;
			float x = fx/bwi;
			sintable[j] += amps[i]*exp(-x*x)/bwi;
		}
	}
	for(int i=0; i<WTS/2; ++i) {
		float p = 2*M_PI*randf();
		float a = sintable[i];
		sintable[i] = a*sin(p);
		costable[i] = a*cos(p);
	}

	ifft(out, nulltable, costable, sintable, WTS);

	normalize(out,WTS);
}


const int SAMPLES = 4096;
const int NWT=3;
float wtables[NWT][WTS];

const int BPM = 140;
// samples per beat
const int SPB = 60*FREQ / BPM;
int curS=0;

const int NLEN = 8;
const short OFF=(1<<15)-1;
short notes[NWT+1][NLEN] = {
	{1,OFF,0,OFF,2,3,OFF,OFF},
	{OFF,1,2,3,OFF,OFF,0,5},
	{0,OFF,0,OFF,0,OFF,0,OFF},
	{OFF,0,OFF,0,OFF,0,OFF,0}
};
struct Envelope {
	float a,d,s,r;
	float z;
};
Envelope envelopes[NWT+1] = {
	{.2,.2,.3,.1,.3},
	{.05,.07,.4,.1,.5},
	{.05,.05,.1,.01,.003},
	{.05,.1,.7,.05,.3}
};
float volume(const Envelope& e, float t)
{
	if (t<e.a) return t/e.a;
	t -= e.a;
	if (t<e.d) return 1 + (e.s-1)*t/e.d;
	t -= e.d;
	if (t<e.z) return e.s;
	t -= e.z;
	if (t<e.r) return e.s*(1-t/e.d);
	return 0;
}
const float BEAT_FREQ = 1500;
const float BEAT_SLOW = 200;

void genMusic(float* buf, int l)
{
	for(int k=0; k<2; ++k) {
		int cur0 = (curS-k*SPB)/SPB;
		int cur = (cur0+NLEN) % NLEN;
		int start= cur0*SPB;

#if 1
		for(int i=0; i<NWT; ++i) {
			if (i==2) continue;
			if (i==1) continue;
			int n = notes[i][cur];
			if (n==OFF) continue;
			float f = exp2(n/12.);
			for(int j=0; j<l; ++j) {
				int k = curS+j;
				float t = (k - start)/(float)FREQ;
				k *= f;
				buf[j] += wtables[i][k%WTS] * volume(envelopes[i], t);
			}
		}
#else
		if (notes[3][cur]==OFF) continue;
		for(int i=0; i<l; ++i) {
			int k = curS+i;
			float t = (k-start)/(float)FREQ;
			buf[i] += sin(BEAT_FREQ*t/(1+t*BEAT_SLOW)) * volume(envelopes[3], t);
		}
#endif
	}
}

void lowpass(float* a, int n, double a0, double a1)
{
	float p=0,pp=0;
	for(int i=0; i<n; ++i) {
		float q=a[i];
		a[i] = a0*a[i] + a1*p + a0*pp;
		pp=p;
		p=q;
	}
}

float stables[NSOUNDS][WTS];
double stimes[] = {.5,.5,.3,.5,.5};

void genWallHitSound()
{
	float* out = stables[EXPLOSION];
	int s = FREQ * stimes[EXPLOSION] + 1;
	memset(sintable,0,sizeof(sintable));
	memset(costable,0,sizeof(costable));
	memset(out,0,4*WTS);
	for(int i=0; i<s/2; ++i) {
		double z = (double)i/s;
//		double f = max(0.0, 1 - 10*z*z);
		double f = 1/(z*z);
		sintable[i]=1;
		costable[i]=1;
	}
	ifft(out, nulltable, sintable, costable, s);
//	normalize(out, s);

//	for(int i=0; i<5; ++i) lowpass(out, s, .2, .5);
	normalize2(out, s);

	Envelope env = {.03,.04,.5,.05,.1};
	for(int i=0; i<s; ++i) {
		out[i] *= 5 * volume(env, double(i)/FREQ);
	}
}
void genMachinegun()
{
	float* out = stables[MACHINEGUNSOUND];
    static float extraout[WTS];
    memset(out,0,sizeof(out));
    memset(extraout,0,sizeof(extraout));
	const int n=64;
	float amps[n]={};
	float freqs[n]={};
    srand(0);
	for(int i=0; i<n; ++i) {
		freqs[i] = 5+i+randf();;
		amps[i] = (n*n-i*i)/(float(n*n));
	}
	genWT(out, amps, freqs, n, 44, 6000, 0.1);
	genWT(extraout, amps, freqs, n, 12, 10.0, 0.5);
	//normalize2(out, WTS);
    normalize2(out,WTS);
    normalize2(extraout,WTS);
    for(int i=0;i<WTS;i++)
    {
        float r = randf();
        float s = 0.85;
        out[i]=out[i]*s+(1-s)*extraout[i];
    }
    for(int i=0;i<10;i++){
        //lowpass(out,WTS,1.0/FREQ,1.1);
    }
    normalize2(out,WTS);

	Envelope env = {.00,.35,.15,.15,.0};
	for(int i=0; i<WTS; ++i) {
		//out[i] *= 10 * (WTS-float(i))/WTS;
        out[i] *= 1* volume(env, double(i)/FREQ);
	}
}
void genShotgun()
{
    std::cout<<"SHOTGUNGEN\n";
	float* out = stables[SHOTGUNSOUND];
    static float extraout[WTS];
    memset(out,0,sizeof(out));
    memset(extraout,0,sizeof(extraout));
	const int n=64;
	float amps[n]={};
	float freqs[n]={};
    srand(0);
	for(int i=0; i<n; ++i) {
		freqs[i] = 5+i+randf();;
		amps[i] = (n*n-i*i)/(float(n*n));
	}
	genWT(out, amps, freqs, n, 44, 6000, 0.1);
	genWT(extraout, amps, freqs, n, 12, 10.0, 0.5);
	//normalize2(out, WTS);
    normalize2(out,WTS);
    normalize2(extraout,WTS);
    for(int i=0;i<WTS;i++)
    {
        float r = randf();
        float s = 0.35;
        out[i]=out[i]*s+(1-s)*extraout[i];
    }
    for(int i=0;i<10;i++){
        //lowpass(out,WTS,1.0/FREQ,1.1);
    }
    normalize2(out,WTS);

	Envelope env = {.00,.35,.15,.15,.0};
	for(int i=0; i<WTS; ++i) {
		//out[i] *= 10 * (WTS-float(i))/WTS;
        out[i] *= 0.5* volume(env, double(i)/FREQ);
	}
}
void genFlame()
{
	float* out = stables[FLAMESOUND];
    static float extraout[WTS];
	const int n=64;
	float amps[n];
	float freqs[n];
	for(int i=0; i<n; ++i) {
		freqs[i] = 1+i;//+i+randf();
		amps[i] = 1.f/(1+i);//(n*n-i*i)/(float(n*n));
	}
	//genWT(out, amps, freqs, n, 22, 200.5, .1);
	genWT(out, amps, freqs, n, 220, 300, 4);
	genWT(extraout, amps, freqs, n, 12, 10.0, 0.5);
	//normalize2(out, WTS);
    normalize2(out,WTS);
    normalize2(extraout,WTS);
    for(int i=0;i<WTS;i++)
    {
        float r = randf();
        float s = 0.7;
        out[i]=out[i]*s+(1-s)*extraout[i];
    }
    for(int i=0;i<10;i++){
        lowpass(out,WTS,1.0/FREQ,1.1);
    }
    normalize2(out,WTS);

	Envelope env = {.15,.35,.15,.15,.05};
	for(int i=0; i<WTS; ++i) {
		//out[i] *= 10 * (WTS-float(i))/WTS;
        out[i] *= 0.4* volume(env, double(i)/FREQ);
        //out[i]*=0.1;
	}
}
void genExplosion()
{
	float* out = stables[EXPLOSION];
    static float extraout[WTS];
	const int n=64;
	float amps[n];
	float freqs[n];
	for(int i=0; i<n; ++i) {
		freqs[i] = 5+i+randf();
		amps[i] = (n*n-i*i)/(float(n*n));
	}
	genWT(out, amps, freqs, n, 44, 6000, .1);
	genWT(extraout, amps, freqs, n, 12, 10.0, 0.5);
	//normalize2(out, WTS);
    normalize2(out,WTS);
    normalize2(extraout,WTS);
    for(int i=0;i<WTS;i++)
    {
        float r = randf();
        float s = 0.3;
        out[i]=out[i]*s+(1-s)*extraout[i];
    }
    for(int i=0;i<10;i++){
        //lowpass(out,WTS,1.0/FREQ,1.1);
    }
    normalize2(out,WTS);

	Envelope env = {.00,.35,.15,.15,.0};
	for(int i=0; i<WTS; ++i) {
		//out[i] *= 10 * (WTS-float(i))/WTS;
        out[i] *= 5* volume(env, double(i)/FREQ);
	}
}
void initSounds()
{
	genShotgun();
	genMachinegun();
	genExplosion();
    genFlame();
}

void genSounds(float* buf, int l)
{
	double dt = (double)l / FREQ;
	for(unsigned i=0; i<sounds.size(); ) {
		Sound& s = sounds[i];
		if (s.pos+l < FREQ*stimes[s.type]) {
			for(int j=0; j<l; ++j) buf[j] += s.vol * stables[s.type][j+s.pos];
			s.pos+=l;
			++i;
		} else sounds[i]=sounds.back(), sounds.pop_back();
	}
}
#include<limits.h>
void callback(void* udata, Uint8* stream, int l)
{
	Uint16* s = (Uint16*)stream;
	l /= 2;
	float buf[SAMPLES]={};
	if (playMusic) genMusic(buf, l);
	float buf2[SAMPLES]={};
	if (playSounds) genSounds(buf2, l);
	else sounds.clear();
	// TODO: normalize bufs?
	for(int i=0; i<l; ++i){
        int v =buf[i]*20000 + buf2[i]*10000; 
        if(v>SHRT_MAX)v = SHRT_MAX;
        if(v<SHRT_MIN)v = SHRT_MIN;
        s[i] = v;
    }
	curS += l;
}

const int NHARM=64;
float hfreqs[NWT][NHARM];
float hamps[NWT][NHARM];

const float fundFreqs[NWT]={220,440};
const float fundBws[NWT]={50,20};
const float bwScales[NWT]={.95,.6};

void initInstruments()
{
	for(int i=0; i<NHARM; ++i) hamps[0][i]=1./(1+i), hfreqs[0][i]=1+i;
	for(int i=0; i<NHARM; ++i) hamps[1][i]=1./(1+i)*(i&1?2:1), hfreqs[1][i]=1+i*(1+.1*i);
	for(int i=0; i<2; ++i) {
		genWT(wtables[i], hamps[i], hfreqs[i], NHARM, fundFreqs[i], fundBws[i], bwScales[i]);
	}

	// drums
#if 1
	for(int i=0; i<WTS/2; ++i) {
#if 1
		float d=2*M_PI*randf();
		float a = cos(i*.01);;
		sintable[WTS-1-i]=sintable[i]=a*sin(d);
		costable[WTS-1-i]=costable[i]=a*cos(d);
#else
		sintable[WTS-1-i]=costable[WTS-1-i]=0;
		costable[WTS-1-i]=costable[i]=1;
#endif
	}
	ifft(wtables[2], nulltable, costable, sintable, WTS);
	normalize(wtables[2], WTS);
#else
	for(int i=0; i<WTS; ++i) wtables[2][i] = 2*randf()-1;
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
} // end namespace

void initMusic()
{
	initInstruments();
	initSounds();
	cout<<"wavetables gen done\n";
	if (SDL_OpenAudio(&spec, 0)<0) {
		cout<<"Opening audio device failed: "<<SDL_GetError()<<'\n';
	} else cout<<"Audio open succesful\n";
}

double distvol(double d)
{
    double z = 5;
    return z/(z+d);
}
