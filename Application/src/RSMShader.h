/******************************************************************************
* DIP - Real-Time Illumination of a Scene - RSMShader.h                       *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - RSMShader - Reflective Shadow Map creation shader.                        *
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

#ifndef RSM_SHADER_H
#define RSM_SHADER_H

#include "Shader.h"

//! Reflective Shadow Map creation shader
class RSMShader : public Shader
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
		viewport = glGetUniformLocation(id, "viewport");
	};

	GLint world;		/*!< Model world matrix */
	GLint view;			/*!< Camera view matrix */
	GLint proj;			/*!< Camera projection matrix */
	GLint normalMat;	/*!< Normal matrix */
	GLint diffuseTex;	/*!< Diffuse material texture */
	GLint viewport;		/*!< Shadow Map dimensions */
};

#endif