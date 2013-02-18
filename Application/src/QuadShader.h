#include "Shader.h"

class QuadShader : public Shader
{
public:
	virtual void initUniforms();

	GLint shadowMapUni;
};

void QuadShader::initUniforms()
{
	shadowMapUni = glGetUniformLocation(id, "shadowMap");
}