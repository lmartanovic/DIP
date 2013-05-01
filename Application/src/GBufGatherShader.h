#include "Shader.h"

class GBufGatherShader : public Shader
{
public:
  virtual void initUniforms();

  GLint blocksX;
  GLint blocksY;
  GLint window;
  GLint splitTex;
};

void GBufGatherShader::initUniforms()
{
  blocksX = glGetUniformLocation(id, "blocksX");
  blocksY = glGetUniformLocation(id, "blocksY");
  window = glGetUniformLocation(id, "window");
  splitTex = glGetUniformLocation(id, "splitTex");
}
