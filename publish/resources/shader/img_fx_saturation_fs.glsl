// 彩色图饱和度控制的fragment shader

#version 330 core
in vec2 frag_tex_coord_1;

out vec4 final_color;

uniform sampler2D main_texture;
uniform float saturation_factor; // 调整饱和度的系数

// 饱和度控制
vec4 Saturation(vec3 color_rgb,float s_factor)
{
	vec3 w = vec3(0.2125,0.7154,0.0721);
	float luminace = dot( color_rgb,w);
	vec3 target = vec3(luminace,luminace,luminace);
	return vec4(mix(target,color_rgb,s_factor),1.0);
}

void main()
{
	vec4 texture_color = texture(main_texture, frag_tex_coord_1);
	final_color = Saturation(texture_color.rgb,saturation_factor);
}