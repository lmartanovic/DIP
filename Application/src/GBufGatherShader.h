/******************************************************************************
* DIP - Real-Time Illumination of a Scene - GBufGatherShader.h                *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - GBufGatherShader - G-buffer gathering step shader.                        *
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

//! G-buffer gathering shader
class GBufGatherShader : public Shader
{
public:
  //! Uniform variable initialisation
  virtual void initUniforms()
  {
	  blocksX = glGetUniformLocation(id, "blocksX");
	  blocksY = glGetUniformLocation(id, "blocksY");
	  window = glGetUniformLocation(id, "window");
	  splitTex = glGetUniformLocation(id, "splitTex");
	}

  GLint blocksX;  /*!< Number of horizontal splitting blocks */
  GLint blocksY;  /*!< Number of vertical splitting blocks */
  GLint window;   /*!< Rendering window dimensions */
  GLint splitTex; /*!< Split texture to be gathered */
};
