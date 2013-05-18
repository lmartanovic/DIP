/******************************************************************************
* DIP - Real-Time Illumination of a Scene - QuadShader.h                      *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - QuadShader - Screen-aligned quad with texture rendering sahder.           *
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

//! Screen-aligned quad with texture rendering sahder
class QuadShader : public Shader
{
public:
	//! Uniform variable initialisation
	virtual void initUniforms() {
		tex = glGetUniformLocation(id, "tex");
	};

	GLint tex;	/*!< Texture to be rendered fullscreene */
};