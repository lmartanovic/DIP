/******************************************************************************
* DIP - Real-Time Illumination of a Scene - DiscontinuityShader.h             *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - DiscontinuityShader - Discontinuity buffer creation shader.               *
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

//! Discontinuity buffer creation shader
class DiscontinuityShader : public Shader
{
public:
  //! Uniform variable initialisation
  virtual void initUniforms()
  {
    normalTex = glGetUniformLocation(id, "normalTex");
    depthTex = glGetUniformLocation(id, "depthTex");
    distThresh = glGetUniformLocation(id, "distThresh");
    normThresh = glGetUniformLocation(id, "normThresh");
    window = glGetUniformLocation(id, "window");
  }

  GLint normalTex;  /*!< Normals - Camera POV */
  GLint depthTex;   /*!< Depth map - Camera POV */
  GLint distThresh; /*!< Depth threshold value */
  GLint normThresh; /*!< Normal threshold value */
  GLint window;     /*!< Rendering window dimensions */
};
