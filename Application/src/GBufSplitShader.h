/******************************************************************************
* DIP - Real-Time Illumination of a Scene - GBufSPlitSHader.h             *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - GBufSplitShader - G-buffer splitting step shader.                         *
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

//! G-buffer splitting shader
class GBufSplitShader : public Shader
{
public:
  //! Uniform variable initialisation
  virtual void initUniforms()
  {
    origColTex = glGetUniformLocation(id, "origColTex");
    origNormTex = glGetUniformLocation(id, "origNormTex");
    origWSCTex = glGetUniformLocation(id, "origWSCTex");
    blocksX = glGetUniformLocation(id, "blocksX");
    blocksY = glGetUniformLocation(id, "blocksY");
    window = glGetUniformLocation(id, "window");
  }

  GLint origColTex;   /*!< Original color texture */
  GLint origNormTex;  /*!< Original normal texture */
  GLint origWSCTex;   /*!< Original world-space coordinates texture */
  GLint blocksX;      /*!< Number of horizontal splitting blocks */
  GLint blocksY;      /*!< Number of vertical splitting blocks */
  GLint window;       /*!< Rendering window dimensions */
};
