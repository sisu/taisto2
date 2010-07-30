#include <cmath>
#include <algorithm>
#include <cassert>
#include <cstring>
#include "padsynth.hpp"
#include "timef.h"
using namespace std;

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
	for(int i=0; i<n/2; ++i) rsrc[n-1-i]=rsrc[i], isrc[n-1-i]=isrc[i];
	calc_fft(rdst,idst,rsrc,isrc,n,-2*M_PI);
	for(int i=0; i<n; ++i) {
		rdst[i] /= n;
		idst[i] /= n;
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
void normalize3(float* a, int n)
{
	const int K=1<<12;
	assert(n%K==0);
	float p=0;
	for(int i=n-K; i<n; ++i) p+=a[i];
	p /= K;
	for(int i=0; i<n; i+=K) {
		float s=0;
		for(int j=0; j<K; ++j) s+=a[i+j];
		s /= K;
		for(int j=0; j<K; ++j) {
			float f = float(j)/K;
			a[i+j] *= .5*(f/s + (1-f)/p);
		}
		p=s;
	}
}

float sintable[WTS], costable[WTS], nulltable[WTS];
#if 0
void ifftw(float* out, float* st, float* ct, int n)
{
	fftw_complex *in,*outt;
	in=(fftw_complex*)fftw_malloc(sizeof(fftw_complex)*n);
	outt=(fftw_complex*)fftw_malloc(sizeof(fftw_complex)*n);
	fftw_plan p = fftw_plan_dft_1d(n, in, outt, FFTW_BACKWARD, FFTW_ESTIMATE);
	for(int i=0; i<n/2; ++i) {
		in[i][0] = ct[i];
		in[i][1] = st[i];
	}
//	for(int i=0; i<100; ++i) cout<<out[i]<<'\n';
	for(int i=0; i<n/2; ++i) {
		in[WTS-1-i][0] = in[i][0];
		in[WTS-1-i][1] = in[i][1];
	}
	fftw_execute(p);
	for(int i=0; i<WTS; ++i) out[i]=outt[i][0];
}
#endif

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
//	ifftw(out, costable, sintable, WTS);

	normalize2(out,WTS);
}

