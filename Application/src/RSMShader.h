#include "Shader.h"

class RSMShader : public Shader
{
public:
	virtual void initUniforms();

	GLint depthMVPUni;
	GLint depthNMUni;
	GLint depthWorldUni;
	GLint depthDiffMaterial;
};

void RSMShader::initUniforms()
{
	depthMVPUni = glGetUniformLocation(id, "depthMVP");
	depthNMUni = glGetUniformLocation(id, "normalMatrix");
	depthWorldUni = glGetUniformLocation(id, "worldMatrix");
	depthDiffMaterial = glGetUniformLocation(id, "diffuseMaterialComponent");
}
