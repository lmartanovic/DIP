#version 330

layout(location=0) in vec3 position;

uniform mat4 WorldMat;
uniform mat4 ViewMat;
uniform mat4 ProjMat;

void main()
{
  gl_Position = ProjMat * ViewMat * WorldMat * vec4(position, 1.0);
}
