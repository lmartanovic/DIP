#version 330

in vec2 TexCoord;

uniform sampler2D inputTex;
uniform vec2 viewport;

out vec4 mmap;

void main()
{
  //init surrounding 4 pixels offset
  vec2 offset[4];
  offset[0] = vec2(-0.5/viewport.x, 0.5/viewport.y);
  offset[1] = vec2(-0.5/viewport.x, -0.5/viewport.y);
  offset[2] = vec2(0.5/viewport.x, 0.5/viewport.y);
  offset[3] = vec2(0.5/viewport.x, -0.5/viewport.y);

  float finerVal = 0; //output value
  int valids = 0;     //number of valid contributors for weighting
  vec4 sample;        //coarse sample
  //get 4 surrounding values
  for(int i = 0; i < 4; i++)
  {
    //get value
    sample = texture2D(inputTex, TexCoord + offset[i]);
    //if valid
    if(sample.a == 1.0)
    {
      finerVal += sample.r;
      valids++;
    }
  }

  //if at least one was valid
  if(valids > 0)
  {
    finerVal /= valids;
    mmap = vec4(vec3(finerVal), 1.0);
  }else{  //else invalidate
    mmap = vec4(1.0, 1.0, 1.0, 0.0);
  }
}
