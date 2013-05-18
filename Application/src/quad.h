/******************************************************************************
* DIP - Real-Time Illumination of a Scene - quad.h                            *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - Simple screen aligned quad geometry specification.                        *
*                                                                             *
*******************************************************************************
* Author                                                                      *
* ------                                                                      *
* Lukáš Martanovič (xmarta00@stud.fit.vutbr.cz)                               *
*                                                                             *
* 18.05.2013                                                                  *
*                                                                             *
*******************************************************************************
* This software is not copyrighted.                                           *
*                                                                             *
* This source code is offered for use in the public domain.                   *
* You may use, modify or distribute it freely.                                *
*                                                                             *
******************************************************************************/

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