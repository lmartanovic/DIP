#version 330
//horizontal blur - x varies
in vec2 TexCoord;

uniform sampler2D inputTex;
uniform sampler2D discontinuityTex;
uniform vec2 window;

out vec4 FragColor;

void main()
{
  float offset = 1.0/window.y;

  float weights[4];
  weights[0] = 0.15;
  weights[1] = 0.12;
  weights[2] = 0.09;
  weights[3] = 0.05;

  vec4 sum = vec4(0.0);
  float valid = 0.0;
  float weight = 0.16;

  sum += texture2D(inputTex, TexCoord) * 0.16;
  valid = texture2D(discontinuityTex, TexCoord).r;

  if(valid != 1.0)
  {
    for(int i = 1; i <= 4; i++)
    {
      valid = texture2D(discontinuityTex, vec2(TexCoord.x - i*offset, TexCoord.y)).r;
      if(valid == 1.0)
        break;
      sum += texture2D(inputTex, vec2(TexCoord.x - i*offset, TexCoord.y)) * weights[i-1];
      weight += weights[i-1];
    }

    for(int i = 1; i <= 4; i++)
    {
      valid = texture2D(discontinuityTex, vec2(TexCoord.x + i*offset, TexCoord.y)).r;
      if(valid == 1.0)
        break;
      sum += texture2D(inputTex, vec2(TexCoord.x + i*offset, TexCoord.y)) * weights[i-1];
      weight += weights[i-1];
    }
  }

  FragColor = sum * (1.0/weight);
}

