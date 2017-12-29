#version 330 core

layout (location = 0) out vec3 pos_to_gbuffer;
layout (location = 1) out vec3 normal_to_gbuffer;
layout (location = 2) out vec4 albedo_to_gbuffer;//gAlbedoSpec;

in vec2 v2f_frag_texture_coords_1;
in vec3 v2f_frag_pos;
in vec3 v2f_frag_normal;

uniform sampler2D texture_diffuse1;

void main()
{    
    pos_to_gbuffer = v2f_frag_pos; // 把片元的世界坐标位置存储到G-buffer的纹理中去
    normal_to_gbuffer = normalize(v2f_frag_normal); // 把片元的法线存储到G-buffer的纹理中去
    albedo_to_gbuffer = texture(texture_diffuse1, v2f_frag_texture_coords_1);
}