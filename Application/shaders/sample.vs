#version 330

in vec3 position;

uniform mat4 World;
uniform mat4 View;
uniform mat4 Proj;

void main()
{
  gl_Position = vec4(position, 1.0);//Proj * View * World * vec4(position, 1.0);
}
