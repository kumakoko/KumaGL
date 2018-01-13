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

out vec4 fragment_color;

struct VS2FS // 同VS中的定义
{
    vec3 Position;   // 片元的经过World变换后的坐标，即世界坐标值
    vec3 Normal;     // 片元的基于世界坐标系下的坐标，把顶点的法线
                     // 从局部模型坐标系变换到世界坐标系，需要乘以
                     // world变换矩阵的逆矩阵
    vec2 TexCoords_1;// 片元的第一层纹理映射坐标
};

in VS2FS v2f;   // 从vs输出到fs的结构体

// 一个简单的光源结构体
struct SimpleLight
{
    vec3 Position;
    vec3 Color;
};

const int LIGHTS_COUNT = 16;
uniform SimpleLight lights[LIGHTS_COUNT];
uniform sampler2D u_diffuse_texture;

void main()
{
    vec3 color = texture(u_diffuse_texture, v2f.TexCoords_1).rgb;
    vec3 normal = normalize(v2f.Normal);
    vec3 ambient = 0.01 * color; // 由漫反射颜色提供一个很微弱的环境光

    vec3 lighting = vec3(0.0); // 光照计算的值
    
    for(int i = 0; i < LIGHTS_COUNT; i++)
    {
        // 计算漫反射分量
        vec3 light_dir = normalize(lights[i].Position - v2f.Position);
        float diff = max(dot(light_dir,normal), 0.0);
        vec3 diffuse = lights[i].Color * diff * color;      
        vec3 result = diffuse;
        // 计算衰减值
        float frag_to_light_distance = length(v2f.Position - lights[i].Position);
        result *= 1.0 / (frag_to_light_distance * frag_to_light_distance);
        lighting += result; // 累加光照结果
    }
    
    fragment_color = vec4(ambient + lighting, 1.0);
}