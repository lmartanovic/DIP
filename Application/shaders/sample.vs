#version 330

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;

uniform mat4 World;
uniform mat4 View;
uniform mat4 Proj;

out vec3 Normal;

void main()
{
  gl_Position = Proj * View * World * vec4(position, 1.0);//Proj * View * World * vec4(position, 1.0);
  Normal = normal;
}
