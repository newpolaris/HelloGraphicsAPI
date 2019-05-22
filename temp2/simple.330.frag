#version 420

layout(location = 0) in vec3 color;
layout(location = 0) out vec4 fragColor;

void main ()
{
  vec4 tmpvar_1;
  tmpvar_1.w = 1.0;
  tmpvar_1.xyz = color;
  fragColor = tmpvar_1;
}

