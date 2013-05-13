#ifndef QUAD_H
#define QUAD_H

struct QuadVertex {
    float position[3];
    float texcoord[2];
} quadVertices[4] = {
    { {-1.0, -1.0, 0}, {0.0, 0.0} },
    { { 1.0, -1.0, 0}, {1.0, 0.0} },
    { { 1.0,  1.0, 0}, {1.0, 1.0} },
    { {-1.0,  1.0, 0}, {0.0, 1.0} },
};

const unsigned short quad[2][3] = {
    {0, 1, 2},
    {0, 2, 3}
};

#endif