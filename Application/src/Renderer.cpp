/******************************************************************************
* DIP - Real-Time Illumination of a Scene - Renderer.cpp                      *
*******************************************************************************
* Contents                                                                    *
* --------                                                                    *
* - Main rendering "engine" implementation.                                   *
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

#include <iostream>
#include "Renderer.h"
#include "quad.h"

//! Default constructor
Renderer::Renderer()
: running(true),
  mode(2),
  winHeight(WIN_HEIGHT),
  winWidth(WIN_WIDTH)
{}
//! Initialisation
void Renderer::init(char* modelFile)
{
	std::cout << "Initialising context..." << std::endl;
	initSFML();
	initOpenGL();
	initCamera();
	initLight();
	std::cout << "Creating shaders..." << std::endl;
	initShaders();
	std::cout << "Loading models..." << std::endl;
	initGeometry(modelFile);
	initQuad();
	std::cout << "Creating FBOs..." << std::endl;
	initFramebuffers();
	std::cout << "Creating Halton texture..." << std::endl;
	initHalton();
	//more inits
}

//----------------------------------------------------------------------------
//MAIN LOOP
//! Start the rendering loop
void Renderer::run()
{
	while(running)
	{
		//poll events
		sf::Event event;
		while(window.pollEvent(event))
		{
			switch(event.type)
			{
				//window events
			case sf::Event::Closed:
				running = false;
				break;
			case sf::Event::Resized:
				winWidth = event.size.width;
				winHeight = event.size.height;
				glViewport(0,0,winWidth, winHeight);
				camera.recomputeProjection(winWidth, winHeight);
				break;
				//keyboard
			case sf::Event::KeyPressed:
				handleKeyPressed(event);
				break;
				//mouse
			case sf::Event::MouseWheelMoved:
				handleMouseWheel(event);
				break;
			case sf::Event::MouseMoved:
				camera.look(event.mouseMove.x, event.mouseMove.y);
				break;
				//unhandled
			default:
				break;
			}
		}//pollEvent

		//reset mouse position to the center of the screem
		sf::Mouse::setPosition(sf::Vector2i(winWidth/2,winHeight/2), window);
		draw();
		window.display();
		camera.resetMoveFlag();
		light.resetMoveFlag();
	}//main loop
}

//----------------------------------------------------------------------------
//Inits
//! Create and initialise camera
void Renderer::initCamera()
{
	camera.create();	//create default camera
}
//! Create and initialise primary light source
void Renderer::initLight()
{
	light.create();		//create default light
}
//! Create and initialise offscreen rendering framebuffers
void Renderer::initFramebuffers()
{
	glGenFramebuffers(NUM_FBOS, FBOs);
	//-------------------------------------------------------------------------
	// HALTON TEXTURE CREATION
	//-------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, FBOs[HALTON_FBO]);
	//textures
	glGenTextures(1, &haltonDepthTex);
	glBindTexture(GL_TEXTURE_2D, haltonDepthTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, VPL_SQRT, VPL_SQRT, 0,
	             GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	//color coded halton coordinates
	glGenTextures(1, &haltonTex);
  	glBindTexture(GL_TEXTURE_2D, haltonTex);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, VPL_SQRT, VPL_SQRT, 0, GL_RG, GL_FLOAT, NULL);
	//attach to FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
						   GL_TEXTURE_2D, haltonDepthTex, 0);
  	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
  		 			 	   GL_TEXTURE_2D, haltonTex, 0);
  	//specify draw buffers
  	GLenum HMRT[] = {GL_COLOR_ATTACHMENT0};
  	glDrawBuffers(1, HMRT);
  	//check completeness
  	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    	std::cout << "Cosi sa posralo halton" << std::endl;
	//-------------------------------------------------------------------------
	// CAMERA POV RENDER
	//-------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, FBOs[RENDER_FBO]);
	//textures
	glGenTextures(1, &renderDepthTex);
	glBindTexture(GL_TEXTURE_2D, renderDepthTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,
				 WIN_WIDTH, WIN_HEIGHT, 0,
				 GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	//create world-space coord texture
	glGenTextures(1, &renderWSCTex);
	glBindTexture(GL_TEXTURE_2D, renderWSCTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
	             WIN_WIDTH, WIN_HEIGHT, 0,
	             GL_RGBA, GL_FLOAT, 0);
	//create normal texture
	glGenTextures(1, &renderNormalTex);
	glBindTexture(GL_TEXTURE_2D, renderNormalTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F,
	             WIN_WIDTH, WIN_HEIGHT, 0,
	             GL_RGB, GL_FLOAT, 0);
	//create color texture
	glGenTextures(1, &renderColorTex);
	glBindTexture(GL_TEXTURE_2D, renderColorTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
				 WIN_WIDTH, WIN_HEIGHT, 0,
				 GL_RGBA, GL_FLOAT, 0);
	//attach textures to FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
	                       renderDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           renderWSCTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
                           renderNormalTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
                           renderColorTex, 0);
	//set draw buffers
	GLenum renderMRT[] = {
		GL_COLOR_ATTACHMENT0,//wsc location0
		GL_COLOR_ATTACHMENT1,//normals location1
		GL_COLOR_ATTACHMENT2 //color location2
	};
	glDrawBuffers(3, renderMRT);
	//check
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    	std::cerr << "Cosi sa posralo render" << std::endl;
    //-------------------------------------------------------------------------
	// Buffer Split
	//-------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, FBOs[SPLIT_FBO]);
	//textures
	//create world-space coord texture
	glGenTextures(1, &splitWSCTex);
	glBindTexture(GL_TEXTURE_2D, splitWSCTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
	             WIN_WIDTH, WIN_HEIGHT, 0,
	             GL_RGBA, GL_FLOAT, 0);
	//create normal texture
	glGenTextures(1, &splitNormalTex);
	glBindTexture(GL_TEXTURE_2D, splitNormalTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F,
	             WIN_WIDTH, WIN_HEIGHT, 0,
	             GL_RGB, GL_FLOAT, 0);
	//create color texture
	glGenTextures(1, &splitColorTex);
	glBindTexture(GL_TEXTURE_2D, splitColorTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
				 WIN_WIDTH, WIN_HEIGHT, 0,
				 GL_RGBA, GL_FLOAT, 0);
	//attach textures to FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
	                       renderDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           splitWSCTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
                           splitNormalTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
                           splitColorTex, 0);
	//set draw buffers
	GLenum splitMRT[] = {
		GL_COLOR_ATTACHMENT0,//wsc location0
		GL_COLOR_ATTACHMENT1,//normals location1
		GL_COLOR_ATTACHMENT2 //color location2
	};
	glDrawBuffers(3, splitMRT);
	//check
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    	std::cerr << "Cosi sa posralo split" << std::endl;
    //-------------------------------------------------------------------------
	// LIGHT POV RENDER
	//-------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, FBOs[RSM_FBO]);
	//textures
	glGenTextures(1, &rsmDepthTex);
	glBindTexture(GL_TEXTURE_2D, rsmDepthTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0,
				 GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	//create world-space coord texture
	glGenTextures(1, &rsmWSCTex);
	glBindTexture(GL_TEXTURE_2D, rsmWSCTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
	             SHADOW_WIDTH, SHADOW_HEIGHT, 0,
	             GL_RGBA, GL_FLOAT, 0);
	//create normal texture
	glGenTextures(1, &rsmNormalTex);
	glBindTexture(GL_TEXTURE_2D, rsmNormalTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F,
	             SHADOW_WIDTH, SHADOW_HEIGHT, 0,
	             GL_RGB, GL_FLOAT, 0);
	//create color texture
	glGenTextures(1, &rsmColorTex);
	glBindTexture(GL_TEXTURE_2D, rsmColorTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
				 SHADOW_WIDTH, SHADOW_HEIGHT, 0,
				 GL_RGBA, GL_FLOAT, 0);
	//attach textures to FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
	                       rsmDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           rsmWSCTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
                           rsmNormalTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
                           rsmColorTex, 0);
	//set draw buffers
	GLenum rsmMRT[] = {
		GL_COLOR_ATTACHMENT0,//wsc location0
		GL_COLOR_ATTACHMENT1,//normals location1
		GL_COLOR_ATTACHMENT2 //color location2
	};
	glDrawBuffers(3, rsmMRT);
	//check
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    	std::cerr << "Cosi sa posralo rsm" << std::endl;
    //-------------------------------------------------------------------------
	// DEFERRED SHADING
	//-------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, FBOs[DEFERRED_FBO]);
	//generate the output color texture
	glGenTextures(1, &deferredColTex);
	glBindTexture(GL_TEXTURE_2D, deferredColTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
	             WIN_WIDTH, WIN_HEIGHT, 0,
	             GL_RGBA, GL_FLOAT, 0);
	//attach textures to FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
	                       renderDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           deferredColTex, 0);
	//set draw buffer
	GLenum deferredRT[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, deferredRT);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    	std::cerr << "Cosi sa posralo deferred" << std::endl;
    //-------------------------------------------------------------------------
	// G-BUFFER GATHERING
	//-------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, FBOs[GATHER_FBO]);
	//we will reuse render textures
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
	                       renderDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           renderColorTex, 0);
	//set draw buffer
	GLenum gatherRT[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, gatherRT);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    	std::cerr << "Cosi sa posralo gather" << std::endl;
    //-------------------------------------------------------------------------
	// DISCONTINUITY BUFFER CREATION
	//-------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, FBOs[DISCONTINUITY_FBO]);
	//cretae textures
	glGenTextures(1, &discontinuityDepthTex);
	glBindTexture(GL_TEXTURE_2D, discontinuityDepthTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,
				 WIN_WIDTH, WIN_HEIGHT, 0,
				 GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	//color
	glGenTextures(1, &discontinuityTex);
	glBindTexture(GL_TEXTURE_2D, discontinuityTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
	             WIN_WIDTH, WIN_HEIGHT, 0,
	             GL_RGBA, GL_FLOAT, 0);
	//attach textures
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
	                       discontinuityDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           discontinuityTex, 0);
	//set draw buffer
	GLenum discontinuityRT[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, discontinuityRT);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    	std::cerr << "Cosi sa posralo discontinuity" << std::endl;
    //-------------------------------------------------------------------------
	// X BLUR
	//-------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, FBOs[XBLUR_FBO]);
	//we will reuse render textures
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
	                       renderDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           deferredColTex, 0);
	//set draw buffer
	GLenum xBlurRT[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, xBlurRT);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    	std::cerr << "Cosi sa posralo xBlur" << std::endl;
    //-------------------------------------------------------------------------
	// Y BLUR
	//-------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, FBOs[YBLUR_FBO]);
	//we will reuse render textures
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
	                       renderDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           renderColorTex, 0);
	//set draw buffer
	GLenum yBlurRT[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, yBlurRT);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    	std::cerr << "Cosi sa posralo yBlur" << std::endl;
    //-------------------------------------------------------------------------
	// ISM and Ping-Pong
	//-------------------------------------------------------------------------
	glBindFramebuffer(GL_FRAMEBUFFER, FBOs[ISM_FBO]);
	//create ping
	glGenTextures(1, &ISMTextureLevel0);
	glBindTexture(GL_TEXTURE_2D, ISMTextureLevel0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
	             VPL_SQRT*ISM_TILE_EDGE+(VPL_SQRT/2*ISM_TILE_EDGE),
	             VPL_SQRT*ISM_TILE_EDGE, 0,
	             GL_RGBA, GL_FLOAT, 0);
	//create pong
	glGenTextures(1, &ISMTextureLevel1);
	glBindTexture(GL_TEXTURE_2D, ISMTextureLevel1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
	             VPL_SQRT*ISM_TILE_EDGE+(VPL_SQRT/2*ISM_TILE_EDGE),
	             VPL_SQRT*ISM_TILE_EDGE, 0,
	             GL_RGBA, GL_FLOAT, 0);
	//attach ping
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
	                       ISMTextureLevel0, 0);
	//attach pong
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
	                       ISMTextureLevel1, 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Cosi sa posralo ISM" << std::endl;

	//unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//! Load and initialise scene geometry
void Renderer::initGeometry(char* modelFile)
{
	model.import(modelFile);
}
//! Create texture encoding halton sequence
void Renderer::initHalton()
{
	//bind halton FBO
	glBindFramebuffer(GL_FRAMEBUFFER, FBOs[HALTON_FBO]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//change viewport to halton resolution
	glViewport(0, 0, VPL_SQRT, VPL_SQRT);
	//draw
	haltonShader.use();
	setUniform(haltonShader.vplSqrt, (int)VPL_SQRT);
	drawFullscreenQuad();
	//change the viewport back
	glViewport(0, 0, winWidth, winHeight);
	//unbind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//! Create OpenGL rendering context
void Renderer::initOpenGL()
{
	glewExperimental = GL_TRUE;
	glewInit();

	//OGL inits
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glCullFace(GL_BACK);
}
//! Create and initialise screen-aligned quad model
void Renderer::initQuad()
{
	//generate VAO
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	//generate buffers
	glGenBuffers(2, quadBuffers);
	//fill in vertices
	glBindBuffer(GL_ARRAY_BUFFER, quadBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices),
                 quadVertices, GL_STATIC_DRAW);
	//fill in indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad),
                 quad, GL_STATIC_DRAW);
	//init attributes
	glEnableVertexAttribArray(0);	//enable position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex),
                          (void*)offsetof(QuadVertex, position));
	glEnableVertexAttribArray(1);	//enable texcoord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex),
                          (void*)offsetof(QuadVertex, texcoord));
	//unbind
	glBindVertexArray(0);
}
//! Initialise rendering window and SFML functionality
void Renderer::initSFML()
{
	sf::ContextSettings settings;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;

	window.create(sf::VideoMode(winWidth, winHeight),
				  "DIP",
				  sf::Style::Default,
				  settings);
	window.setMouseCursorVisible(false);	//hide mouse cursor
}
//! Create, load and compile shader programs
void Renderer::initShaders()
{
	//Camera POV rendering
	renderShader.create();
	renderShader.addShader(VS, "Application/shaders/render.vs");
	renderShader.addShader(FS, "Application/shaders/render.fs");
	renderShader.link();
	renderShader.initUniforms();
	//Light POV rendering
	rsmShader.create();
	rsmShader.addShader(VS, "Application/shaders/render.vs");
	rsmShader.addShader(FS, "Application/shaders/rsm.fs");
	rsmShader.link();
	rsmShader.initUniforms();
	//simple quad rendering with single texture
	quadShader.create();
	quadShader.addShader(VS, "Application/shaders/quad.vs");
	quadShader.addShader(FS, "Application/shaders/quad.fs");
	quadShader.link();
	quadShader.initUniforms();
	//point rendering
	pointsShader.create();
	pointsShader.addShader(VS, "Application/shaders/points.vs");
	pointsShader.addShader(FS, "Application/shaders/points.fs");
	pointsShader.link();
	pointsShader.initUniforms();
	//deferred shader
	deferredShader.create();
	deferredShader.addShader(VS, "Application/shaders/quad.vs");
	deferredShader.addShader(FS, "Application/shaders/deferred.fs");
	deferredShader.link();
	deferredShader.initUniforms();
	//halton texture
	haltonShader.create();
	haltonShader.addShader(VS, "Application/shaders/quad.vs");
	haltonShader.addShader(FS, "Application/shaders/halton.fs");
	haltonShader.link();
	haltonShader.initUniforms();
	//G-buffer splitting shader
	splitShader.create();
	splitShader.addShader(VS, "Application/shaders/quad.vs");
	splitShader.addShader(FS, "Application/shaders/gBufSplit.fs");
	splitShader.link();
	splitShader.initUniforms();
	//G-buffer gathering shader
	gatherShader.create();
	gatherShader.addShader(VS, "Application/shaders/quad.vs");
	gatherShader.addShader(FS, "Application/shaders/gBufGather.fs");
	gatherShader.link();
	gatherShader.initUniforms();
	//discontinuity buffer creation
	discontinuityShader.create();
	discontinuityShader.addShader(VS, "Application/shaders/quad.vs");
	discontinuityShader.addShader(FS, "Application/shaders/discontinuity.fs");
	discontinuityShader.link();
	discontinuityShader.initUniforms();
	//x Blur
	xBlurShader.create();
	xBlurShader.addShader(VS, "Application/shaders/quad.vs");
	xBlurShader.addShader(FS, "Application/shaders/xBlur.fs");
	xBlurShader.link();
	xBlurShader.initUniforms();
	//y Blur
	yBlurShader.create();
	yBlurShader.addShader(VS, "Application/shaders/quad.vs");
	yBlurShader.addShader(FS, "Application/shaders/yBlur.fs");
	yBlurShader.link();
	yBlurShader.initUniforms();
	//ISM creation
	ismShader.create();
	ismShader.addShader(VS, "Application/shaders/ism.vs");
	ismShader.addShader(FS, "Application/shaders/ism.fs");
	ismShader.link();
	ismShader.initUniforms();
	//pull
	pullShader.create();
	pullShader.addShader(VS, "Application/shaders/quad.vs");
	pullShader.addShader(FS, "Application/shaders/pull.fs");
	pullShader.link();
	pullShader.initUniforms();
	//push
	pushShader.create();
	pushShader.addShader(VS, "Application/shaders/quad.vs");
	pushShader.addShader(FS, "Application/shaders/push.fs");
	pushShader.link();
	pushShader.initUniforms();
}

//----------------------------------------------------------------------------
//Rendering
//! Draw the scene
void Renderer::draw()
{
	//compute normal matrix for light
	glm::mat3 NormalMatrix = glm::transpose(glm::inverse(glm::mat3(light.getViewMatrix()*model.getWorldMatrix())));
	//set up window vector for discontinuity, split and gather
	glm::vec2 window = glm::vec2((float)winWidth, (float)winHeight);
	glm::vec2 shadowViewport = glm::vec2((float)SHADOW_WIDTH, (float)SHADOW_HEIGHT);
	glm::vec2 ismViewport = glm::vec2(2*(float)VPL_SQRT*ISM_TILE_EDGE/3,
									  (float)VPL_SQRT*ISM_TILE_EDGE);

	if(light.hasMoved())
	{
		//---------------------------------------------------------------------------
		//Render into G-buffer - Camera POV
		//---------------------------------------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, FBOs[RSM_FBO]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		rsmShader.use();
		setUniform(rsmShader.view, light.getViewMatrix());
		setUniform(rsmShader.proj, light.getProjectionMatrix());
		setUniform(rsmShader.world, model.getWorldMatrix());
		setUniform(rsmShader.normalMat, NormalMatrix);
		setUniform(rsmShader.viewport, shadowViewport);
		//activate diffuse texture unit and set uniform
		glActiveTexture(GL_TEXTURE0);
		setUniform(rsmShader.diffuseTex, 0);
		//texture bound by model
		model.draw();
		//---------------------------------------------------------------------------
		//ISM creation
		//---------------------------------------------------------------------------
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glBindFramebuffer(GL_FRAMEBUFFER, FBOs[ISM_FBO]);
		GLenum ismRT[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
		glDrawBuffers(1, &ismRT[0]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0,0, VPL_SQRT*ISM_TILE_EDGE, VPL_SQRT*ISM_TILE_EDGE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, haltonTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, rsmWSCTex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, rsmNormalTex);
		ismShader.use();
		setUniform(ismShader.vplSqrt, (int)VPL_SQRT);
		setUniform(ismShader.haltonTex, 0);
		setUniform(ismShader.wscTex, 1);
		setUniform(ismShader.normalTex, 2);
		setUniform(ismShader.world, model.getWorldMatrix());
		model.drawPointCloud();
		//---------------------------------------------------------------------------
		//Pull-Push
		//---------------------------------------------------------------------------
		//Pull
		//----
		pullShader.use();
		//change output buffer
		glDrawBuffers(1, &ismRT[1]);
		//change viewport and texcoord
		glViewport(VPL_SQRT*ISM_TILE_EDGE, VPL_SQRT*ISM_TILE_EDGE/2,
               	  (VPL_SQRT*ISM_TILE_EDGE)/2, (VPL_SQRT*ISM_TILE_EDGE)/2);
		setQuadTexCoord(0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ISMTextureLevel0);
		setUniform(pullShader.inputTex, 0);
		setUniform(pullShader.viewport, shadowViewport);
		drawFullscreenQuad();
		//----
		glDrawBuffers(1, &ismRT[0]);
		glViewport(VPL_SQRT*ISM_TILE_EDGE, VPL_SQRT*ISM_TILE_EDGE/4,
              	  (VPL_SQRT*ISM_TILE_EDGE)/4, (VPL_SQRT*ISM_TILE_EDGE)/4);
		setQuadTexCoord(1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ISMTextureLevel1);
		setUniform(pullShader.inputTex, 1);
		drawFullscreenQuad();
		//----
		//Push
		//----
		pushShader.use();
		glViewport(VPL_SQRT*ISM_TILE_EDGE, VPL_SQRT*ISM_TILE_EDGE/2,
               	  (VPL_SQRT*ISM_TILE_EDGE)/2, (VPL_SQRT*ISM_TILE_EDGE)/2);
		glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, ISMTextureLevel0);
    	glActiveTexture(GL_TEXTURE1);
    	glBindTexture(GL_TEXTURE_2D, ISMTextureLevel1);
    	setUniform(pushShader.offset, (float)2.0/3);
    	setUniform(pushShader.lvlSwitch, 1);
    	setUniform(pushShader.viewport, shadowViewport);
    	setUniform(pushShader.coarserTex, 0);
    	setUniform(pushShader.currentTex, 1);
    	drawFullscreenQuad();
    	//----
    	glDrawBuffers(1, &ismRT[1]);
    	glViewport(0,0, VPL_SQRT*ISM_TILE_EDGE, VPL_SQRT*ISM_TILE_EDGE);
    	setQuadTexCoord(0);
    	glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, ISMTextureLevel0);
    	glActiveTexture(GL_TEXTURE1);
    	glBindTexture(GL_TEXTURE_2D, ISMTextureLevel0);
    	setUniform(pushShader.lvlSwitch, 0);
    	drawFullscreenQuad();
		//reset quad and clear color
		setQuadTexCoord(-1);
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glViewport(0, 0, winWidth, winHeight);
	}
	if(camera.hasMoved())	//recompute camera POV buffers only if necessary
	{
		//-------------------------------------------------------------------------------
		//Render into G-buffer - Camera POV
		//-------------------------------------------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, FBOs[RENDER_FBO]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		NormalMatrix = glm::transpose(glm::inverse(glm::mat3(camera.getViewMatrix()*model.getWorldMatrix())));
		renderShader.use();
		setUniform(renderShader.view, camera.getViewMatrix());
		setUniform(renderShader.proj, camera.getProjectionMatrix());
		setUniform(renderShader.world, model.getWorldMatrix());
		setUniform(renderShader.normalMat, NormalMatrix);
		//activate diffuse texture unit and set uniform
		glActiveTexture(GL_TEXTURE0);
		setUniform(renderShader.diffuseTex, 0);
		//texture bound by model
		model.draw();
		//-------------------------------------------------------------------------------
		//Create Discontinuity Buffer
		//-------------------------------------------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, FBOs[DISCONTINUITY_FBO]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderDepthTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, renderNormalTex);
		discontinuityShader.use();
		setUniform(discontinuityShader.depthTex, 0);
		setUniform(discontinuityShader.normalTex, 1);
		setUniform(discontinuityShader.distThresh, (float)0.5);	//0.25
	  	setUniform(discontinuityShader.normThresh, (float)0.75); // 0.5
		setUniform(discontinuityShader.window, window);
		drawFullscreenQuad();
		//-------------------------------------------------------------------------------
		//G-BUFFER SPLITTING
		//-------------------------------------------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, FBOs[SPLIT_FBO]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderWSCTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, renderNormalTex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, renderColorTex);
		splitShader.use();
		setUniform(splitShader.origWSCTex, 0);
		setUniform(splitShader.origNormTex, 1);
		setUniform(splitShader.origColTex, 2);
		setUniform(splitShader.blocksX, GBUF_BLOCKS_X);
		setUniform(splitShader.blocksY, GBUF_BLOCKS_Y);
		setUniform(splitShader.window, window);
		drawFullscreenQuad();
	}
	//only if any change
	if(light.hasMoved() || camera.hasMoved())
	{
		//-------------------------------------------------------------------------------
		//DEFERRED SHADING
		//-------------------------------------------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, FBOs[DEFERRED_FBO]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 DVP = Light::biasMatrix * light.getProjectionMatrix() * light.getViewMatrix();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, splitWSCTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, splitNormalTex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, splitColorTex);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, rsmDepthTex);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, ISMTextureLevel1);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, rsmWSCTex);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, rsmNormalTex);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, rsmColorTex);
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, haltonTex);
		deferredShader.use();
		setUniform(deferredShader.wscTex, 0);
		setUniform(deferredShader.normalTex, 1);
		setUniform(deferredShader.colorTex, 2);
		setUniform(deferredShader.shadowTex, 3);
		setUniform(deferredShader.ismTex, 4);
		setUniform(deferredShader.rsmWSCTex, 5);
		setUniform(deferredShader.rsmNormalTex, 6);
		setUniform(deferredShader.rsmColorTex, 7);
		setUniform(deferredShader.haltonTex, 8);
		setUniform(deferredShader.ismViewport, ismViewport);
		setUniform(deferredShader.vplSqrt, (int)VPL_SQRT);
		setUniform(deferredShader.view, camera.getViewMatrix());
		setUniform(deferredShader.biasDVP, DVP);
		setUniform(deferredShader.cameraPos, camera.getOrigin());
		setUniform(deferredShader.lightPos, light.getOrigin());
		setUniform(deferredShader.window, window);
		setUniform(deferredShader.blocksX, GBUF_BLOCKS_X);
		setUniform(deferredShader.blocksY, GBUF_BLOCKS_Y);
		setUniform(deferredShader.mode, mode);
		drawFullscreenQuad();
		//-------------------------------------------------------------------------------
		//G-BUFFER GATHERING
		//-------------------------------------------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, FBOs[GATHER_FBO]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, deferredColTex);
		gatherShader.use();
		setUniform(gatherShader.splitTex, 0);
		setUniform(gatherShader.window, window);
		setUniform(gatherShader.blocksX, GBUF_BLOCKS_X);
		setUniform(gatherShader.blocksY, GBUF_BLOCKS_Y);
		drawFullscreenQuad();
		//-------------------------------------------------------------------------------
		//X-BLUR
		//-------------------------------------------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, FBOs[XBLUR_FBO]);
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, discontinuityTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, renderColorTex);	//output of gathering phase
		xBlurShader.use();
		setUniform(xBlurShader.discontinuityTex, 0);
		setUniform(xBlurShader.inputTex, 1);
		setUniform(xBlurShader.window, window);
		drawFullscreenQuad();	//output in deferredColTex
		//-------------------------------------------------------------------------------
		//Y-BLUR
		//-------------------------------------------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, FBOs[YBLUR_FBO]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, deferredColTex);	//output of xBlur phase
		yBlurShader.use();
		setUniform(yBlurShader.discontinuityTex, 0);	//TEXTURE0 still holds discontinuityTex
		setUniform(yBlurShader.inputTex, 1);
		setUniform(yBlurShader.window, window);
		drawFullscreenQuad();	//output in renderColorTex
	}
	//-------------------------------------------------------------------------------
	// Show Result
	//-------------------------------------------------------------------------------
	//bind main FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*pointsShader.use();
	setUniform(pointsShader.viewMat, camera.getViewMatrix());
	setUniform(pointsShader.projMat, camera.getProjectionMatrix());
	setUniform(pointsShader.worldMat, model.getWorldMatrix());
	model.drawPointCloud();*/
	quadShader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderColorTex);
	setUniform(quadShader.tex, 0);
	//render quad
	drawFullscreenQuad();
}
//! Draw screen-aligned quad
void Renderer::drawFullscreenQuad()
{
	glBindVertexArray(quadVAO);
	glDrawElements(GL_TRIANGLES, sizeof(quad)/sizeof(**quad),
                   GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(0);
}

//----------------------------------------------------------------------------
//IO event handling
//! Handle keyboard input
void Renderer::handleKeyPressed(sf::Event & event)
{
	glm::vec3& ol = light.getOrigin();
	glm::vec3& oc = camera.getOrigin();
	
	switch(event.key.code)
	{
	case sf::Keyboard::Escape:
		running = false;
		break;
	case sf::Keyboard::Down:
		ol.z += 0.2f;
		light.move();
		break;
	case sf::Keyboard::Up:
		ol.z -= 0.2f;
		light.move();
		break;
	case sf::Keyboard::Left:
		ol.x -= 0.2f;
		light.move();
		break;
	case sf::Keyboard::Right:
		ol.x += 0.2f;
		light.move();
		break;
	case sf::Keyboard::Add:
		ol.y += 0.2f;
		light.move();
		break;
	case sf::Keyboard::Subtract:
		ol.y -= 0.2f;
		light.move();
		break;
	case sf::Keyboard::W:
		oc += camera.getDirection();//1.0f;
		camera.move();
		break;
	case sf::Keyboard::S:
		oc -= camera.getDirection();//1.0f;
		camera.move();
		break;
	case sf::Keyboard::A:
		oc -= camera.getRight();//1.0f;
		camera.move();
		break;
	case sf::Keyboard::D:
		oc += camera.getRight();//1.0f;
		camera.move();
		break;
	case sf::Keyboard::Q:
		oc.y += 1.0f;
		camera.move();
		break;
	case sf::Keyboard::E:
		oc.y -= 1.0f;
		camera.move();
		break;
	case sf::Keyboard::U:
		mode = 0;
		break;
	case sf::Keyboard::I:
		mode = 1;
		break;
	case sf::Keyboard::O:
		mode = 2;
		break;
	default:
		break;
	}
}
//! Handle mouse wheel movement
void Renderer::handleMouseWheel(sf::Event & event)
{
	//TODO: change rotation based on mouse.wheel.delta
}
//! Set texture coordinates corresponding to quad vertices
void Renderer::setQuadTexCoord(int mipMapLevel) //-1 -> default
{
  switch(mipMapLevel)
  {
    case 0:
      quadVertices[0].texcoord[0] = 0;
      quadVertices[0].texcoord[1] = 0;
      quadVertices[1].texcoord[0] = 2.0/3;
      quadVertices[1].texcoord[1] = 0.0;
      quadVertices[2].texcoord[0] = 2.0/3;
      quadVertices[2].texcoord[1] = 1.0;
      quadVertices[3].texcoord[0] = 0.0;
      quadVertices[3].texcoord[1] = 1.0;
      break;
    case 1:
      quadVertices[0].texcoord[0] = 2.0/3;
      quadVertices[0].texcoord[1] = 0.5;
      quadVertices[1].texcoord[0] = 1.0;
      quadVertices[1].texcoord[1] = 0.5;
      quadVertices[2].texcoord[0] = 1.0;
      quadVertices[2].texcoord[1] = 1.0;
      quadVertices[3].texcoord[0] = 2.0/3;
      quadVertices[3].texcoord[1] = 1.0;
      break;
    default:
      quadVertices[0].texcoord[0] = 0.0;
	  quadVertices[0].texcoord[1] = 0.0;
	  quadVertices[1].texcoord[0] = 1.0;
	  quadVertices[1].texcoord[1] = 0.0;
	  quadVertices[2].texcoord[0] = 1.0;
	  quadVertices[2].texcoord[1] = 1.0;
	  quadVertices[3].texcoord[0] = 0.0;
	  quadVertices[3].texcoord[1] = 1.0;
	  break;
  }
  //bind quad
  glBindVertexArray(quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, quadBuffers[0]);
  //change data
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices),
               quadVertices, GL_STATIC_DRAW);
  //unbind
  glBindVertexArray(0);
}