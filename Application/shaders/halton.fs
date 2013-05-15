#version 330

uniform int vplSqrt;

layout(location=0) out vec2 Halton;

void main()
{
  //determine grid index based on fragment coordinates
  int index = (int(gl_FragCoord.x-0.5)*vplSqrt + int(gl_FragCoord.y-0.5));
  float r1 = 0;
  float r2 = 0;
  float f1 = 1.0/2;
  float f2 = 1.0/3;
  int i1 = index;
  int i2 = index;
  //compute halton number at given index for base 2
  while(i1 > 0)
  {
    r1 = r1 + f1*(i1%2);
    i1 = i1/2;
    f1 = f1/2;
  }
  //compute halton number at given index for base 3
  while(i2 > 0)
  {
    r2 = r2 + f2*(i2%3);
    i2 = i2/3;
    f2 = f2/3;
  }
  //compute halton sequence texture coordinates and store as RG value
  Halton = vec2(r1, r2);
}
