#version 330
//horizontal blur - x varies
in vec2 TexCoord;

uniform sampler2D inputTex;
uniform sampler2D discontinuityTex;
uniform vec2 window;

out vec4 FragColor;

void main()
{
  float offset = 1.0/window.x; //1px offset

  float weights[2];
  weights[0] = 0.24;
  weights[1] = 0.12;

  vec4 sum = vec4(0.0);
  float valid = 0.0;
  float weight = 0.3;

  //get center
  sum += texture2D(inputTex, TexCoord) * 0.3;
  //get its value in discontinuity buffer
  valid = texture2D(discontinuityTex, TexCoord).r;

  //if not on a discontinuity - 1.0 means edge
  if(valid != 1.0)
  {
    //get values up
    for(int i = 1; i <= 2; i++)
    {
      //find out if neighbor is on a discontinuity
      valid = texture2D(discontinuityTex, vec2(TexCoord.x, TexCoord.y - i*offset)).r;
      //if it is
      if(valid == 1.0)
        break;  //don't get any more in this direction
      //otherwise get it's color value and add it
      sum += texture2D(inputTex, vec2(TexCoord.x, TexCoord.y - i*offset)) * weights[i-1];
      //add up total pixel value (to normalize intensity)
      weight += weights[i-1];
    }

    //get values down and do the same
    for(int i = 1; i <= 2; i++)
    {
      valid = texture2D(discontinuityTex, vec2(TexCoord.x, TexCoord.y + i*offset)).r;
      if(valid == 1.0)
        break;
      sum += texture2D(inputTex, vec2(TexCoord.x, TexCoord.y + i*offset)) * weights[i-1];
      weight += weights[i-1];
    }
  }

  //normalize
  FragColor = sum * (1.0/weight);
}
