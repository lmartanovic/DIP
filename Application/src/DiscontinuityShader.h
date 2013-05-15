#include "Shader.h"

class DiscontinuityShader : public Shader
{
public:
  virtual void initUniforms()
  {
    normalTex = glGetUniformLocation(id, "normalTex");
    depthTex = glGetUniformLocation(id, "depthTex");
    distThresh = glGetUniformLocation(id, "distThresh");
    normThresh = glGetUniformLocation(id, "normThresh");
    window = glGetUniformLocation(id, "window");
  }

  GLint normalTex;
  GLint depthTex;
  GLint distThresh;
  GLint normThresh;
  GLint window;
};
