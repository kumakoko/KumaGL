/**************************************************************************************************************************
Copyright(C) 2014-2019 www.xionggf.com

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
// 绘制场景用到的顶点着色器
#version 330 

layout (location = 0) in vec3 vsin_position; 
layout (location = 1) in vec2 vsin_texture_coord_0; 
layout (location = 2) in vec3 vsin_normal; 
// layout (location = 3) in vec3 vsin_caustic_normal;

uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;
uniform vec4 u_water_plane;
out vec2  v2f_texture_coord_0; 
out vec3  v2f_normal;
// out vec3  v2f_caustic;
out float v2f_height;

void main()
{
	gl_ClipDistance[0] = dot(vec4(vsin_position, 1.0), u_water_plane);
    gl_Position =  u_projection_matrix * u_view_matrix * vec4(vsin_position, 1.0); 
    v2f_texture_coord_0 = vsin_texture_coord_0; 
    v2f_normal = normalize(vsin_normal);
    // v2f_caustic = normalize(vsin_caustic_normal);
    v2f_height = vsin_position.y;
}

