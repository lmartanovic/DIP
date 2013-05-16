#version 330

in vec2 TexCoord;
//camera POV
uniform sampler2D wscTex;
uniform sampler2D normalTex;
uniform sampler2D colorTex;
//ism
uniform vec2 shadowViewport;
uniform int vplSqrt;
uniform sampler2D ismTex;
uniform sampler2D haltonTex;
//light POV
uniform sampler2D rsmWSCTex;
uniform sampler2D rsmNormalTex;
uniform sampler2D rsmColorTex;
uniform sampler2DShadow shadowTex;
uniform mat4 View;
uniform mat4 BiasDVP;
uniform vec3 cameraPos;
uniform vec3 lightPos;
//general
uniform vec2 window;
uniform int blocksX;
uniform int blocksY;
uniform int mode; 

out vec4 fragColor;

//compute VPL view matrix from its origin and normal
mat4 computeView(vec3 origin, vec3 dir)
{
  dir = normalize(origin+dir);
  vec3 right = cross(dir, vec3(0,1,0));
  right = normalize(right);
  vec3 up = cross(dir, right);
  up = normalize(up);

  mat4 v = mat4(right.x, up.x, -dir.x, 0.0,
                right.y, up.y, -dir.y, 0.0,
                right.z, up.z, -dir.z, 0.0,
                0.0, 0.0, 0.0, 1.0);

  mat4 t = mat4(1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                -origin.x, -origin.y, -origin.z, 1.0);
  return v*t;
}

void main()
{
	//get fragment world coordinates
	vec4 wpos = texture2D(wscTex, TexCoord);
	//project them into light space
	vec4 shPos = BiasDVP * wpos;
	//apply shadow acne bias
	shPos.z -= 0.005;
	//get shadow value
	float shadow = textureProj(shadowTex, shPos);

	//get material diffuse color - used for both direct and indirect
	vec4 color = texture2D(colorTex, TexCoord);
	//---------------------------------------------------------------------
	// Direct
	//---------------------------------------------------------------------
	vec4 dirColor = vec4(0.0);
	if(shadow != 0.0){	//if not in shadow
		vec3 L = normalize(lightPos - wpos.xyz);	//light vector
		vec3 N = texture2D(normalTex, TexCoord).xyz;//normal
		float cosTheta = max(0.0, dot(N,L));		//face orientation
		vec4 diffuse = color*cosTheta;				//diffuse component

		vec4 esPos = View * wpos;					//fragment position in eye space
		vec3 Eye = normalize(cameraPos - esPos.xyz);//eye vector
		vec3 R = reflect(-L, N);					//reflection from normal
		float cosAlpha = max(0.0, dot(Eye,R));
		vec4 specular = vec4(1.0) * pow(cosAlpha, 15.0);	//specular component

		dirColor = diffuse + specular;				//direct illumination
	}

	//--------------------------------------------------------------------
	// Indirect
	//--------------------------------------------------------------------
	//compute texture coordinates for ISM cell
	vec2 ismTC = TexCoord;
	ismTC.x *=  2.0/3.0;
	ismTC /= vplSqrt; //in one cell
	//compute which splitting cell we are in
	int x = int(gl_FragCoord.x - 0.5);
	int y = int(gl_FragCoord.y - 0.5);
	int blockWidth = int(window.x)/blocksX;
	int blockHeight = int(window.y)/blocksY;
	int xoffset = x/blockWidth;
	int yoffset = y/blockHeight;
	//get halton row number
	int rowNum = yoffset*blocksX + xoffset;
	//shift to correct ISM row according to grid position
	ismTC.y += rowNum*(1.0/vplSqrt);
	//sampling and projection
	float yHalton = rowNum*(1.0/vplSqrt);
	float xHalton;
	vec4 Position;
	float Depth, ClipDepth;
	vec2 mapCoord;
	vec3 VPLpos;
	vec3 VPLnorm;
	vec4 VPLcol;
 	float ISMdepth;
    int IsmS = 0;
    mat4 lightView;
    float near = 0.01;
    float far = 20;

    vec4 indirColor = vec4(0.0);
    for(int i = 0; i < vplSqrt; i++)
	{
		//move to neighbour cell in row
		vec2 ttcc = vec2(ismTC.x + i*(2.0/(3.0*vplSqrt)), ismTC.y);
		//move in halton row
		xHalton = i*(1.0/vplSqrt);
		mapCoord = texture2D(haltonTex, vec2(xHalton, yHalton)).rg; //getVPL tex coords
		VPLpos = texture2D(rsmWSCTex, mapCoord).rgb; //get position
		VPLnorm = texture2D(rsmNormalTex, mapCoord).rgb; //get normal
		//compute view matrix
		lightView = computeView(VPLpos.xyz, VPLnorm.xyz);
		//move fragment into light space
		Position = lightView * wpos;
		//compute projection
		Position /= Position.w;

		float l = length(Position.xyz);
		Position /= l;

		ClipDepth = Position.z;

		Position.x /= Position.z + 1.0;
		Position.y /= Position.z + 1.0;

		Position.z = (l-near)/(far - near);

		ISMdepth = texture2D(ismTex, ttcc).r;//get ISM value
		//compare fragment depth in paraboloid space with ISM value
		if(ISMdepth > Position.z)	//visible
		{
		  //sample VPL color
		  VPLcol = texture2D(rsmColorTex, mapCoord);
		  //add indirect lighting contribution
		  indirColor += VPLcol*(1.0/vplSqrt)*0.5;//*color
		}
	}
	//final composition
  	if(mode == 0) fragColor = indirColor;
  	else if(mode == 1) fragColor = dirColor;
  	else fragColor = indirColor + dirColor;
}