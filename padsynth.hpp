#ifndef PADSYNTH_HPP
#define PADSYNTH_HPP

void fft(float* rdst, float* idst, const float* rsrc, const float* isrc, int n);
void ifft(float* rdst, float* idst, float* rsrc, float* isrc, int n);
void normalize(float* a, int n);
void normalize2(float* a, int n);
void normalize3(float* a, int n);
void genWT(float* out, const float* amps, const float* freqs, int n, float fundfreq, float fundbw, float bwscale);

const int WTS=1<<16;
const int FREQ = 44100;
extern float sintable[WTS], costable[WTS], nulltable[WTS];


#endif
