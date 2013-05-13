#include "Shader.h"

class QuadShader : public Shader
{
public:
	virtual void initUniforms() {
		tex = glGetUniformLocation(id, "tex");
	};

	GLint tex;
};