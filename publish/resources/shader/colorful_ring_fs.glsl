// 彩色环
#version 330 core

uniform float global_time;
uniform vec2 screen_resolution;

in vec3 vs_output_color;
in vec2 frag_tex_coord_1;
in vec2 frag_tex_coord_2;
in vec2 frag_tex_coord_3;
in vec2 frag_tex_coord_4;

out vec4 fragment_color;

void main()
{
	// vec2 sr = vec2(800.0,600.0);
	vec2 p = (2.0 * gl_FragCoord.xy- screen_resolution.xy)/ screen_resolution.y;  
    float tau = 3.1415926535*2.0;  
	float a = atan(p.x,p.y);  
	float r = length(p)*0.75;  
	vec2 uv = vec2(a/tau,r);  
              
	//get the color  
	float xCol = (uv.x - (global_time / 3.0)) * 3.0;  
	xCol = sign(xCol)*mod(xCol, 3.0);  
	vec3 horColour = vec3(0.25, 0.25, 0.25);  
              
	if (xCol < 1.0)
	{  
		horColour.r += 1.0 - xCol;  
		horColour.g += xCol;  
	} 
	else if (xCol < 2.0) 
	{  
		xCol -= 1.0;  
		horColour.g += 1.0 - xCol;  
		horColour.b += xCol;  
	} 
	else 
	{  
		xCol -= 2.0;  
		horColour.b += 1.0 - xCol;  
		horColour.r += xCol;  
	}  
  
	// draw color beam  
	uv = (2.0 * uv) - 1.0;  
	float beamWidth = (0.7+0.5*cos(uv.x*10.0*tau*0.15*clamp(floor(5.0 + 10.0*cos(global_time)), 0.0, 10.0))) * abs(1.0 / (30.0 * uv.y));  
	vec3 horBeam = vec3(beamWidth,beamWidth,beamWidth);  
	fragment_color = vec4((( horBeam)* horColour ), 1.0);
}