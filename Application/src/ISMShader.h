#include "Shader.h"

class ISMShader : public Shader
{
public:
	virtual void initUniforms();

	GLint xoff;
	GLint yoff;
	GLint haltonTexUni;
	GLint wscTexUni;
	GLint normalTexUni;
	GLint worldMatUni;
};

void ISMShader::initUniforms()
{
	xoff = glGetUniformLocation(id, "xoffset");
	yoff = glGetUniformLocation(id, "yoffset");
	haltonTexUni = glGetUniformLocation(id, "halton");
	wscTexUni = glGetUniformLocation(id, "wscTex");
	normalTexUni = glGetUniformLocation(id, "normalTex");
	worldMatUni = glGetUniformLocation(id, "WorldMat");
}