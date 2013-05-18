/******************************************************************************
* DIP - Real-Time Illumination of a Scene - RenderShader.h                    *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - RenderShader - MRT scene rendering shader.                                *
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

#ifndef RENDER_SHADER_H
#define RENDER_SHADER_H

#include "Shader.h"

//! MRT scene rendering shader
class RenderShader : public Shader
{
public:
	//! Uniform variable initialisation
	virtual void initUniforms()	
	{
		world = glGetUniformLocation(id, "World");
		view = glGetUniformLocation(id, "View");
		proj = glGetUniformLocation(id, "Proj");
		normalMat = glGetUniformLocation(id, "NormalMat");
		diffuseTex = glGetUniformLocation(id, "diffuseTex");
	};

	GLint world;		/*!< Model world matrix */
	GLint view;			/*!< Camera view matrix */
	GLint proj;			/*!< Camera projection matrix */
	GLint normalMat;	/*!< Normal matrix */
	GLint diffuseTex;	/*!< Diffuse material texture */
};

#endif