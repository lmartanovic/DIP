#include "Renderer.h"

Renderer::Renderer()
: running(true),
  winHeight(600),
  winWidth(800)
{}

void Renderer::init()
{
	initSFML();
	initOpenGL();
	initCamera();
	//more inits
}

//----------------------------------------------------------------------------
//MAIN LOOP
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
				//TODO: Camera - rebuild projection matrix
				break;
				//keyboard
			case sf::Event::KeyPressed:
				handleKeyPressed(event);
				break;
				//mouse
			case sf::Event::MouseWheelMoved:
				handleMouseWheel(event);
				break;
				//unhandled
			default:
				break;
			}
		}//pollEvent

		//TODO: drawing
		window.display();
	}//main loop
}

//----------------------------------------------------------------------------
//Inits
//SFML initialisation
void Renderer::initCamera()
{
	camera.create();	//create default camera
}

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
}

//GLEW and quad initialisation
void Renderer::initOpenGL()
{
	glewExperimental = GL_TRUE;
	glewInit();

	//TODO: make quad
}

//----------------------------------------------------------------------------
//IO event handling
//Keyboard
void Renderer::handleKeyPressed(sf::Event & event)
{
	switch(event.key.code)
	{
	case sf::Keyboard::Escape:
		running = false;
		break;
	default:
		break;
	}
}

//Mouse
void Renderer::handleMouseWheel(sf::Event & event)
{
	//TODO: change rotation based on mouse.wheel.delta
}