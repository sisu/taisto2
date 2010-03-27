
#ifndef TEXGEN_HPP

#define TEXGEN_HPP
struct Texture{
    int w,h;
    unsigned* pix;
    unsigned glid;
};
extern Texture ammo;

extern void initTextures();

#endif
