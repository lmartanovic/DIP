#version 330

in vec2 TexCoord;

uniform bool lvl;
uniform float offset;
uniform sampler2D currentTex; //1
uniform sampler2D coarserTex; //0
uniform vec2 viewport;

out vec4 outColor;

//give corresponding weight to pixel
float weigh(float valid, int coord)
{
  if(valid == 0.0) return valid;
  else{
    if(coord == 0) return 9.0/16;
    else if(coord == 1 || coord == 3) return 3.0/16;
    else return 1.0/16;
  }
}

void main()
{
  float weights[4];
  vec4 samples[4];
  vec2 offsets[4];
  offsets[0] = vec2(0.0, 0.0);
  offsets[1] = vec2(0.0, 0.5/viewport.y);
  offsets[2] = vec2(-0.5/viewport.x, 0.5/viewport.y);
  offsets[3] = vec2(-0.5/viewport.x, 0.0);

  if(lvl)
    outColor = texture2D(currentTex, TexCoord);
  else outColor = texture2D(coarserTex, TexCoord);
  //if invalid pixel
  if(outColor.a == 0.0)
  {
    //compute coordinets of corresponding pixel in coarser level
    vec2 tc;
    if(lvl) //lvl1
    {
      tc.x = (TexCoord.x-offset)/2+offset;//+0.5/width;
      tc.y = TexCoord.y/2;
    }else{  //lvl0
      tc.x = TexCoord.x/2+offset;//+0.5/width;
      tc.y = TexCoord.y/2+0.5;
    }
    //get coarser level samples
    for(int i = 0; i < 4; i++)
    {
      samples[i] = texture2D(coarserTex, tc+offsets[i]);
      weights[i] = weigh(samples[i].a, i);
    }

    //adjust weights - only valid contribute
    float w = 0;
    for(int i = 0; i < 4; i++)
    {
      w += weights[i];
    }

    float outVal = 0;
    //if at least one valid
    if(w > 0)
    {
      //add up weighted values
      for(int i = 0; i < 4; i++)
      {
        weights[i] /= w;
        outVal += samples[i].r*weights[i];
      }

      outColor = vec4(vec3(outVal), 1.0);
    }
  }//if outColor.a
}
