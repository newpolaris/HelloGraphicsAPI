const char* fragment = STRINGIFY(120, 

uniform mat4 u_MVPMatrix;
attribute vec4 a_Position;
attribute vec2 a_texCoord;
varying vec2 v_texCoord;
uniform float u_fWidth;
uniform float u_fHeight;
uniform float u_cameradist;

void main()
{
	gl_Position = a_Position;
	gl_Position.x /= u_fHeight;
	gl_Position.y /= u_fHeight;
	gl_Position.z /= u_fHeight;
	gl_Position = u_MVPMatrix * gl_Position;
	gl_Position.x *= u_fHeight/u_fWidth;
	gl_Position.x = gl_Position.x * 2.0 - u_cameradist;
	gl_Position.y = gl_Position.y * 2.0 - u_cameradist;
	v_texCoord = a_texCoord;
}

);
