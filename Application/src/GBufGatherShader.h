#include "Shader.h"

class GBufGatherShader : public Shader
{
public:
  virtual void initUniforms()
  {
	  blocksX = glGetUniformLocation(id, "blocksX");
	  blocksY = glGetUniformLocation(id, "blocksY");
	  window = glGetUniformLocation(id, "window");
	  splitTex = glGetUniformLocation(id, "splitTex");
	}

  GLint blocksX;
  GLint blocksY;
  GLint window;
  GLint splitTex;
};
