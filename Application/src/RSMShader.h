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

#include "Shader.h"


//! Reflective Shadow Map creation shader
class RSMShader : public Shader
{
public:
	//! Uniform variable initialisation
	virtual void initUniforms();

	GLint depthMVPUni;
	GLint depthNMUni;
	GLint depthWorldUni;
	GLint depthDiffMaterial;
};

void RSMShader::initUniforms()
{
	depthMVPUni = glGetUniformLocation(id, "depthMVP");
	depthNMUni = glGetUniformLocation(id, "normalMatrix");
	depthWorldUni = glGetUniformLocation(id, "worldMatrix");
	depthDiffMaterial = glGetUniformLocation(id, "diffuseMaterialComponent");
}
