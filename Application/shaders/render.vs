#version 330

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 texCoord;

uniform mat4 World;
uniform mat4 View;
uniform mat4 Proj;
uniform mat3 NormalMat;

out vec3 Normal;
out vec4 Position;
out vec2 TexCoord;

void main()
{
  gl_Position = Proj * View * World * vec4(position, 1.0);
  Position = (World * vec4(position,1.0));
  Normal = NormalMat * normal;
  TexCoord = texCoord;
}
