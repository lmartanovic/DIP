/******************************************************************************
* DIP - Real-Time Illumination of a Scene - ISMShader.h                       *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - ISMShader - Imperfect Shadow Map generation shader.                       *
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

//! ISM creation shader
class ISMShader : public Shader
{
public:
	//! Uniform variable initialisation
	virtual void initUniforms()
	{
		vplSqrt = glGetUniformLocation(id, "vplSqrt");
		haltonTex = glGetUniformLocation(id, "halton");
		wscTex = glGetUniformLocation(id, "wscTex");
		normalTex = glGetUniformLocation(id, "normalTex");
		world = glGetUniformLocation(id, "World");
	}

	GLint vplSqrt;		/*!< Number of VPLs per ISM edge */
	GLint haltonTex;	/*!< Halton texture */
	GLint wscTex;		/*!< World-space coordinates - light POV */
	GLint normalTex;	/*!< Normals - light POV */
	GLint world;		/*!< Point cloud world matrix */
};
