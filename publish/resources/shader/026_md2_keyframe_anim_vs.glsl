/**************************************************************************************************************************
Copyright(C) 2014-2020www.xionggf.com

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
// 用来对关键帧进行插值，形成动画效果的顶点着色器
// 可以用在对md2模型文件的显示

#version 330

uniform mat4 u_projection_matrix;
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_normal_matrix;
uniform float u_interpolation;

layout (location = 0) in vec3 vsin_position;
layout (location = 1) in vec2 vsin_texture_coord_1;
layout (location = 2) in vec3 vsin_normal;
layout (location = 3) in vec3 vsin_next_position;
layout (location = 4) in vec3 vsin_next_normal;

smooth out vec3 fragment_normal;            // 从vs输出到fs的片元的法线
smooth out vec2 fragment_texture_coord_1;   // 从vs输出到fs的片元的第一层纹理贴图坐标
smooth out vec3 fragment_world_space_pos;   // 从vs输出到fs的片元的基于世界空间的坐标值
smooth out vec4 fragment_view_space_pos;    // 从vs输出到fs的片元的基于观察空间的坐标值

void main()
{
    mat4 model_view_matrix = u_view_matrix*u_model_matrix;  
    mat4 model_view_project_matrix = u_projection_matrix * u_view_matrix * u_model_matrix;

    fragment_texture_coord_1 = vsin_texture_coord_1;

    vec3 interpolated_position = vsin_position;

    // 如果存在着插值系数，则根据本帧的position和下一关键帧的position，算出在当前时刻应有的顶点position
    if(u_interpolation >= 0.0)
        interpolated_position += (vsin_next_position - vsin_position) * u_interpolation;

    // 将当前时刻应有的顶点position，变换到观察空间，以及变换到裁剪空间
    fragment_view_space_pos = model_view_matrix*vec4(interpolated_position, 1.0);
    gl_Position = model_view_project_matrix*vec4(interpolated_position, 1.0);

    // 如果存在着插值系数，则根据本帧的position和下一关键帧的position，算出在当前时刻应有的顶点position
    vec3 interpolated_normal = vsin_normal;
    
    if(u_interpolation >= 0.0f)
        interpolated_normal += (vsin_next_normal - vsin_normal) * u_interpolation;

    // 将插值得到的法线也转换之
    fragment_normal = (u_normal_matrix * vec4(interpolated_normal, 1.0)).xyz;

    // 得到片元在世界坐标系下的位置值
    fragment_world_space_pos = (u_model_matrix*vec4(interpolated_position, 1.0)).xyz;
}