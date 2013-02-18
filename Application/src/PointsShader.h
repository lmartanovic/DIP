#include "Shader.h"

class PointsShader : public Shader
{
public:
	virtual void initUniforms();

	GLint worldMat;
	GLint viewMat;
	GLint projMat;
};

void PointsShader::initUniforms()
{
	worldMat = glGetUniformLocation(id, "WorldMat");
	viewMat = glGetUniformLocation(id, "ViewMat");
	projMat = glGetUniformLocation(id, "ProjMat");
}