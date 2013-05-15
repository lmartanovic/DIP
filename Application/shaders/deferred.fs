#version 330

in vec2 TexCoord;

uniform sampler2D wscTex;
uniform sampler2D normalTex;
uniform sampler2D colorTex;
uniform sampler2DShadow shadowTex;
uniform mat4 View;
uniform mat4 BiasDVP;
uniform vec3 cameraPos;
uniform vec3 lightPos;

out vec4 fragColor;

void main()
{
	vec4 wpos = texture2D(wscTex, TexCoord);
	vec4 shPos = BiasDVP * wpos;

	shPos.z -= 0.005;
	
	float shadow = textureProj(shadowTex, shPos);
	if(shadow == 0.0) fragColor = vec4(0.0);
	else{
		vec4 color = texture2D(colorTex, TexCoord);
		vec3 L = normalize(lightPos - wpos.xyz);
		vec3 N = texture2D(normalTex, TexCoord).xyz;
		float cosTheta = max(0.0, dot(N,L));
		vec4 diffuse = color*cosTheta;

		vec4 esPos = View * wpos;
		vec3 Eye = normalize(cameraPos - esPos.xyz);
		vec3 R = reflect(-L, N);
		float cosAlpha = max(0.0, dot(Eye,R));
		vec4 specular = vec4(1.0) * pow(cosAlpha, 15.0);


		fragColor = diffuse + specular;
	}
}