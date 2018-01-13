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
#version 330 core

layout (location = 0) in vec3 vsin_position;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec2 vsin_tex_coords_1;

struct VS2FS
{
    vec3 Position;   // 片元的经过World变换后的坐标，即世界坐标值
    vec3 Normal;     // 片元的基于世界坐标系下的坐标，把顶点的法线
                     // 从局部模型坐标系变换到世界坐标系，需要乘以
                     // world变换矩阵的逆矩阵
    vec2 TexCoords_1;// 片元的第一层纹理映射坐标
};

out VS2FS v2f;       // 从vs输出到fs的结构体

uniform mat4 u_projection_matrix; // 投影矩阵
uniform mat4 u_view_matrix;       // 观察矩阵
uniform mat4 u_world_matrix;      // 世界矩阵
uniform bool u_inverse_normals;   // 需要对法线进行取反吗

void main()
{
    // 把输入的顶点坐标变换到世界坐标系下
    v2f.Position = vec3(u_world_matrix * vec4(vsin_position, 1.0));

    // 把输入的顶点纹理映射坐标输出到下一个阶段
    v2f.TexCoords_1 = vsin_tex_coords_1;

    // 是否对法线进行取反向
    vec3 n = u_inverse_normals ? -vsin_normal : vsin_normal;

    // 得到世界变换矩阵的逆转置矩阵，用来把法线转换到世界坐标系下，并单位化
    mat3 normal_matrix = transpose(inverse(mat3(u_world_matrix)));
    v2f.Normal = normalize(normal_matrix * n);

    gl_Position = u_projection_matrix * u_view_matrix * u_world_matrix * vec4(vsin_position, 1.0);
}