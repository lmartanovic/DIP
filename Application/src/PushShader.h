/******************************************************************************
* DIP - Real-Time Illumination of a Scene - PushShader.h                      *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - PushShader - Pull-push - push phase shader.                               *
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

#include "Shader.h"

//! Pull-push - push phase shader
class PushShader : public Shader
{
public:
  //! Uniform variable initialisation
  virtual void initUniforms()
  {
    currentTex = glGetUniformLocation(id, "currentTex");
    coarserTex = glGetUniformLocation(id, "coarserTex");
    offset = glGetUniformLocation(id, "offset");
    lvlSwitch = glGetUniformLocation(id, "lvl");
    viewport = glGetUniformLocation(id, "viewport");
  }

  GLint currentTex; /*!< Current mip-map level */
  GLint coarserTex; /*!< Coarser mip-map level */
  GLint offset;     /*!< Viewport offset */
  GLint lvlSwitch;  /*!< Odd/even mip-map level indicator */
  GLint viewport;   /*!< Viewport dimensions */
};
