#version 330

in vec3 Position;
in vec3 Normal;
in vec3 EyeDir;

out vec4 fragColor;

void main()
{
	vec3 N = normalize(Normal);
	vec3 L = normalize(vec3(150.0, 150.0, 0.0) - Position);
	float angle = max(dot(N,L), 0.0);

	vec3 Eye = normalize(EyeDir);
	vec3 R = reflect(-L, N);
	float angle2 = max(0.0, dot(Eye,R));
	float shininess = 15.0;

	vec4 diffuse = vec4(1.0, 0.0, 0.0, 1.0)*angle;
	vec4 specular = pow(angle2, shininess) * vec4(1.0);
	vec4 ambient = vec4(0.05, 0.0, 0.0, 1.0);
	fragColor = diffuse + specular;// + ambient;
}
