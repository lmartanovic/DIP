#version 330

layout(location=0) out vec4 worldCoord;
layout(location=1) out vec3 normal;
layout(location=2) out vec4 diffColor;

in vec2 TexCoord;

uniform sampler2D origColTex;
uniform sampler2D origWSCTex;
uniform sampler2D origNormTex;
uniform int blocksX;
uniform int blocksY;
uniform vec2 window;

void main()
{
  //get block size
  int xsam = int(window.x)/blocksX;
  int ysam = int(window.y)/blocksY;
  //compute texture coords
  int x = int(gl_FragCoord.x - 0.5);
  int y = int(gl_FragCoord.y - 0.5);
  int tcx = (x%xsam)*blocksX + (x/xsam);
  int tcy = (y%ysam)*blocksY + (y/ysam);
  //tex coord + correction due to integer division
  vec2 tc = vec2(tcx/window.x+1.0/(blocksX*window.x), tcy/window.y+1.0/(blocksY*window.y));
  //split
  worldCoord = texture2D(origWSCTex, tc);
  normal = texture2D(origNormTex, tc).rgb;
  diffColor = texture2D(origColTex, tc);
}