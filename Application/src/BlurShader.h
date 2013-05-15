#include "Shader.h"

class BlurShader : public Shader
{
public:
	virtual void initUniforms()
	{
	  inputTex = glGetUniformLocation(id, "inputTex");
	  discontinuityTex = glGetUniformLocation(id, "discontinuityTex");
	  window = glGetUniformLocation(id, "window");
	}

	GLint inputTex;
	GLint discontinuityTex;
	GLint window;
};
