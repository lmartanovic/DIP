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

#define WIN_HEIGHT 600
#define WIN_WIDTH 800
#define GBUF_BLOCKS_X 5
#define GBUF_BLOCKS_Y 5
#define ISM_TILE_EDGE 256	//pixels per ISM tile

#define NUM_FBOS 10
#define RENDER_FBO 0
#define DEFERRED_FBO 1
#define RSM_FBO 2
#define HALTON_FBO 3
#define SPLIT_FBO 4
#define GATHER_FBO 5
#define DISCONTINUITY_FBO 6
#define XBLUR_FBO 7
#define YBLUR_FBO 8
#define ISM_FBO 9

class Renderer
{
public:
	Renderer();
	void init(char* modelFile);
	void run();
private:
	void draw();
	void drawFullscreenQuad();
	void handleKeyPressed(sf::Event & event);
	void handleMouseWheel(sf::Event & event);
	void initCamera();
	void initFramebuffers();
	void initGeometry(char* modelFile);
	void initHalton();
	void initLight();
	void initOpenGL();
	void initQuad();
	void initSFML();
	void initShaders();
	void setQuadTexCoord(int mipMapLevel);

	bool running;
	int winHeight;
	int winWidth;
	sf::RenderWindow window;

	Camera camera;
	Light light;
	Model model;
	//TODO: nahradit zoznamov vsetkych shaderov
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
	//FBO array
	GLuint FBOs[NUM_FBOS];
	//render textures
	GLuint renderDepthTex, renderWSCTex, renderNormalTex, renderColorTex;
	GLuint deferredColTex;
	GLuint rsmDepthTex, rsmWSCTex, rsmNormalTex, rsmColorTex;
	GLuint haltonDepthTex, haltonTex;
	GLuint splitWSCTex, splitNormalTex, splitColorTex;
	GLuint discontinuityDepthTex, discontinuityTex;
	GLuint ISMTextureLevel0, ISMTextureLevel1;
	//fullscreen quad rendering
	GLuint quadVAO;
	GLuint quadBuffers[2];
	//animation
	float ry;
};
#endif