/******************************************************************************
* DIP - Real-Time Illumination of a Scene - Renderer.h                        *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - Renderer - Main rendering "engine" class.                                 *
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

#ifndef RENDERER_H
#define RENDERER_H

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//SFML
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
//DIP
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "BlurShader.h"
#include "DeferredShader.h"
#include "DiscontinuityShader.h"
#include "GBufGatherShader.h"
#include "GBufSplitShader.h"
#include "HaltonShader.h"
#include "ISMShader.h"
#include "QuadShader.h"
#include "PointsShader.h"
#include "PullShader.h"
#include "PushShader.h"
#include "RenderShader.h"
#include "RSMShader.h"

#define WIN_HEIGHT 600
#define WIN_WIDTH 800
#define GBUF_BLOCKS_X 5
#define GBUF_BLOCKS_Y 5
#define ISM_TILE_EDGE 256	//pixels per ISM tile

#define NUM_FBOS 10

//! Framebuffer object types
enum FBOType
{
	RENDER_FBO,			/*!< MRT rendering */
	DEFERRED_FBO,		/*!< Deferred shading */
	RSM_FBO,			/*!< Reflective shadow map */
	HALTON_FBO,			/*!< Halton texturecreation */
	SPLIT_FBO,			/*!< G-buffer splitting */
	GATHER_FBO,			/*!< G-buffer gathering */
	DISCONTINUITY_FBO,	/*!< Discontinuity buffer creation */
	XBLUR_FBO,			/*!< X blur */
	YBLUR_FBO,			/*!< Y blut */
	ISM_FBO				/*!< ISM creation */
};

/*!
 * \class Renderer
 * \brief Main rendering "engine"
*/
class Renderer
{
public:
	//! Default constructor
	Renderer();
	//! Initialisation
	/*!
	  \param modelFile - file containing the model/scene
	*/
	void init(char* modelFile);
	//! Start the rendering loop
	void run();
private:
	//! Draw the scene
	void draw();
	//! Draw screen-aligned quad
	void drawFullscreenQuad();
	//! Handle keyboard input
	/*!
	  \param event - SFML event object
	*/
	void handleKeyPressed(sf::Event & event);
	//! Handle mouse wheel movement
	/*!
	  \param event - SFML event object
	*/
	void handleMouseWheel(sf::Event & event);
	//! Create and initialise camera
	void initCamera();
	//! Create and initialise offscreen rendering framebuffers
	void initFramebuffers();
	//! Load and initialise scene geometry
	/*!
	  \param modelFile - file containing the scene/model information
	*/
	void initGeometry(char* modelFile);
	//! Create texture encoding halton sequence
	void initHalton();
	//! Create and initialise primary light source
	void initLight();
	//! Create OpenGL rendering context
	void initOpenGL();
	//! Create and initialise screen-aligned quad model
	void initQuad();
	//! Initialise rendering window and SFML functionality
	void initSFML();
	//! Create, load and compile shader programs
	void initShaders();
	//! Set texture coordinates corresponding to quad vertices
	/*!
	  \param mipMapLevel - level of pull-push algorithm
	*/
	void setQuadTexCoord(int mipMapLevel);

	bool running;				/*!< Exit flag */
	int mode;					/*!< Rendering mode (indirect/direct/combined) flag */
	int winHeight;				/*!< Rendering winodw height */
	int winWidth;				/*!< Rendering window width */
	sf::RenderWindow window;	/*!< Rendering window */

	Camera camera;				/*! Camera */
	Light light;				/*! Primary light source */
	Model model;				/*! Scene geometry */
	
	//! Various shaders
	BlurShader xBlurShader, yBlurShader;
	DeferredShader deferredShader;
	DiscontinuityShader discontinuityShader;
	GBufGatherShader gatherShader;
	GBufSplitShader splitShader;
	HaltonShader haltonShader;
	ISMShader ismShader;
	QuadShader quadShader;
	PointsShader pointsShader;
	PullShader pullShader;
	PushShader pushShader;
	RenderShader renderShader;
	RSMShader rsmShader;

	GLuint FBOs[NUM_FBOS];		/*!< Offscreen rendering framebuffer objects */
	//! Offscreen rendering textures
	GLuint renderDepthTex, renderWSCTex, renderNormalTex, renderColorTex;
	GLuint deferredColTex;
	GLuint rsmDepthTex, rsmWSCTex, rsmNormalTex, rsmColorTex;
	GLuint haltonDepthTex, haltonTex;
	GLuint splitWSCTex, splitNormalTex, splitColorTex;
	GLuint discontinuityDepthTex, discontinuityTex;
	GLuint ISMTextureLevel0, ISMTextureLevel1;
	//fullscreen quad rendering
	GLuint quadVAO;				/*!< Screen-aligned quad VAO */
	GLuint quadBuffers[2];		/*!< Screen-aligned buffers (VBO, EBO) */
};
#endif