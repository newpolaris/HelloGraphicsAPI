#pragma once

namespace gles {
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

const BrCHAR fragment[] =  
	"precision mediump float;								\n"
	"varying vec2 v_texCoord;								\n"
	"uniform sampler2D u_tex0Map;							\n"

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
	// SUM(min, max)
	"float sumMinMax(float r, float g, float b)				\n"
	"{														\n"
	"  float fmin, fmax;									\n"
	"  if (r > g)											\n"
	"  {													\n"
	"    fmin = g; fmax=r;									\n"
	"  }													\n"
	"  else													\n"
	"  {													\n"
	"    fmin = r; fmax=g;									\n"
	"  }													\n"
	"  if (fmin > b) fmin = b;								\n"
	"  if (fmax < b) fmax = b;								\n"
	"  return (fmin + fmax);								\n"
	"}														\n"
	// get Complementary Color
	"float comp(float basic, float new, float ratio)		\n"
	"{														\n"
	"  float ftemp;											\n"
	"  if (ratio < 0.5)													\n"
	"    ftemp = basic + (max(basic, new) - basic) * ratio * 2.0;		\n"
	"  else																\n"
	"    ftemp = new + (max(basic, new) - new) * (1.0 - ratio) * 2.0;	\n"
	"  return ftemp;										\n"
	"}														\n"
	// convert rgb to hsv
	// 		"vec3 rgb2hsv(vec3 c)													\n"
	// 		"{																		\n"
	// 		"    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);					\n"
	// 		"    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));	\n"
	// 		"    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));	\n"
	// 		"    float d = q.x - min(q.w, q.y);										\n"
	// 		"    float e = 1.0e-10;													\n"
	// 		"    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);\n"
	// 		"}																		\n"
	// 		// convert hsv to rgb
	// 		"vec3 hsv2rgb(vec3 c)									\n"
	// 		"{														\n"
	// 		"    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);		\n"
	// 		"    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);	\n"
	// 		"    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);\n"
	// 		"}														\n"

	//color value 0 to 1
	"vec4 validateColor(vec4 col)							\n"
	"{														\n"
	"	col.r = max(min(col.r, 1.0), 0.0);					\n"
	"	col.g = max(min(col.g, 1.0), 0.0);					\n"
	"	col.b = max(min(col.b, 1.0), 0.0);					\n"
	"	col.a = max(min(col.a, 1.0), 0.0);					\n"
	"	return col;											\n"
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
	"	col = validateColor(col);							\n"
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

	// to B&W Color
	"  if (u_bwratio > 0.0)									\n"
	"  {													\n"
	"    vec4 hsla = rgb2hsl(tex0Color);					\n"		
	"    hsla.y -= 0.71 * u_bwratio;						\n"		
	"    tex0Color = hsl2rgb(hsla);							\n"
	"  }													\n"

	// to Complementary Color
	"  if (u_cmratio > 0.0)									\n"
	"  {													\n"
	"    vec4 hsl = rgb2hsl(tex0Color);							\n"
	"    hsl.x = hsl.x + 0.25 * u_cmratio;					\n"
	"    hsl.x = hsl.x > 1.0 ? hsl.x - 1.0 : hsl.x;			\n"
	"    tex0Color = hsl2rgb(hsl);								\n"

	"  }													\n"

	// to Complementary Color2
	"  if (u_cmratio < 0.0)								\n"
	"  {													\n"
	"    vec4 hsl = rgb2hsl(tex0Color);							\n"
	"    hsl.x = hsl.x + 0.67 * -u_cmratio;					\n"
	"    hsl.x = hsl.x > 1.0 ? hsl.x - 1.0 : hsl.x;			\n"
	"    tex0Color = hsl2rgb(hsl);								\n"

	"  }													\n"

	// to Contrasting Color
	"  if (u_cnratio > 0.0)									\n"
	"  {													\n"
	"    vec4 hsl = rgb2hsl(tex0Color);							\n"
	"    hsl.x = hsl.x + 0.5 * u_cnratio;					\n"
	"    hsl.x = hsl.x > 1.0 ? hsl.x - 1.0 : hsl.x;			\n"
	"    tex0Color = hsl2rgb(hsl);								\n"

	"  }													\n"

	// to Lightness
	"  if (u_lightratio != 0.0)								\n"
	"  {													\n"		
	"    vec4 hsla = rgb2hsl(tex0Color);					\n"
	"    hsla.y += 0.12 * u_lightratio;						\n"
	"    hsla.z += 0.25 * u_lightratio;						\n"		
	"    tex0Color = hsl2rgb(hsla);							\n"		
	"  }													\n"

	"  tex0Color.r *= u_mulcolour;							\n"
	"  tex0Color.g *= u_mulcolour;							\n"
	"  tex0Color.b *= u_mulcolour;							\n"
	"  tex0Color.a *= u_alpha;								\n"
	"  gl_FragColor = tex0Color;							\n"
	"}														\n";

} // namespace gles {
