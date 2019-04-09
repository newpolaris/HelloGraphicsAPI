#version 110

uniform sampler2D tex;
uniform vec3 color;
varying vec2 texcoord;
void main()
{
    gl_FragColor = vec4(color * texture2D(tex, texcoord).rgb, 1.0);
}
