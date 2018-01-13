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

layout(location = 0) in vec3 vsin_position;
layout(location = 1) in vec3 vsin_normal;
layout(location = 2) in vec4 vsin_color;

// 使用flat指令告诉顶点的颜色不用插值
flat out vec3 pass_color;

uniform vec3 u_light_direction;
uniform vec3 u_light_color;
uniform vec2 u_light_bias;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

// 简单的计算漫反射光照的函数
vec3 calculateLighting()
{
    // 把传递进来的法线的分量的取值范围限制在（0，1）
    vec3 normal = vsin_normal.xyz * 2.0 - 1.0;
    float brightness = max(dot(-u_light_direction, normal), 0.0);
    return (u_light_color * u_light_bias.x) + (brightness * u_light_color * u_light_bias.y);
}

void main()
{
    gl_Position = u_projection_matrix * u_view_matrix * vec4(vsin_position, 1.0);
    vec3 lighting = calculateLighting();
    pass_color = vsin_color.rgb * lighting; // 光照颜色乘以顶点颜色作为片元的颜色
}