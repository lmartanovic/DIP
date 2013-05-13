#ifndef RENDER_SHADER_H
#define RENDER_SHADER_H

#include "Shader.h"
class RenderShader : public Shader
{
public:
	
	virtual void initUniforms()	
	{
		world = glGetUniformLocation(id, "World");
		view = glGetUniformLocation(id, "View");
		proj = glGetUniformLocation(id, "Proj");
		normalMat = glGetUniformLocation(id, "NormalMat");
	};

	GLint world;
	GLint view;
	GLint proj;
	GLint cameraPos;
	GLint normalMat;
};

#endif