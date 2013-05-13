#version 330

in vec2 TexCoord;

uniform sampler2D tex;

out vec4 fragColor;

void main()
{
	fragColor = texture2D(tex, TexCoord);
}