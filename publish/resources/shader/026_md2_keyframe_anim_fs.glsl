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
// 用来对关键帧进行插值，形成动画效果的片元着色器
// 可以用在对md2模型文件的显示

#version 330

smooth in vec3 fragment_normal;            // 从vs输出到fs的片元的法线
smooth in vec2 fragment_texture_coord_1;   // 从vs输出到fs的片元的第一层纹理贴图坐标
smooth in vec3 fragment_world_space_pos;   // 从vs输出到fs的片元的基于世界空间的坐标值
smooth in vec4 fragment_view_space_pos;    // 从vs输出到fs的片元的基于观察空间的坐标值

out vec4 fragment_color;

uniform sampler2D model_texture;

void main()
{
     fragment_color = texture2D(model_texture, fragment_texture_coord_1);
}