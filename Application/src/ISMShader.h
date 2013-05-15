#include "Shader.h"

class ISMShader : public Shader
{
public:
	virtual void initUniforms()
	{
		vplSqrt = glGetUniformLocation(id, "vplSqrt");
		haltonTex = glGetUniformLocation(id, "halton");
		wscTex = glGetUniformLocation(id, "wscTex");
		normalTex = glGetUniformLocation(id, "normalTex");
		world = glGetUniformLocation(id, "World");
	}

	GLint vplSqrt;
	GLint haltonTex;
	GLint wscTex;
	GLint normalTex;
	GLint world;
};
