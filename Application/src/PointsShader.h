/******************************************************************************
* DIP - Real-Time Illumination of a Scene - PointsShader.h                    *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - PointsShader - Point cloud rendering shader.                              *
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

//! Point cloud rendering shader
class PointsShader : public Shader
{
public:
	//! Uniform variable initialisation
	virtual void initUniforms()
	{
		worldMat = glGetUniformLocation(id, "WorldMat");
		viewMat = glGetUniformLocation(id, "ViewMat");
		projMat = glGetUniformLocation(id, "ProjMat");
	}

	GLint worldMat;	/*!< Point cloud world matrix */
	GLint viewMat;	/*!< Camera view matrix */
	GLint projMat;	/*!< Camera projection matrix */
};