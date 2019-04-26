const char* vertex = STRINGIFY(

#version 120											

uniform mat4 u_MVPMatrix;
attribute vec4 a_Position;
attribute vec2 a_texCoord;
varying vec2 v_texCoord;
uniform float u_fWidth;
uniform float u_fHeight;
uniform float u_cameradist;

#ifdef SUPPORT_LIGHTING_TRANSITION
attribute vec4 a_Normal; // normal vector
uniform vec4 u_LightPosition; // light position
varying float v_LightIntensity; // light intensity
uniform float u_enableLight; // light position
uniform float u_LightCoefficient; // light coefficient
#endif // SUPPORT_LIGHTING_TRANSITION

void main()	
{
	gl_Position = a_Position;
	gl_Position.x /= u_fHeight;
	gl_Position.y /= u_fHeight;
	gl_Position.z /= u_fHeight;

#ifdef SUPPORT_LIGHTING_TRANSITION
	v_LightIntensity = 1.0;
	if(u_enableLight > 0.0)
	{
		vec4 normal = a_Normal;
		vec4 positionVec = normalize(a_Position);
		vec4 lightVec = normalize(u_LightPosition - positionVec);	
		v_LightIntensity = u_LightCoefficient * max(dot(normal, lightVec), 0.0);
	}
#endif // SUPPORT_LIGHTING_TRANSITION

	gl_Position = u_MVPMatrix * gl_Position;
	gl_Position.x *= u_fHeight/u_fWidth;
	gl_Position.x = gl_Position.x * 2.0 - u_cameradist;
	gl_Position.y = gl_Position.y * 2.0 - u_cameradist;
	v_texCoord = a_texCoord;
}

);
