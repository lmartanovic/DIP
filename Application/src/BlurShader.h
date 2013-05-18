/******************************************************************************
* DIP - Real-Time Illumination of a Scene - BlurShader.h                      *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - BlurShader - Gaussian blur shader class.                                  *
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

//! Gaussian blur shader
class BlurShader : public Shader
{
public:
	//! Uniform variable initialisation
	virtual void initUniforms()
	{
	  inputTex = glGetUniformLocation(id, "inputTex");
	  discontinuityTex = glGetUniformLocation(id, "discontinuityTex");
	  window = glGetUniformLocation(id, "window");
	}

	GLint inputTex;			/*!< texture to be blurred */
	GLint discontinuityTex; /*!< discontinuity buffer */
	GLint window;			/*!< screen dimensions */
};
