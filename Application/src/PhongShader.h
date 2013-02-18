#include "Shader.h"

class PhongShader : public Shader
{
public:
	virtual void initUniforms();

	//matrices
	GLint worldUni;
	GLint viewUni;
	GLint projUni;
	GLint normMatUni;
	GLint depthMatUni;
	//material properties
	GLint ambMaterial;
	GLint diffMaterial;
	GLint specMaterial;
	GLint shininessMaterial;
	//light properties
	GLint lightAmb;
	GLint lightDiff;
	GLint lightSpec;
	GLint lightPos;
	//fog toggle
	GLint fogUni;
	//textures
	GLint depthTexUni;
	GLint wscTexUni;
	GLint haltonTexUni;
};

void PhongShader::initUniforms()
{
	worldUni = glGetUniformLocation(id, "WorldMat");
	viewUni = glGetUniformLocation(id, "ViewMat");
	projUni = glGetUniformLocation(id, "ProjMat");
	normMatUni = glGetUniformLocation(id, "NormalMat");

	ambMaterial = glGetUniformLocation(id, "ambientMat");
	diffMaterial = glGetUniformLocation(id, "diffMat");
	specMaterial = glGetUniformLocation(id, "specMat");
	shininessMaterial = glGetUniformLocation(id, "shininess");

	lightAmb = glGetUniformLocation(id, "lightAmb");
	lightDiff = glGetUniformLocation(id, "lightDiff");
	lightSpec = glGetUniformLocation(id, "lightSpec");
	lightPos = glGetUniformLocation(id, "lightPos");

	fogUni = glGetUniformLocation(id, "fog");
	depthMatUni = glGetUniformLocation(id, "depthBiasMVP");
	depthTexUni = glGetUniformLocation(id, "depthMap");
	wscTexUni = glGetUniformLocation(id, "wscMap");
	haltonTexUni = glGetUniformLocation(id, "halton");
}