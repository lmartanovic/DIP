#version 330

in float ClipDepth;
in float Depth;

layout(location=0) out vec4 fragColor;

void main()
{
  if(ClipDepth < 0)
    discard;
  fragColor = vec4(Depth, Depth, Depth,1.0);
}
