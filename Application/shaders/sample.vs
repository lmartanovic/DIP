#version 330

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;

uniform mat4 World;
uniform mat4 View;
uniform mat4 Proj;
uniform mat3 NormalMat;
uniform vec3 CameraPos;

out vec3 Normal;
out vec3 Position;
out vec3 EyeDir;

void main()
{
  vec4 eyePos = View * World * vec4(position, 1.0);
  EyeDir = CameraPos - eyePos.xyz;
  gl_Position = Proj * View * World * vec4(position, 1.0);
  Position = (World * vec4(position,1.0)).xyz;
  Normal = NormalMat * normal;
}
