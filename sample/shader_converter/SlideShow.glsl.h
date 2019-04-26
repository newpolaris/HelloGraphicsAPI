#pragma once

namespace glsl {
namespace light {

const char vertex[] =
	"#version 150											\n"
	"														\n"
	"uniform mat4 u_MVPMatrix;								\n"
	"attribute vec4 a_Position;								\n"
	"attribute vec2 a_texCoord;								\n"
	"varying vec2 v_texCoord;								\n"
	"uniform float u_fWidth;								\n"
	"uniform float u_fHeight;								\n"
	"uniform float u_cameradist;							\n"		

#ifdef SUPPORT_LIGHTING_TRANSITION
	"attribute vec4 a_Normal;								\n"			// normal vector
	"uniform vec4 u_LightPosition;							\n"			// light position
	"varying float v_LightIntensity;						\n"			// light intensity
	"uniform float u_enableLight;							\n"			// light position
	"uniform float u_LightCoefficient;						\n"			// light coefficient
#endif // SUPPORT_LIGHTING_TRANSITION

	"void main()											\n"
	"{														\n"
	"	gl_Position = a_Position;							\n"
	"   gl_Position.x /= u_fHeight;							\n"
	"   gl_Position.y /= u_fHeight;							\n"
	"   gl_Position.z /= u_fHeight;							\n"

#ifdef SUPPORT_LIGHTING_TRANSITION
	"	v_LightIntensity = 1.0;										\n"
	"	if(u_enableLight > 0.0)										\n"
	"	{															\n"
	"		vec4 normal = a_Normal;									\n"
	"		vec4 positionVec = normalize(a_Position);					\n"
	"		vec4 lightVec = normalize(u_LightPosition - positionVec);	\n"
	"		v_LightIntensity = u_LightCoefficient * max(dot(normal, lightVec), 0.0);			\n"
	"	}															\n"
#endif // SUPPORT_LIGHTING_TRANSITION

	"	gl_Position = u_MVPMatrix * gl_Position;			\n"
	"   gl_Position.x *= u_fHeight/u_fWidth;				\n"
	"   gl_Position.x = gl_Position.x * 2.0 - u_cameradist;	\n"
	"   gl_Position.y = gl_Position.y * 2.0 - u_cameradist;	\n"
	"   v_texCoord = a_texCoord;							\n"
	"}														\n";

const char fragment[] =  
	"#version 150											\n"
	"														\n"
	"varying vec2 v_texCoord;								\n"
	"uniform sampler2D u_tex0Map;							\n"

#ifdef USE_ANIMATION_COMPONENT_COUTDEV
	"uniform vec3 u_darkHSL;								\n"
	"uniform vec3 u_lightHSL;								\n"
	"uniform vec3 u_desatHSL;								\n"
	"uniform vec3 u_compHSL;								\n"
	"uniform vec3 u_comp2HSL;								\n"
	"uniform vec3 u_contHSL;								\n"
#endif // USE_ANIMATION_COMPONENT_COUTDEV

	"uniform vec3 u_inccolour;								\n"
	"uniform float u_mulcolour;								\n"
	"uniform float u_alpha;									\n"
	//Lightness
	"uniform float u_lightratio;							\n"
	// Black&White Color
	"uniform float u_bwratio;								\n"
	// Complementary Color
	"uniform float u_cmratio;								\n"
	// Contrasting Color
	"uniform float u_cnratio;								\n"

#ifdef SUPPORT_LIGHTING_TRANSITION
	"varying float v_LightIntensity;						\n"				// light intensity
#endif // SUPPORT_LIGHTING_TRANSITION

	//color value 0 to 1
	"vec4 validateColor(vec4 col)							\n"
	"{														\n"
	"	col.r = max(min(col.r, 1.0), 0.0);					\n"
	"	col.g = max(min(col.g, 1.0), 0.0);					\n"
	"	col.b = max(min(col.b, 1.0), 0.0);					\n"
	"	col.a = max(min(col.a, 1.0), 0.0);					\n"
	"	return col;											\n"
	"}														\n"

	// validate hsl
	"vec4 validateHSLA(vec4 hsla)							\n"
	"{														\n"
	"    hsla.x = hsla.x > 1.0 ? hsla.x - 1.0 : hsla.x;		\n"
	"    hsla.x = hsla.x < 0.0 ? hsla.x + 1.0 : hsla.x;		\n"
	"	 hsla.y = max(min(hsla.y, 1.0), 0.0);				\n"
	"	 hsla.z = max(min(hsla.z, 1.0), 0.0);				\n"
	"	 return hsla;										\n"
	"}														\n"

	// convert rgb to hsl
	"vec4 rgb2hsl(vec4 col)									\n"
	"{														\n"
	"	col = validateColor(col);							\n"
	"	float red   = col.r;								\n"
	"	float green = col.g;								\n"
	"	float blue  = col.b;								\n"
	"														\n"
	"	float minc  = min( col.r, col.g );					\n"
	"	minc  = min( minc, col.b );							\n"
	"	float maxc  = max( col.r, col.g );					\n"
	"	maxc  = max( maxc, col.b );					\n"
	"	float delta = maxc - minc;							\n"
	"														\n"
	"	float lum = (minc + maxc) * 0.5;					\n"
	"	float sat = 0.0;									\n"
	"	float hue = 0.0;									\n"
	"														\n"
	"	if (lum > 0.0 && lum < 1.0) {						\n"
	"	    float mul = (lum < 0.5)  ?  (lum)  :  (1.0-lum);\n"
	"	    sat = delta / (mul * 2.0);						\n"
	"	}													\n"
	"														\n"
	"	vec3 masks = vec3(									\n"
	"	    (maxc == red   && maxc != green) ? 1.0 : 0.0,	\n"
	"	    (maxc == green && maxc != blue)  ? 1.0 : 0.0,	\n"
	"	    (maxc == blue  && maxc != red)   ? 1.0 : 0.0	\n"
	"	);													\n"
	"														\n"
	"	vec3 adds = vec3(									\n"
	"	          ((green - blue ) / delta),				\n"
	"	    2.0 + ((blue  - red  ) / delta),				\n"
	"	    4.0 + ((red   - green) / delta) 				\n"
	"	);													\n"
	"														\n"
	"	float deltaGtz = (delta > 0.0) ? 1.0 : 0.0;			\n"
	"														\n"
	"	hue += dot( adds, masks );							\n"
	"	hue *= deltaGtz;									\n"
	"	hue /= 6.0;											\n"
	"														\n"
	"	if (hue < 0.0)										\n"
	"	    hue += 1.0;										\n"
	"														\n"
	"	return vec4( hue, sat, lum, col.a );				\n"
	"}														\n"

	// convert hsl to rgb
	"vec4 hsl2rgb(vec4 col)									\n"
	"{														\n"
	"	col = validateHSLA(col);							\n"
	"	const float onethird = 1.0 / 3.0;					\n"
	"	const float twothird = 2.0 / 3.0;					\n"
	"	const float rcpsixth = 6.0;							\n"
	"														\n"
	"	float hue = col.x;									\n"
	"	float sat = col.y;									\n"
	"	float lum = col.z;									\n"
	"														\n"
	"	vec3 xt = vec3(										\n"
	"	    rcpsixth * (hue - twothird),					\n"
	"	    0.0,											\n"
	"	    rcpsixth * (1.0 - hue)							\n"
	"	);													\n"
	"														\n"
	"	if (hue < twothird) {								\n"
	"	    xt.r = 0.0;										\n"
	"	    xt.g = rcpsixth * (twothird - hue);				\n"
	"	    xt.b = rcpsixth * (hue      - onethird);		\n"
	"	} 													\n"
	"														\n"
	"	if (hue < onethird) {								\n"
	"	    xt.r = rcpsixth * (onethird - hue);				\n"
	"	    xt.g = rcpsixth * hue;							\n"
	"	    xt.b = 0.0;										\n"
	"	}													\n"
	"														\n"
	"	xt = min( xt, 1.0 );								\n"
	"														\n"
	"	float sat2   =  2.0 * sat;							\n"
	"	float satinv =  1.0 - sat;							\n"
	"	float luminv =  1.0 - lum;							\n"
	"	float lum2m1 = (2.0 * lum) - 1.0;					\n"
	"	vec3  ct     = (sat2 * xt) + satinv;				\n"
	"														\n"
	"	vec3 rgb;											\n"
	"	if (lum >= 0.5)										\n"
	"	     rgb = (luminv * ct) + lum2m1;					\n"
	"	else rgb =  lum    * ct;							\n"
	"														\n"
	"	return vec4( rgb, col.a );							\n"		
	"}														\n"

	// main func
	"void main()											\n"
	"{														\n"
	"  vec4 tex0Color;										\n"
	"  vec4 tex1Color;										\n"
	"  float ftmpcolor, ftmpcolorR, ftmpcolorG, ftmpcolorB;	\n"
	// range check for Ripple transition
	"  if (v_texCoord.x < 0.0 || v_texCoord.y < 0.0 || v_texCoord.x > 1.0 || v_texCoord.y > 1.0)	\n"
	"	 tex0Color = vec4(0.0, 0.0, 0.0, 0.0);						\n"
	"  else													\n"
	"	 tex0Color = texture2D(u_tex0Map, v_texCoord);				\n"

	"  tex0Color += vec4(u_inccolour, 0.0);					\n"

#ifdef USE_ANIMATION_COMPONENT_COUTDEV
	// to B&W Color
	"  if (u_bwratio > 0.0)									\n"
	"  {													\n"
	"    vec4 hsla = rgb2hsl(tex0Color);					\n"	
	"    hsla += vec4(u_desatHSL, 0.0) * u_bwratio;			\n"
	"    tex0Color = hsl2rgb(hsla);							\n"
	"  }													\n"

	// to Complementary Color
	"  if (u_cmratio > 0.0)									\n"
	"  {													\n"
	"    vec4 hsla = rgb2hsl(tex0Color);					\n"
	"    hsla += vec4(u_compHSL, 0.0) * u_cmratio;			\n"
	"    tex0Color = hsl2rgb(hsla);							\n"
	"  }													\n"

	// to Complementary Color2
	"  if (u_cmratio < 0.0)									\n"
	"  {													\n"
	"    vec4 hsla = rgb2hsl(tex0Color);					\n"
	"    hsla += vec4(u_comp2HSL, 0.0) * (-u_cmratio);		\n"
	"    tex0Color = hsl2rgb(hsla);							\n"
	"  }													\n"

	// to Contrasting Color
	"  if (u_cnratio > 0.0)									\n"
	"  {													\n"
	"    vec4 hsla = rgb2hsl(tex0Color);					\n"
	"    hsla += vec4(u_contHSL, 0.0) * u_cnratio;			\n"
	"    tex0Color = hsl2rgb(hsla);							\n"
	"  }													\n"

	// to Lightness
	"  if (u_lightratio > 0.0)								\n"
	"  {													\n"
	"    vec4 hsla = rgb2hsl(tex0Color);					\n"
	"    hsla += vec4(u_lightHSL, 0.0) * u_lightratio;		\n"
	"    tex0Color = hsl2rgb(hsla);							\n"
	"  }													\n"

	// to Darkness
	"  if (u_lightratio < 0.0)								\n"
	"  {													\n"
	"    vec4 hsla = rgb2hsl(tex0Color);					\n"
	"    hsla += vec4(u_darkHSL, 0.0) * (-u_lightratio);		\n"
	"    tex0Color = hsl2rgb(hsla);							\n"
	"  }													\n"
#endif // USE_ANIMATION_COMPONENT_COUTDEV

	"  tex0Color.r *= u_mulcolour;							\n"
	"  tex0Color.g *= u_mulcolour;							\n"
	"  tex0Color.b *= u_mulcolour;							\n"
	"  tex0Color.a *= u_alpha;								\n"

#ifdef SUPPORT_LIGHTING_TRANSITION
	"  if(v_LightIntensity < 1.0)							\n"
	"  {													\n"
	"		tex0Color = vec4(v_LightIntensity * tex0Color.rgb, tex0Color.a);		\n"		
	"  }													\n"
#endif // SUPPORT_LIGHTING_TRANSITION

	"  gl_FragColor = tex0Color;							\n"
	"}														\n";
} // namespace light {
namespace nolight {
const char vertex[] =
	"uniform mat4 u_MVPMatrix;								\n"
	"attribute vec4 a_Position;								\n"
	"attribute vec2 a_texCoord;								\n"
	"varying vec2 v_texCoord;								\n"
	"uniform float u_fWidth;								\n"
	"uniform float u_fHeight;								\n"
	"uniform float u_cameradist;							\n"		

	"void main()											\n"
	"{														\n"
	"	gl_Position = a_Position;							\n"
	"   gl_Position.x /= u_fHeight;							\n"
	"   gl_Position.y /= u_fHeight;							\n"
	"   gl_Position.z /= u_fHeight;							\n"
	"	gl_Position = u_MVPMatrix * gl_Position;			\n"
	"   gl_Position.x *= u_fHeight/u_fWidth;				\n"
	"   gl_Position.x = gl_Position.x * 2.0 - u_cameradist;	\n"
	"   gl_Position.y = gl_Position.y * 2.0 - u_cameradist;	\n"
	"   v_texCoord = a_texCoord;							\n"
	"}														\n";

const char fragment[] =  
	"varying vec2 v_texCoord;								\n"
	"uniform sampler2D u_tex0Map;							\n"

#ifdef USE_ANIMATION_COMPONENT_COUTDEV
	"uniform vec3 u_darkHSL;								\n"
	"uniform vec3 u_lightHSL;								\n"
	"uniform vec3 u_desatHSL;								\n"
	"uniform vec3 u_compHSL;								\n"
	"uniform vec3 u_comp2HSL;								\n"
	"uniform vec3 u_contHSL;								\n"
#endif // USE_ANIMATION_COMPONENT_COUTDEV

	"uniform vec3 u_inccolour;								\n"
	"uniform float u_mulcolour;								\n"
	"uniform float u_alpha;									\n"
	//Lightness
	"uniform float u_lightratio;							\n"
	// Black&White Color
	"uniform float u_bwratio;								\n"
	// Complementary Color
	"uniform float u_cmratio;								\n"
	// Contrasting Color
	"uniform float u_cnratio;								\n"

	//color value 0 to 1
	"vec4 validateColor(vec4 col)							\n"
	"{														\n"
	"	col.r = max(min(col.r, 1.0), 0.0);					\n"
	"	col.g = max(min(col.g, 1.0), 0.0);					\n"
	"	col.b = max(min(col.b, 1.0), 0.0);					\n"
	"	col.a = max(min(col.a, 1.0), 0.0);					\n"
	"	return col;											\n"
	"}														\n"

	// validate hsl
	"vec4 validateHSLA(vec4 hsla)							\n"
	"{														\n"
	"    hsla.x = hsla.x > 1.0 ? hsla.x - 1.0 : hsla.x;		\n"
	"    hsla.x = hsla.x < 0.0 ? hsla.x + 1.0 : hsla.x;		\n"
	"	 hsla.y = max(min(hsla.y, 1.0), 0.0);				\n"
	"	 hsla.z = max(min(hsla.z, 1.0), 0.0);				\n"
	"	 return hsla;										\n"
	"}														\n"

	// convert rgb to hsl
	"vec4 rgb2hsl(vec4 col)									\n"
	"{														\n"
	"	col = validateColor(col);							\n"
	"	float red   = col.r;								\n"
	"	float green = col.g;								\n"
	"	float blue  = col.b;								\n"
	"														\n"
	"	float minc  = min( col.r, col.g );					\n"
	"	minc  = min( minc, col.b );							\n"
	"	float maxc  = max( col.r, col.g );					\n"
	"	maxc  = max( maxc, col.b );					\n"
	"	float delta = maxc - minc;							\n"
	"														\n"
	"	float lum = (minc + maxc) * 0.5;					\n"
	"	float sat = 0.0;									\n"
	"	float hue = 0.0;									\n"
	"														\n"
	"	if (lum > 0.0 && lum < 1.0) {						\n"
	"	    float mul = (lum < 0.5)  ?  (lum)  :  (1.0-lum);\n"
	"	    sat = delta / (mul * 2.0);						\n"
	"	}													\n"
	"														\n"
	"	vec3 masks = vec3(									\n"
	"	    (maxc == red   && maxc != green) ? 1.0 : 0.0,	\n"
	"	    (maxc == green && maxc != blue)  ? 1.0 : 0.0,	\n"
	"	    (maxc == blue  && maxc != red)   ? 1.0 : 0.0	\n"
	"	);													\n"
	"														\n"
	"	vec3 adds = vec3(									\n"
	"	          ((green - blue ) / delta),				\n"
	"	    2.0 + ((blue  - red  ) / delta),				\n"
	"	    4.0 + ((red   - green) / delta) 				\n"
	"	);													\n"
	"														\n"
	"	float deltaGtz = (delta > 0.0) ? 1.0 : 0.0;			\n"
	"														\n"
	"	hue += dot( adds, masks );							\n"
	"	hue *= deltaGtz;									\n"
	"	hue /= 6.0;											\n"
	"														\n"
	"	if (hue < 0.0)										\n"
	"	    hue += 1.0;										\n"
	"														\n"
	"	return vec4( hue, sat, lum, col.a );				\n"
	"}														\n"

	// convert hsl to rgb
	"vec4 hsl2rgb(vec4 col)									\n"
	"{														\n"
	"	col = validateHSLA(col);							\n"
	"	const float onethird = 1.0 / 3.0;					\n"
	"	const float twothird = 2.0 / 3.0;					\n"
	"	const float rcpsixth = 6.0;							\n"
	"														\n"
	"	float hue = col.x;									\n"
	"	float sat = col.y;									\n"
	"	float lum = col.z;									\n"
	"														\n"
	"	vec3 xt = vec3(										\n"
	"	    rcpsixth * (hue - twothird),					\n"
	"	    0.0,											\n"
	"	    rcpsixth * (1.0 - hue)							\n"
	"	);													\n"
	"														\n"
	"	if (hue < twothird) {								\n"
	"	    xt.r = 0.0;										\n"
	"	    xt.g = rcpsixth * (twothird - hue);				\n"
	"	    xt.b = rcpsixth * (hue      - onethird);		\n"
	"	} 													\n"
	"														\n"
	"	if (hue < onethird) {								\n"
	"	    xt.r = rcpsixth * (onethird - hue);				\n"
	"	    xt.g = rcpsixth * hue;							\n"
	"	    xt.b = 0.0;										\n"
	"	}													\n"
	"														\n"
	"	xt = min( xt, 1.0 );								\n"
	"														\n"
	"	float sat2   =  2.0 * sat;							\n"
	"	float satinv =  1.0 - sat;							\n"
	"	float luminv =  1.0 - lum;							\n"
	"	float lum2m1 = (2.0 * lum) - 1.0;					\n"
	"	vec3  ct     = (sat2 * xt) + satinv;				\n"
	"														\n"
	"	vec3 rgb;											\n"
	"	if (lum >= 0.5)										\n"
	"	     rgb = (luminv * ct) + lum2m1;					\n"
	"	else rgb =  lum    * ct;							\n"
	"														\n"
	"	return vec4( rgb, col.a );							\n"		
	"}														\n"

	// main func
	"void main()											\n"
	"{														\n"
	"  vec4 tex0Color;										\n"
	"  vec4 tex1Color;										\n"
	"  float ftmpcolor, ftmpcolorR, ftmpcolorG, ftmpcolorB;	\n"
	// range check for Ripple transition
	"  if (v_texCoord.x < 0.0 || v_texCoord.y < 0.0 || v_texCoord.x > 1.0 || v_texCoord.y > 1.0)	\n"
	"	 tex0Color = vec4(0.0, 0.0, 0.0, 0.0);						\n"
	"  else													\n"
	"	 tex0Color = texture2D(u_tex0Map, v_texCoord);				\n"

	"  tex0Color += vec4(u_inccolour, 0.0);					\n"

#ifdef USE_ANIMATION_COMPONENT_COUTDEV
	// to B&W Color
	"  if (u_bwratio > 0.0)									\n"
	"  {													\n"
	"    vec4 hsla = rgb2hsl(tex0Color);					\n"	
	"    hsla += vec4(u_desatHSL, 0.0) * u_bwratio;			\n"
	"    tex0Color = hsl2rgb(hsla);							\n"
	"  }													\n"

	// to Complementary Color
	"  if (u_cmratio > 0.0)									\n"
	"  {													\n"
	"    vec4 hsla = rgb2hsl(tex0Color);					\n"
	"    hsla += vec4(u_compHSL, 0.0) * u_cmratio;			\n"
	"    tex0Color = hsl2rgb(hsla);							\n"
	"  }													\n"

	// to Complementary Color2
	"  if (u_cmratio < 0.0)									\n"
	"  {													\n"
	"    vec4 hsla = rgb2hsl(tex0Color);					\n"
	"    hsla += vec4(u_comp2HSL, 0.0) * (-u_cmratio);		\n"
	"    tex0Color = hsl2rgb(hsla);							\n"
	"  }													\n"

	// to Contrasting Color
	"  if (u_cnratio > 0.0)									\n"
	"  {													\n"
	"    vec4 hsla = rgb2hsl(tex0Color);					\n"
	"    hsla += vec4(u_contHSL, 0.0) * u_cnratio;			\n"
	"    tex0Color = hsl2rgb(hsla);							\n"
	"  }													\n"

	// to Lightness
	"  if (u_lightratio > 0.0)								\n"
	"  {													\n"
	"    vec4 hsla = rgb2hsl(tex0Color);					\n"
	"    hsla += vec4(u_lightHSL, 0.0) * u_lightratio;		\n"
	"    tex0Color = hsl2rgb(hsla);							\n"
	"  }													\n"

	// to Darkness
	"  if (u_lightratio < 0.0)								\n"
	"  {													\n"
	"    vec4 hsla = rgb2hsl(tex0Color);					\n"
	"    hsla += vec4(u_darkHSL, 0.0) * (-u_lightratio);		\n"
	"    tex0Color = hsl2rgb(hsla);							\n"
	"  }													\n"
#endif // USE_ANIMATION_COMPONENT_COUTDEV

	"  tex0Color.r *= u_mulcolour;							\n"
	"  tex0Color.g *= u_mulcolour;							\n"
	"  tex0Color.b *= u_mulcolour;							\n"
	"  tex0Color.a *= u_alpha;								\n"

	"  gl_FragColor = tex0Color;							\n"
	"}														\n";
} // namespace nolight {
} // namespace glsl {
