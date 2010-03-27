#include<iostream>
#include<cmath>
#include<GL/gl.h>
#include<GL/glu.h>
#include"texgen.hpp"
static Texture getAmmo()
{
    Texture ret;
    ret.w = 32;
    ret.h = 32;
    ret.pix = new unsigned[ret.w*ret.h];
    for(int x=0;x<ret.w;x++)
    {
        for(int y=0;y<ret.h;y++)
        {
            float z = (ret.w/2+0.5-x);
            z = z*z+(ret.h/2-y)*(ret.h/2-y);
            if(z!=0)
                z = 420000.0/(z*z);

            int iz = std::min(int(z),255);
            if(iz<5)iz=0;
            ret.pix[y*ret.w+x]=iz|iz<<24|(iz/2)<<16;
        }
    }
    unsigned tex;
    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_2D,tex);
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the original
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    std::cout<<"mipmaps = "<<gluBuild2DMipmaps( GL_TEXTURE_2D, 4,ret.w,ret.h,
            GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, ret.pix );
    ret.glid = tex;
    glBindTexture(GL_TEXTURE_2D,0);
    return ret;
}

Texture ammo;

void initTextures()
{
    ammo = getAmmo();
}
