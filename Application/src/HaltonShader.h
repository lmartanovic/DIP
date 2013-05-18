/******************************************************************************
* DIP - Real-Time Illumination of a Scene - HaltonShader.h                    *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - HaltonShader - Halton sequence encoding shader.                           *
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

//! Halton texture creation shader
class HaltonShader : public Shader
{
public:
	//! Uniform variable initialisation
	virtual void initUniforms()
	{
		vplSqrt = glGetUniformLocation(id, "vplSqrt");
	}

	GLint vplSqrt;	/*!< Number of VPLs per ISM edge */
};
