
#ifndef TEXGEN_HPP

#define TEXGEN_HPP
struct Texture{
    int w,h;
    unsigned* pix;
    unsigned glid;
};
extern Texture ammo;
extern Texture salama;
extern unsigned explosionTex;
extern unsigned groundTex;
extern unsigned buildingTex;
extern unsigned healthTex;

extern void initTextures();

#endif
