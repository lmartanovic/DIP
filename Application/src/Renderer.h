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
#include "Geometry.h"
#include "SampleShader.h"

class Renderer
{
public:
	Renderer();
	void init();
	void run();
private:
	void draw();
	void handleKeyPressed(sf::Event & event);
	void handleMouseWheel(sf::Event & event);
	void initCamera();
	void initGeometry();
	void initOpenGL();
	void initSFML();
	void initShaders();

	bool running;
	int winHeight;
	int winWidth;
	sf::RenderWindow window;

	Camera camera;
	//TODO: nahradit za zoznam modelov
	Geometry teapot;
	//TODO: nahradit zoznamov vsetkych shaderov
	SampleShader sampleShader;
};

#endif