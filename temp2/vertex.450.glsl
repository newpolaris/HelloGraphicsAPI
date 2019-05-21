#version 450

layout (location = POSITION) in vec3 aPos;
layout (location = COLOR0) in vec4 aColor;
layout (location = TEXCOORD0) in vec2 aCoord;

layout (std140) uniform matrices
{
    mat4 projection;
    mat4 view;
    mat4 model;
};

layout (location = COLOR0) out flat vec4 outColor;
layout (location = TEXCOORD0) out vec2 outCoord;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    outColor = aColor;
    outCoord = aCoord;
}  

