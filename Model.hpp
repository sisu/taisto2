#ifndef MODEL_HPP
#define MODEL_HPP
#include<GL/gl.h>
struct Model
{
    float* data;
    short* index;
    int vn;
    int in;
};
extern void draw_model(Model* m,unsigned mode= GL_TRIANGLES);

#endif
