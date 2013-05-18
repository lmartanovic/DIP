/******************************************************************************
* DIP - Real-Time Illumination of a Scene - main.cpp                          *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - Simple ISM demonstration.                                                 *
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

//standard library
#include <iostream>
//DIP
#include "Renderer.h"
//main
int main(int argc, char* argv[])
{
  Renderer renderer;

  renderer.init(argv[1]);
  renderer.run();
  //cleanup - destroy VAOs, FBOs, ...
  return 0;
}
