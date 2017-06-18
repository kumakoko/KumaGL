// 浮雕效果的fragment shader

#version 330 core
in vec2 frag_tex_coord_1;

out vec4 final_color;

uniform sampler2D main_texture;

float Emboss(sampler2D source_texture,vec2 tex_st)
{
	ivec2 texture_resolution = textureSize(source_texture,0);
	
	float rs = float(texture_resolution.s);
	float rt = float(texture_resolution.t);
	
	vec2 stpo = vec2(1.0 / rs,0.0);
	vec2 stpp = vec2(1.0 / rs,1.0 / rt);
	
	vec3 texture_color_rgb = texture(source_texture,tex_st).rgb;
	vec3 cp1cp1  = texture(source_texture,tex_st+stpp).rgb;
	vec3 diffs = texture_color_rgb - cp1cp1;
	
	float max_v = diffs.r;
	
	if (abs(diffs.g) > abs(max_v))
		max_v = diffs.g;
	
	if (abs(diffs.b) > abs(max_v))
		max_v = diffs.b;
		
	float gray = clamp( max_v + 0.5 , 0.0,1.0);
	return gray;
}

void main()
{
	vec4 texture_color = texture(main_texture, frag_tex_coord_1);
	float gray = Emboss(main_texture,frag_tex_coord_1);
	final_color = vec4(gray,gray,gray,1.0);
}