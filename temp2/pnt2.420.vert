#version 420

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexcoord;
layout(location = 0) out vec3 color;

layout(std140) uniform transforms
{
    float uScale;
    vec3 uTranslate;
    vec4 uOrientation;
    mat4 uProject;
};

vec3 rotate_position(vec4 quat, vec3 v)
{
    return v + 2.0 * cross(quat.xyz, cross(quat.xyz, v) + quat.w * v);
}

void main()
{
    color = vNormal * 0.5 + vec3(0.5);
    gl_Position = uProject * vec4(rotate_position(uOrientation, vPosition) * uScale + uTranslate, 1.0);
}
