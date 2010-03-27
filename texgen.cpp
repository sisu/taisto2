#include<iostream>
#include<GL/gl.h>
#include<GL/glu.h>
#include"texgen.hpp"
static Texture getAmmo()
{
    Texture ret;
    return ret;
    ret.w = 32;
    ret.h = 32;
    ret.pix = new unsigned[ret.w*ret.h];
    for(int x=0;x<ret.w;x++)
    {
        for(int y=0;y<ret.h;y++)
        {
            int z = (ret.w/2-x);
            z = z*z+(ret.h/2-y)*(ret.h/2-y);
            z = std::max(z,255);
            ret.pix[y*ret.w+x]=z<<24|z<<16;
        }
    }
    return ret;
    unsigned tex;
    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_2D,tex);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the original
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    gluBuild2DMipmaps( GL_TEXTURE_2D, 4,ret.w,ret.h,
            GL_RGBA, GL_UNSIGNED_BYTE, ret.pix );
    ret.glid = tex;
    glBindTexture(GL_TEXTURE_2D,0);
    return ret;
}

Texture ammo;

void initTextures()
{
    ammo = getAmmo();
}
