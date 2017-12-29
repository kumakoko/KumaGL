// 延迟渲染中的渲染几何信息阶段的vertex shader
#version 330 core

layout (location = 0) in vec3 vsin_position;         // 顶点位置坐标
layout (location = 1) in vec2 vsin_texture_coords_1; // 顶点第一层纹理坐标
layout (location = 2) in vec3 vsin_normal;           // 顶点法线

out vec3 v2f_frag_pos;				// 经光栅器插值处理后，片元的位置坐标
out vec2 v2f_frag_texture_coords_1; // 经光栅器插值处理后，片元的第一层纹理坐标
out vec3 v2f_frag_normal;           // 经光栅器插值处理后，片元的法线

uniform mat4 u_world_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

void main()
{
    vec4 world_pos = u_world_matrix * vec4(vsin_position, 1.0);
    v2f_frag_pos = world_pos.xyz; 
    v2f_frag_texture_coords_1 = vsin_texture_coords_1;
    
	// 把顶点的法线变换到最终的投影空间，需要用到world矩阵的逆转置矩阵
    mat3 normal_matrix = transpose(inverse(mat3(u_world_matrix)));
    v2f_frag_normal = normal_matrix * vsin_normal;

    gl_Position = u_projection_matrix * u_view_matrix * world_pos;
}