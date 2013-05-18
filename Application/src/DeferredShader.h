/******************************************************************************
* DIP - Real-Time Illumination of a Scene - DeferredShader.h                  *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - DeferredShader - Deferred illumination shader class.                      *
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

//! Deferred illumination shader
class DeferredShader : public Shader
{
public:
	//! Uniform variable initialisation
	virtual void initUniforms() {
		wscTex = glGetUniformLocation(id, "wscTex");
		normalTex = glGetUniformLocation(id, "normalTex");
		colorTex = glGetUniformLocation(id, "colorTex");
		ismViewport = glGetUniformLocation(id, "ismViewport");
		vplSqrt = glGetUniformLocation(id, "vplSqrt");
		ismTex = glGetUniformLocation(id, "ismTex");
		rsmWSCTex = glGetUniformLocation(id, "rsmWSCTex");
		rsmNormalTex = glGetUniformLocation(id, "rsmNormalTex");
		rsmColorTex = glGetUniformLocation(id, "rsmColorTex");
		haltonTex = glGetUniformLocation(id, "haltonTex");
		//direct
		shadowTex = glGetUniformLocation(id, "shadowTex");
		view = glGetUniformLocation(id, "View");
		biasDVP = glGetUniformLocation(id, "BiasDVP");
		cameraPos = glGetUniformLocation(id, "cameraPos");
		lightPos = glGetUniformLocation(id, "lightPos");

		window = glGetUniformLocation(id, "window");
		blocksX = glGetUniformLocation(id, "blocksX");
		blocksY = glGetUniformLocation(id, "blocksY");
		mode = glGetUniformLocation(id, "mode");
	};

	GLint wscTex;		/*!< World-space coordinates - camera POV */
	GLint normalTex;	/*!< Normals - camera POV */
	GLint colorTex;		/*!< Color - camera POV */
	GLint ismViewport;	/*!< Post pull-push ISM dimensions */
	GLint vplSqrt;		/*!< Number of VPLs per ISM edge */
	GLint ismTex;		/*!< ISM */
	GLint rsmWSCTex;	/*!< World-space coordinates - light POV */
	GLint rsmNormalTex; /*!< Normals - light POV */
	GLint rsmColorTex;	/*!< Color - light POV */
	GLint haltonTex;	/*!< Halton texture */
	GLint shadowTex;	/*!< Depth map - camera POV */
	GLint view;			/*!< Camera view matrix */
	GLint biasDVP;		/*!< Light bias Vie-Projection matrix */
	GLint cameraPos;	/*!< Camera position in world-space */
	GLint lightPos;		/*!< Primary light position in world-space */
	GLint window;		/*!< Rendering window dimensions */
	GLint blocksX;		/*!< Number of horizontal G-buffer splitting blocks */
	GLint blocksY;		/*!< Number of vertical G-buffer splitting blocks */
	GLint mode;			/*!< Rendering mode (indirect/direct/combined) */
};