#include "Shader.h"

class PullShader : public Shader
{
public:
  virtual void initUniforms();

  GLint mapUni;
  GLint width;
  GLint height;
};

void PullShader::initUniforms()
{
  mapUni = glGetUniformLocation(id, "map");
  width = glGetUniformLocation(id, "width");
  height = glGetUniformLocation(id, "height");
}
