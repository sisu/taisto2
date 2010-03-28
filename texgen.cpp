#include<iostream>
#include<cstdlib>
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
static Texture getSalama()
{
    Texture ret;
    ret.w = 64;
    ret.h = 64;
    ret.pix = new unsigned[ret.w*ret.h];
    for(int x=0;x<ret.w;x++)
    {
        for(int y=0;y<ret.h;y++)
        {
            float z = (ret.w/2+0.5-x);
            z = abs(z);
            if(z!=0)
                z = 420.0/(z);
            else z = 1000;
            z+=z*(0.1*(rand()%10));
            //int iz = std::min(int(z),700);
            int iz = z;
            iz=(iz+70)/2;
            if(iz<10)iz=0;
            ret.pix[y*ret.w+x]=(std::max(0,std::min(255,iz-40)))|std::min(255,iz/2)<<24|std::min(255,iz/2)<<16|std::min(255,int(sqrt(1000*iz)))<<8;
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
Texture salama;

void initTextures()
{
    ammo = getAmmo();
    salama = getSalama();
}
