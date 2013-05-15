#version 330

in vec2 TexCoord;

uniform float normThresh;
uniform float distThresh;
uniform vec2 window;
uniform sampler2D normalTex;
uniform sampler2D depthTex;

out vec4 FragColor;

void main()
{
  //sample offsets
  vec2 offset[4];
  offset[0] = vec2(0.0, 1.0/window.y);
  offset[1] = vec2(1.0/window.x, 1.0/window.y);
  offset[2] = vec2(0.0, 0.0);
  offset[3] = vec2(1.0/window.x, 0.0);

  //x kernel
  float weightX[4];
  weightX[0] = 1.0;
  weightX[1] = 0.0;
  weightX[2] = 0.0;
  weightX[3] = -1.0;

  //y kernel
  float weightY[4];
  weightY[0] = 0.0;
  weightY[1] = 1.0;
  weightY[2] = -1.0;
  weightY[3] = 0.0;

  float sampleDepth;
  vec3 sampleNorm;
  float diffPosX = 0.0;
  float diffPosY = 0.0;
  vec3 diffNormX = vec3(0.0);
  vec3 diffNormY = vec3(0.0);

  for(int i = 0; i < 4; i++)
  {
    sampleDepth = texture2D(depthTex, TexCoord + offset[i]).r;
    sampleNorm = texture2D(normalTex, TexCoord + offset[i]).rgb;
    diffPosX += sampleDepth * weightX[i];
    diffPosY += sampleDepth * weightY[i];
    diffNormX += sampleNorm * weightX[i];
    diffNormY += sampleNorm * weightY[i];
  }
  //normal distance
  float dnx = length(diffNormX);
  float dny = length(diffNormY);

  //gradient magnitude for both depth distance and normals
  float Gpos = sqrt(diffPosX*diffPosX + diffPosY*diffPosY);
  float Gnorm = sqrt(dnx*dnx + dny*dny);

  //thresholding
  if((Gnorm > normThresh) || (Gpos > distThresh))
    FragColor = vec4(1.0);
  else FragColor = vec4(0.0);
}
