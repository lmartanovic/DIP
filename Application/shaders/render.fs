#version 330

in vec4 Position;
in vec3 Normal;

layout(location=0) out vec4 wscPosition;
layout(location=1) out vec3 normals;
layout(location=2) out vec4 diffuseColor;

void main()
{
	wscPosition = Position;
	normals = Normal;
	diffuseColor = vec4(1.0, 0.0, 0.0, 1.0);
}
