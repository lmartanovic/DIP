#include "Shader.h"

class PushShader : public Shader
{
public:
  virtual void initUniforms()
  {
    currentTex = glGetUniformLocation(id, "currentTex");
    coarserTex = glGetUniformLocation(id, "coarserTex");
    offset = glGetUniformLocation(id, "offset");
    lvlSwitch = glGetUniformLocation(id, "lvl");
    viewport = glGetUniformLocation(id, "viewport");
  }

  GLint currentTex;
  GLint coarserTex;
  GLint offset;
  GLint lvlSwitch;
  GLint viewport;
};
