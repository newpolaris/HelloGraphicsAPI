#version 110

uniform mat4 MVP;
attribute vec3 vPosition;
attribute vec3 vNormal;
attribute vec2 vTexcoord;

varying vec3 color;

void main()
{
    gl_Position = vec4(vPosition/3.0 + vec3(0.0, 0.0, 0.0), 1.0);
    color = vec3(vNormal * 0.5 + vec3(0.5));
}
