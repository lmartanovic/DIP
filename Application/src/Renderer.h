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
#include "Model.h"
#include "QuadShader.h"
#include "RenderShader.h"

#define WIN_HEIGHT 600
#define WIN_WIDTH 800

#define NUM_FBOS 1
#define RENDER_FBO 0

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
	void initOpenGL();
	void initQuad();
	void initSFML();
	void initShaders();

	bool running;
	int winHeight;
	int winWidth;
	sf::RenderWindow window;

	Camera camera;
	//TODO: nahradit za zoznam modelov
	//Geometry teapot;
	Model model;
	//TODO: nahradit zoznamov vsetkych shaderov
	QuadShader quadShader;
	RenderShader renderShader;
	//FBO array
	GLuint FBOs[NUM_FBOS];
	//render textures
	GLuint renderDepthTex, renderWSCTex, renderNormalTex, renderColorTex;
	//fullscreen quad rendering
	GLuint quadVAO;
	GLuint quadBuffers[2];
	//animation
	float ry;
};
#endif