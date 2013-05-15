#version 330

in vec2 TexCoord;

uniform int blocksX;
uniform int blocksY;
uniform vec2 window;
uniform sampler2D splitTex;

out vec4 FragColor;

void main()
{
  int x = int(gl_FragCoord.x - 0.5);
  int y = int(gl_FragCoord.y - 0.5);
  int blockWidth = int(window.x)/blocksX;//ako uniformnu?
  int blockHeight = int(window.y)/blocksY;
  x = blockWidth*(x%blocksX) + x/blocksX;
  y = blockHeight*(y%blocksY) + y/blocksY;

  //tex coord + correction due to integer division
  vec2 tc = vec2(x/window.x+1.0/(blocksX*window.x), y/window.y+1.0/(blocksY*window.y));
  FragColor = texture2D(splitTex, tc);
}
