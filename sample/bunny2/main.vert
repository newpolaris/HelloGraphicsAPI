#version 110

attribute vec3 vPosition;
attribute vec3 vNormal;
attribute vec2 vTexcoord;

varying vec3 color;

uniform float uScale;
uniform vec3 uTranslate;
uniform vec4 uOrientation;
uniform mat4 uProject;

vec3 rotate_position(vec4 quat, vec3 v)
{
    return v + 2.0 * cross(quat.xyz, cross(quat.xyz, v) + quat.w * v);
}

void main()
{
    color = vNormal * 0.5 + vec3(0.5);
    gl_Position = uProject * vec4(rotate_position(uOrientation, vPosition) * uScale + uTranslate, 1.0);
}
