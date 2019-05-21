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
// 使用视差贴图，实现凹凸效果的片元着色器
#version 330 core

out vec4 fragment_color;

in VSOutput 
{
	vec2 fragment_tex_cooord_1;			// 第一层纹理坐标
	vec3 camera_dir_in_tangent_space;	// 摄像机位置点到顶点的连线向量，变换到切线空间后的值
}fs_in;

uniform sampler2D diffuse_map;
uniform sampler2D height_map;
uniform vec2 offset_value;       // x分量为偏移倍数值,y分量为偏移差数值

void main()
{   
	vec3 camera_dir = normalize(fs_in.camera_dir_in_tangent_space);
    vec4 offset_uv = texture(height_map,fs_in.fragment_tex_cooord_1);
    offset_uv = offset_uv * offset_value.x - offset_value.y;
    vec2 offset_tex_coords = offset_uv.xy * camera_dir.xy + fs_in.fragment_tex_cooord_1;
    fragment_color = texture( diffuse_map, offset_tex_coords ); 
}