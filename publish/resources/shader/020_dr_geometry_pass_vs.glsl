/*************************************************************************************
Copyright(C) 2014-2018 www.xionggf.com

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute,sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
 OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,OUT OF OR IN CONNECTION WITH THE 
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
************************************************************************************/
// 延迟渲染中的渲染几何信息阶段的vertex shader
#version 330 core

layout (location = 0) in vec3 vsin_position;         // 顶点位置坐标
layout (location = 1) in vec2 vsin_texture_coords_1; // 顶点第一层纹理坐标
layout (location = 2) in vec3 vsin_normal;           // 顶点法线

out vec3 v2f_frag_pos;              // 经光栅器插值处理后，片元的位置坐标
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