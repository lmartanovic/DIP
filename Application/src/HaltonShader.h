#include "Shader.h"

class HaltonShader : public Shader
{
public:
	virtual void initUniforms();

	GLint haltonUni;
};

void HaltonShader::initUniforms()
{
	haltonUni = glGetUniformLocation(id, "NUM_VPLS_SQUARE");
}