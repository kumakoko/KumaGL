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