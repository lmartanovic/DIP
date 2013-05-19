#version 330

layout(location=0) in vec3 position;
layout(location=1) in int VPLindex;

uniform int vplSqrt;           //VPLs per edge
uniform sampler2D halton;     //halton sequence
uniform sampler2D wscTex;     //world-space coordinate texture
uniform sampler2D normalTex;  //normal in world-space texture
uniform mat4 World;

out float ClipDepth;
out float Depth;

//create VPL view matrix
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

//=============================================================================
void main()
{
  //lp sa ziska z haltona a RSM
  int xoffset = VPLindex % vplSqrt;
  int yoffset = VPLindex / vplSqrt;

  float dividor = 1.0/float(vplSqrt);
  //look into halton  +0.5 for pixel precision
  vec4 mapCoord = texture2D(halton, vec2((xoffset+0.5)*dividor, (yoffset+0.5)*dividor));
  //light position from wsctex on halton coords
  vec4 VPLpos = texture2D(wscTex, mapCoord.xy);
  //VPL normal
  vec4 VPLnormal = texture2D(normalTex, mapCoord.xy);

  vec4 Position;
  //near and far planes of VPL
  float near = 0.01;
  float far = 20;
  //compute light view matrix
  mat4 lightView = computeView(VPLpos.xyz, VPLnormal.xyz);
  //transform the point int light view
  vec4 worldPos = World * vec4(position, 1.0);
  Position = lightView * worldPos;
  //compute projection
  Position /= Position.w;

  float l = length(Position.xyz);
  Position /= l;

  ClipDepth = Position.z;

  Position.x /= Position.z + 1.0;
  Position.y /= Position.z + 1.0;

  Position.z = (l-near)/(far - near);
  Position.w = 1.0;

  Depth = Position.z;

  //put in correct place in grid
  gl_Position = Position;
  gl_Position.xy *= dividor;
  gl_Position.x += (-1.0 + (2*xoffset+1)*dividor);
  gl_Position.y += (-1.0 + (2*yoffset+1)*dividor);
  //change splat size
  gl_PointSize = 15 - pow(distance(VPLpos, worldPos),2);
  if(gl_PointSize < 5) gl_PointSize = 5;
}
