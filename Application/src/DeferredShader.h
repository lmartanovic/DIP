#include "Shader.h"

class DeferredShader : public Shader
{
public:
	virtual void initUniforms() {
		wscTex = glGetUniformLocation(id, "wscTex");
		normalTex = glGetUniformLocation(id, "normalTex");
		colorTex = glGetUniformLocation(id, "colorTex");
		shadowTex = glGetUniformLocation(id, "shadowTex");
		view = glGetUniformLocation(id, "View");
		biasDVP = glGetUniformLocation(id, "BiasDVP");
		cameraPos = glGetUniformLocation(id, "cameraPos");
		lightPos = glGetUniformLocation(id, "lightPos");
	};

	GLint wscTex;
	GLint normalTex;
	GLint colorTex;
	GLint shadowTex;
	GLint view;
	GLint biasDVP;
	GLint cameraPos;
	GLint lightPos;
};