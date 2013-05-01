#include "Shader.h"

class DeferredPhongShader : public Shader
{
public:
  virtual void initUniforms();

  GLint lightPos;
  GLint esPosition;
  GLint normal;
  GLint diffColor;
  GLint depthMap;
  GLint invVPMat;
  GLint depthMVP;
  //indirect
  GLint ismDim;
  GLint vplsPerEdge;
  GLint ISM;
  GLint RSMnormal;
  GLint RSMpos;
  GLint RSMflux;
  GLint halton;
  //how the window is set up and split
  GLint window;
  GLint blocksX;
  GLint blocksY;
  //draw mode
  GLint mode;
};

void DeferredPhongShader::initUniforms()
{
  lightPos = glGetUniformLocation(id, "lightPos");
  esPosition = glGetUniformLocation(id, "esPosition");
  normal = glGetUniformLocation(id, "normal");
  diffColor = glGetUniformLocation(id, "diffColor");
  depthMap = glGetUniformLocation(id, "depthMap");
  invVPMat = glGetUniformLocation(id, "invViewProj");
  depthMVP = glGetUniformLocation(id, "depthMVP");
  //indirect
  ismDim = glGetUniformLocation(id, "ismDim");
  vplsPerEdge = glGetUniformLocation(id, "vplsPerEdge");
  ISM = glGetUniformLocation(id, "ISM");
  RSMnormal = glGetUniformLocation(id, "RSMnormal");
  RSMpos = glGetUniformLocation(id, "RSMpos");
  RSMflux = glGetUniformLocation(id, "RSMflux");
  halton = glGetUniformLocation(id, "halton");
  //how the window is set up and split
  window = glGetUniformLocation(id, "window");
  blocksX = glGetUniformLocation(id, "blocksX");
  blocksY = glGetUniformLocation(id, "blocksY");
  //draw mode
  mode = glGetUniformLocation(id, "mode");
}
