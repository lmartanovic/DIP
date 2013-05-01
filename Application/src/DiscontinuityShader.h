#include "Shader.h"

class DiscontinuityShader : public Shader
{
public:
  virtual void initUniforms();

  GLint normalTex;
  GLint depthTex;
  GLint distThresh;
  GLint normThresh;
  GLint window;
};

void DiscontinuityShader::initUniforms()
{
  normalTex = glGetUniformLocation(id, "normalTex");
  depthTex = glGetUniformLocation(id, "WSCTex");
  distThresh = glGetUniformLocation(id, "distThresh");
  normThresh = glGetUniformLocation(id, "normThresh");
  window = glGetUniformLocation(id, "window");
}
