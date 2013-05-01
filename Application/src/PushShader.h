#include "Shader.h"

class PushShader : public Shader
{
public:
  virtual void initUniforms();

  GLint current;
  GLint coarser;
  GLint lvlSwitch;
  GLint offset;
  GLint width;
  GLint height;
};

void PushShader::initUniforms()
{
  current = glGetUniformLocation(id, "currentTex");
  coarser = glGetUniformLocation(id, "coarserTex");
  offset = glGetUniformLocation(id, "offset");
  lvlSwitch = glGetUniformLocation(id, "lvl");
  width = glGetUniformLocation(id, "width");
  height = glGetUniformLocation(id, "height");
}
