#include "Shader.h"

class PointsShader : public Shader
{
public:
	virtual void initUniforms()
	{
		worldMat = glGetUniformLocation(id, "WorldMat");
		viewMat = glGetUniformLocation(id, "ViewMat");
		projMat = glGetUniformLocation(id, "ProjMat");
	}

	GLint worldMat;
	GLint viewMat;
	GLint projMat;
};