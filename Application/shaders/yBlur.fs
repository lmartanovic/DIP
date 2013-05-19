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

  float weights[2];
  weights[0] = 0.24;
  weights[1] = 0.12;

  vec4 sum = vec4(0.0);
  float valid = 0.0;
  float weight = 0.3;

  sum += texture2D(inputTex, TexCoord) * 0.3;
  valid = texture2D(discontinuityTex, TexCoord).r;

  if(valid != 1.0)
  {
    for(int i = 1; i <= 2; i++)
    {
      valid = texture2D(discontinuityTex, vec2(TexCoord.x - i*offset, TexCoord.y)).r;
      if(valid == 1.0)
        break;
      sum += texture2D(inputTex, vec2(TexCoord.x - i*offset, TexCoord.y)) * weights[i-1];
      weight += weights[i-1];
    }

    for(int i = 1; i <= 2; i++)
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

