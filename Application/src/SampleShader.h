#ifndef SAMPLE_SHADER_H
#define SAMPLE_SHADER_H

#include "Shader.h"
class SampleShader : public Shader
{
public:
	
	virtual void initUniforms()	
	{
		world = glGetUniformLocation(id, "World");
		view = glGetUniformLocation(id, "View");
		proj = glGetUniformLocation(id, "Proj");
	};

	GLint world;
	GLint view;
	GLint proj;
};

#endif