/******************************************************************************
* DIP - Real-Time Illumination of a Scene - PullShader.h                      *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - PullShader - Pull-push - pull phase shader.                               *
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

//! Pull-push - pull phase shader.
class PullShader : public Shader
{
public:
  //! Uniform variable initialisation
  virtual void initUniforms()
  {
	  inputTex = glGetUniformLocation(id, "inputTex");
	  viewport = glGetUniformLocation(id, "viewport");
  }	

  GLint inputTex;	/*!< Input (finer) texture */
  GLint viewport;	/*!< Viewport in ping-pong to render to */
};
