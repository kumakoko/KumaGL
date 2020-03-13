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
// 演示在各个坐标空间中进行变换的示例程序的vertex shader
#version 330 core

layout (location = 0) in vec3 vertex_position;          // 顶点坐标
layout (location = 1) in vec2 vertex_texture_coord_1;   // 顶点第一层纹理贴图坐标
layout (location = 2) in vec3 vertex_normal;            // 顶点的法线
layout (location = 3) in vec4 vertex_color;             // 顶点的颜色

out vec3 fragment_normal;       // 将要输出到光栅器，然后输出到fragment shader的法线
out vec2 fragment_tex_coord_1;  // 将要输出到光栅器，然后输出到fragment shader的第一层片元纹理贴图坐标
out vec3 fragment_world_pos;    // 将要输出到光栅器，然后输出到fragment shader的第一层片元纹理贴图坐标

uniform mat4 u_world_matrix;      // 把顶点从模型局部空间变换到世界空间下的变换矩阵
uniform mat4 u_view_matrix;       // 把顶点从世界空间变换到观察空间下的变换矩阵
uniform mat4 u_projection_matrix; // 把顶点从观察空间变换到投影空间下的变换矩阵

void main()
{
    vec4 vp = vec4(vertex_position, 1.0f);
    gl_Position = u_projection_matrix * u_view_matrix * u_world_matrix * vp;
    fragment_tex_coord_1 = vertex_texture_coord_1;
    fragment_normal = mat3(u_world_matrix) * vertex_normal;
    fragment_world_pos = vec3(u_world_matrix * vp);
}