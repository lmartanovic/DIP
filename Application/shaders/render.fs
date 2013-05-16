#version 330

in vec4 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D diffuseTex;

layout(location=0) out vec4 wscPosition;
layout(location=1) out vec3 normals;
layout(location=2) out vec4 diffuseColor;

void main()
{
	wscPosition = Position;
	normals = normalize(Normal);
	diffuseColor = texture2D(diffuseTex, TexCoord);
	if(diffuseColor == vec4(0.0)) diffuseColor = vec4(1.0, 0.2, 0.0, 1.0);
}
