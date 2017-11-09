// 演示在各个坐标空间中进行变换的示例程序的vertex shader
#version 330 core

layout (location = 0) in vec3 vertex_position;			// 顶点坐标
layout (location = 1) in vec2 vertex_texture_coord_1;	// 顶点第一层纹理贴图坐标
layout (location = 2) in vec3 vertex_normal;			// 顶点的法线
layout (location = 3) in vec4 vertex_color; 			// 顶点的颜色

out vec4 fragment_color;		// 将要输出光栅器，然后输出到fragment shader的颜色
out vec2 fragment_tex_coord_1;	// 将要输出光栅器，然后输出到fragment shader的第一层片元纹理贴图坐标
uniform mat4 world_matrix; 		// 把顶点从模型局部空间变换到世界空间下的变换矩阵
uniform mat4 view_matrix; 		// 把顶点从世界空间变换到观察空间下的变换矩阵
uniform mat4 projection_matrix; // 把顶点从观察空间变换到投影空间下的变换矩阵

void main()
{
    gl_Position = projection_matrix * view_matrix * world_matrix * vec4(vertex_position, 1.0f);
    fragment_tex_coord_1 = vertex_texture_coord_1;
	fragment_color = vertex_color;
}