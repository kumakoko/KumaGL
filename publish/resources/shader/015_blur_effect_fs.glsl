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
// 对texture进行模糊效果的片元着色器

#version 330 core

out vec4 final_color;
in vec2 fsin_tex_coords;
// 由vs直接输出到fs的纹理贴图坐标值
uniform sampler2D image;
// 片元着色器进行采样的纹理
uniform int horizontal;
// 值为true时表示对纹理进行水平方向的模糊偏移
uniform int use_blur;
// 大于0的话使用blur效果，否则直接使用输出
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);
// 执行高斯模糊运算时的权重值

void main()
{
    if (use_blur > 0)
    {
        // textureSize函数细节参见网页 ： https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/textureSize.xhtml
        // 取得LOD级别为0，即第一级的texture的高宽，然后以高宽的倒数为一个纹素偏移度
        vec2 tex_offset = 1.0 / textureSize(image, 0);
        // 对某个指定的纹理映射坐标对应的颜色，乘以第一个权重值
        vec3 result = texture(image, fsin_tex_coords).rgb * weight[0];

        if(horizontal > 0)
        {
            // 水平方向上，对某个指定的纹理映射坐标，向左取得4个（偏移得到的纹素颜色 * 权重值），
         // 向右取得4个（偏移得到的纹素颜色 * 权重值），共9个数值。然后累加得到某指定的纹理映射坐标颜色的值
            for(int i = 1; i < 5; ++i)
            {
                result += texture(image, fsin_tex_coords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
                result += texture(image, fsin_tex_coords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            }
        }
        else
        {
            // 垂直方向上，对某个指定的纹理映射坐标，向上取得4个（偏移得到的纹素颜色 * 权重值），
            // 向下取得4个（偏移得到的纹素颜色 * 权重值），共9个数值。然后累加得到某指定的纹理映射坐标颜色的值
            for(int i = 1; i < 5; ++i)
            {
                result += texture(image, fsin_tex_coords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
                result += texture(image, fsin_tex_coords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            }
        }

        final_color = vec4(result, 1.0);
    }
    else
    {
        final_color = vec4(texture(image, fsin_tex_coords).rgb,1.0);
    }
}