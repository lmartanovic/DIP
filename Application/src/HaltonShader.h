#include "Shader.h"

class HaltonShader : public Shader
{
public:
	virtual void initUniforms()
	{
		vplSqrt = glGetUniformLocation(id, "vplSqrt");
	}

	GLint vplSqrt;
};
