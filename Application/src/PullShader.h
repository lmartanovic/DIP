#include "Shader.h"

class PullShader : public Shader
{
public:
  virtual void initUniforms()
  {
	  inputTex = glGetUniformLocation(id, "inputTex");
	  viewport = glGetUniformLocation(id, "viewport");
  }	

  GLint inputTex;
  GLint viewport;
};
