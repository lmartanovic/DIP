#include "Shader.h"

class BlurShader : public Shader
{
public:
  virtual void initUniforms();

  GLint composedTex;
  GLint discontinuityTex;
  GLint winDim;
};

void BlurShader::initUniforms()
{
  composedTex = glGetUniformLocation(id, "composedTex");
  discontinuityTex = glGetUniformLocation(id, "discontinuityTex");
  winDim = glGetUniformLocation(id, "winDim");
}
