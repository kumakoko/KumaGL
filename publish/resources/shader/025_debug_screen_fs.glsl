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
// �����۲쵱ǰ��Ӱ��ͼ��fragment shader

#version 330 core
out vec4 fragment_color;

in vec2 v2f_texture_coords;

uniform sampler2D u_depth_map;
uniform float u_near_plane;
uniform float u_far_plane;

// �����ֵ���Ի�֮
float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * u_near_plane * u_far_plane) / (u_far_plane + u_near_plane - z * (u_far_plane - u_near_plane));
}

// ʹ��͸�ӽǶ�ʱ����Ȼ�������ɫ
vec4 PerspectiveColor(float depth_value)
{
	return vec4(vec3(LinearizeDepth(depth_value) / u_far_plane), 1.0);
}

// ʹ������͸�ӽǶ�ʱ����Ȼ�������ɫ
vec4 Orthographic(float depth_value)
{
	return vec4(vec3(depth_value), 1.0);
}

void main()
{
	float depth_value = texture(u_depth_map, v2f_texture_coords).r;
	// fragment_color = PerspectiveColor(depth_value); // ͸��
	fragment_color = Orthographic(depth_value); // ����
}