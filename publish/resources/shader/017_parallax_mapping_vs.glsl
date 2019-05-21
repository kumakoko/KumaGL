/**************************************************************************************************************************
Copyright(C) 2014-2017 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute,sublicense, and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************************************/
// 使用视差贴图，实现凹凸效果的顶点着色器
#version 330 core

layout (location = 0) in vec3 vertex_position;     	// 顶点位置坐标，基于世界空间
layout (location = 1) in vec3 vertex_normal; 		// 顶点法线
layout (location = 2) in vec3 vertex_tangent; 		// 顶点切线
layout (location = 3) in vec3 vertex_binormal; 		// 顶点副法线
layout (location = 4) in vec2 vertex_tex_coord_1; 	// 顶点第一层纹理映射坐标

out VSOutput 
{
	vec2 fragment_tex_cooord_1;			// 第一层纹理坐标
	vec3 camera_dir_in_tangent_space;	// 摄像机位置点到顶点的连线向量，变换到切线空间后的值
}vs_out;

uniform mat4 world_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform vec3 camera_pos_in_world;


void main()
{
	gl_Position = projection_matrix * view_matrix * world_matrix * vec4(vertex_position, 1.0);
	vs_out.fragment_tex_cooord_1 = vertex_tex_coord_1;
	
	// 变换顶点坐标，法线，副法线，切线到观察空间
	mat4 wv_matrix = view_matrix * world_matrix;
	vec4 tangent_in_view = wv_matrix * vec4(vertex_tangent,1.0);
	vec4 normal_in_view = wv_matrix * vec4(vertex_normal,1.0);
	vec4 binormal_in_view = wv_matrix * vec4(vertex_binormal,1.0);
	vec4 vertex_pos_in_view = wv_matrix * vec4(vertex_position,1.0);
   
    // vertex_pos_in_view就是在观察空间中，摄像机位置点到顶点的连线向量（未单位化）
	// 现在把这个连线向量变换到切线空间中
    vs_out.camera_dir_in_tangent_space.x = dot(tangent_in_view.xyz , vertex_pos_in_view.xyz);
    vs_out.camera_dir_in_tangent_space.y = dot(binormal_in_view.xyz, vertex_pos_in_view.xyz);
    vs_out.camera_dir_in_tangent_space.z = dot(normal_in_view.xyz , vertex_pos_in_view.xyz);
}