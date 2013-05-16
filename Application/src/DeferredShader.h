#include "Shader.h"

class DeferredShader : public Shader
{
public:
	virtual void initUniforms() {
		wscTex = glGetUniformLocation(id, "wscTex");
		normalTex = glGetUniformLocation(id, "normalTex");
		colorTex = glGetUniformLocation(id, "colorTex");
		ismViewport = glGetUniformLocation(id, "ismViewport");
		vplSqrt = glGetUniformLocation(id, "vplSqrt");
		ismTex = glGetUniformLocation(id, "ismTex");
		rsmWSCTex = glGetUniformLocation(id, "rsmWSCTex");
		rsmNormalTex = glGetUniformLocation(id, "rsmNormalTex");
		rsmColorTex = glGetUniformLocation(id, "rsmColorTex");
		haltonTex = glGetUniformLocation(id, "haltonTex");
		//direct
		shadowTex = glGetUniformLocation(id, "shadowTex");
		view = glGetUniformLocation(id, "View");
		biasDVP = glGetUniformLocation(id, "BiasDVP");
		cameraPos = glGetUniformLocation(id, "cameraPos");
		lightPos = glGetUniformLocation(id, "lightPos");

		window = glGetUniformLocation(id, "window");
		blocksX = glGetUniformLocation(id, "blocksX");
		blocksY = glGetUniformLocation(id, "blocksY");
		mode = glGetUniformLocation(id, "mode");
	};

	GLint wscTex;
	GLint normalTex;
	GLint colorTex;
	GLint ismViewport;
	GLint vplSqrt;
	GLint ismTex;
	GLint rsmWSCTex;
	GLint rsmNormalTex;
	GLint rsmColorTex;
	GLint haltonTex;
	GLint shadowTex;
	GLint view;
	GLint biasDVP;
	GLint cameraPos;
	GLint lightPos;
	GLint window;
	GLint blocksX;
	GLint blocksY;
	GLint mode;
};