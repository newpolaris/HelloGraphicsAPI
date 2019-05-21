#version 140
in vec3 vPosition;
in vec3 vNormal;
out vec3 color;
uniform float uScale;
uniform vec3 uTranslate;
uniform vec4 uOrientation;
uniform mat4 uProject;
void main ()
{
  color = ((vNormal * 0.5) + vec3(0.5, 0.5, 0.5));
  vec3 b_1;
  b_1 = (((uOrientation.yzx * vPosition.zxy) - (uOrientation.zxy * vPosition.yzx)) + (uOrientation.w * vPosition));
  vec4 tmpvar_2;
  tmpvar_2.w = 1.0;
  tmpvar_2.xyz = (((vPosition + 
    (2.0 * ((uOrientation.yzx * b_1.zxy) - (uOrientation.zxy * b_1.yzx)))
  ) * uScale) + uTranslate);
  gl_Position = (uProject * tmpvar_2);
}


