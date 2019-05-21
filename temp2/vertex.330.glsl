#version 330
#ifdef GL_ARB_shading_language_420pack
#extension GL_ARB_shading_language_420pack : require
#endif

uniform vec4 uOrientation;
uniform float uScale;
uniform vec3 uTranslate;
uniform mat4 uProject;

out vec3 color;
layout(location = 1) in vec3 vNormal;
layout(location = 0) in vec3 vPosition;

void main()
{
    color = (vNormal * 0.5) + vec3(0.5);
    vec3 b_1 = ((uOrientation.yzx * vPosition.zxy) - (uOrientation.zxy * vPosition.yzx)) + (vPosition * uOrientation.w);
    vec4 tmpvar_2;
    tmpvar_2.w = 1.0;
    vec3 _68 = ((vPosition + (((uOrientation.yzx * b_1.zxy) - (uOrientation.zxy * b_1.yzx)) * 2.0)) * uScale) + uTranslate;
    tmpvar_2 = vec4(_68.x, _68.y, _68.z, tmpvar_2.w);
    gl_Position = uProject * tmpvar_2;
}


