const char* vertex = STRINGIFY(120,

varying vec2 v_texCoord;								
uniform sampler2D u_tex0Map;							

#ifdef USE_ANIMATION_COMPONENT_COUTDEV
uniform vec3 u_darkHSL;								
uniform vec3 u_lightHSL;								
uniform vec3 u_desatHSL;								
uniform vec3 u_compHSL;								
uniform vec3 u_comp2HSL;								
uniform vec3 u_contHSL;								
#endif // USE_ANIMATION_COMPONENT_COUTDEV

uniform vec3 u_inccolour;								
uniform float u_mulcolour;								
uniform float u_alpha;									
//Lightness
uniform float u_lightratio;							
// Black&White Color
uniform float u_bwratio;								
// Complementary Color
uniform float u_cmratio;								
// Contrasting Color
uniform float u_cnratio;								

//color value 0 to 1
vec4 validateColor(vec4 col)							
{														
	col.r = max(min(col.r, 1.0), 0.0);					
	col.g = max(min(col.g, 1.0), 0.0);					
	col.b = max(min(col.b, 1.0), 0.0);					
	col.a = max(min(col.a, 1.0), 0.0);					
	return col;											
}														

// validate hsl
vec4 validateHSLA(vec4 hsla)							
{														
	hsla.x = hsla.x > 1.0 ? hsla.x - 1.0 : hsla.x;		
	hsla.x = hsla.x < 0.0 ? hsla.x + 1.0 : hsla.x;		
	hsla.y = max(min(hsla.y, 1.0), 0.0);				
	hsla.z = max(min(hsla.z, 1.0), 0.0);				
	return hsla;										
}														

// convert rgb to hsl
vec4 rgb2hsl(vec4 col)									
{														
	col = validateColor(col);							
	float red   = col.r;								
	float green = col.g;								
	float blue  = col.b;								

	float minc  = min( col.r, col.g );					
	minc  = min( minc, col.b );							
	float maxc  = max( col.r, col.g );					
	maxc  = max( maxc, col.b );					
	float delta = maxc - minc;							

	float lum = (minc + maxc) * 0.5;					
	float sat = 0.0;									
	float hue = 0.0;									

	if (lum > 0.0 && lum < 1.0) {						
		float mul = (lum < 0.5)  ?  (lum)  :  (1.0-lum);
		sat = delta / (mul * 2.0);						
	}													

	vec3 masks = vec3(									
			(maxc == red   && maxc != green) ? 1.0 : 0.0,	
			(maxc == green && maxc != blue)  ? 1.0 : 0.0,	
			(maxc == blue  && maxc != red)   ? 1.0 : 0.0	
			);													

	vec3 adds = vec3(									
			((green - blue ) / delta),				
			2.0 + ((blue  - red  ) / delta),				
			4.0 + ((red   - green) / delta) 				
			);													

	float deltaGtz = (delta > 0.0) ? 1.0 : 0.0;			

	hue += dot( adds, masks );							
	hue *= deltaGtz;									
	hue /= 6.0;											

	if (hue < 0.0)										
		hue += 1.0;										

	return vec4( hue, sat, lum, col.a );				
}														

// convert hsl to rgb
vec4 hsl2rgb(vec4 col)									
{														
	col = validateHSLA(col);							
	const float onethird = 1.0 / 3.0;					
	const float twothird = 2.0 / 3.0;					
	const float rcpsixth = 6.0;							

	float hue = col.x;									
	float sat = col.y;									
	float lum = col.z;									

	vec3 xt = vec3(										
			rcpsixth * (hue - twothird),					
			0.0,											
			rcpsixth * (1.0 - hue)							
			);													

	if (hue < twothird) {								
		xt.r = 0.0;										
		xt.g = rcpsixth * (twothird - hue);				
		xt.b = rcpsixth * (hue      - onethird);		
	} 													

	if (hue < onethird) {								
		xt.r = rcpsixth * (onethird - hue);				
		xt.g = rcpsixth * hue;							
		xt.b = 0.0;										
	}													

	xt = min( xt, 1.0 );								

	float sat2   =  2.0 * sat;							
	float satinv =  1.0 - sat;							
	float luminv =  1.0 - lum;							
	float lum2m1 = (2.0 * lum) - 1.0;					
	vec3  ct     = (sat2 * xt) + satinv;				

	vec3 rgb;											
	if (lum >= 0.5)										
		rgb = (luminv * ct) + lum2m1;					
	else rgb =  lum    * ct;							

	return vec4( rgb, col.a );									
}														

// main func
void main()											
{														
	vec4 tex0Color;										
	vec4 tex1Color;										
	float ftmpcolor, ftmpcolorR, ftmpcolorG, ftmpcolorB;	
	// range check for Ripple transition
	if (v_texCoord.x < 0.0 || v_texCoord.y < 0.0 || v_texCoord.x > 1.0 || v_texCoord.y > 1.0)	
		tex0Color = vec4(0.0, 0.0, 0.0, 0.0);						
	else													
		tex0Color = texture2D(u_tex0Map, v_texCoord);				

	tex0Color += vec4(u_inccolour, 0.0);					

#ifdef USE_ANIMATION_COMPONENT_COUTDEV
	// to B&W Color
	if (u_bwratio > 0.0)									
	{													
		vec4 hsla = rgb2hsl(tex0Color);						
		hsla += vec4(u_desatHSL, 0.0) * u_bwratio;			
		tex0Color = hsl2rgb(hsla);							
	}													

	// to Complementary Color
	if (u_cmratio > 0.0)									
	{													
		vec4 hsla = rgb2hsl(tex0Color);					
		hsla += vec4(u_compHSL, 0.0) * u_cmratio;			
		tex0Color = hsl2rgb(hsla);							
	}													

	// to Complementary Color2
	if (u_cmratio < 0.0)									
	{													
		vec4 hsla = rgb2hsl(tex0Color);					
		hsla += vec4(u_comp2HSL, 0.0) * (-u_cmratio);		
		tex0Color = hsl2rgb(hsla);							
	}													

	// to Contrasting Color
	if (u_cnratio > 0.0)									
	{													
		vec4 hsla = rgb2hsl(tex0Color);					
		hsla += vec4(u_contHSL, 0.0) * u_cnratio;			
		tex0Color = hsl2rgb(hsla);							
	}													

	// to Lightness
	if (u_lightratio > 0.0)								
	{													
		vec4 hsla = rgb2hsl(tex0Color);					
		hsla += vec4(u_lightHSL, 0.0) * u_lightratio;		
		tex0Color = hsl2rgb(hsla);							
	}													

	// to Darkness
	if (u_lightratio < 0.0)								
	{													
		vec4 hsla = rgb2hsl(tex0Color);					
		hsla += vec4(u_darkHSL, 0.0) * (-u_lightratio);		
		tex0Color = hsl2rgb(hsla);							
	}													
#endif // USE_ANIMATION_COMPONENT_COUTDEV

	tex0Color.r *= u_mulcolour;							
	tex0Color.g *= u_mulcolour;							
	tex0Color.b *= u_mulcolour;							
	tex0Color.a *= u_alpha;								

	gl_FragColor = tex0Color;							
}
);
