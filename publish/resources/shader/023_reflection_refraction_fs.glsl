/*************************************************************************************
Copyright(C) 2014-2019 www.xionggf.com

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
 /*
in vec3 normal0;
in vec2 texCoord0;
in vec4 clipSpace;
in vec3 worldPos0;
 
out vec4 outColor;

out vec4 fragment_color;
*/
struct VS2FS // 同VS中的定义
{
    vec3 Position;   // 片元的经过World变换后的坐标，即世界坐标值
    vec3 Normal;     // 片元的基于世界坐标系下的坐标，把顶点的法线
                     // 从局部模型坐标系变换到世界坐标系，需要乘以
                     // world变换矩阵的逆矩阵
    vec2 TexCoords_1;// 片元的第一层纹理映射坐标
};

in VS2FS v2f;   // 从vs输出到fs的结构体

uniform sampler2D reflectionTexture;	// 反射效果用到的贴图
uniform sampler2D refractionTexture;	// 折射效果用到的贴图
 
void main()
{
	vec3 ndc = (clipSpace.xyz / clipSpace.w)/ 2.0 + 0.5;
 
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
 
	vec4 reflectColor = texture2D(reflectionTexture, reflectTexCoords);
	vec4 refractionColor = texture2D(refractionTexture, refractTexCoords);
 
    outColor = reflectColor;
}