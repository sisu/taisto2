#ifndef MODEL_HPP
#define MODEL_HPP

struct Model
{
    float* data;
    short* index;
    int vn;
    int in;
};
extern void draw_model(Model* m);

#endif
