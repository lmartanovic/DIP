#include "Shader.h"

class ISMShader : public Shader
{
public:
	virtual void initUniforms();

	GLint VPLSQRT;
	GLint haltonTexUni;
	GLint wscTexUni;
	GLint normalTexUni;
	GLint worldMatUni;
};

void ISMShader::initUniforms()
{
	VPLSQRT = glGetUniformLocation(id, "VPLSQRT");
	haltonTexUni = glGetUniformLocation(id, "halton");
	wscTexUni = glGetUniformLocation(id, "wscTex");
	normalTexUni = glGetUniformLocation(id, "normalTex");
	worldMatUni = glGetUniformLocation(id, "WorldMat");
}
