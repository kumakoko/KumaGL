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
// 阴影着色顶点shader
#version 330 core
out vec4 fragment_color;

in V2F
{
    vec3 fragment_position;
    vec3 fragment_normal;
    vec2 fragment_texture_coords;
    vec4 fragment_pos_in_light_space;
} fs_in;

uniform sampler2D u_diffuse_texture;
uniform sampler2D u_shadow_map;

uniform vec3 u_light_world_pos;
uniform vec3 u_camera_world_pos;

uniform bool u_bias_and_sample; // 计算阴影时是否使用bias和sample进行阴影质量提升

float ShadowCalculation(vec4 frag_pos_light_space)
{
    // 透视除法，如果使用正交投影的话此步骤无意义但不影响正确结构，所以就保留之
    vec3 projection_coords = frag_pos_light_space.xyz / frag_pos_light_space.w;
    
    // 投影变换坐标的Z值是在[-1,1]的，现在将其变换到
    // [0,1]的范围，用作为shadow map的纹理取值坐标
    projection_coords = projection_coords * 0.5 + 0.5;
    
    // 取得最近点的深度(使用[0,1]范围下的frag_pos_light_space当坐标)
    float closest_depth = texture(u_shadow_map, projection_coords.xy).r;
    
    // 取得当前片元在光源视角下的深度projection_coords.z，检
    // 查当前片元是否在阴影中，在的话返回1，不在就返回0
    return projection_coords.z > closest_depth  ? 1.0 : 0.0;
}

// 计算深度阴影，同时利用深度偏移值和多重采样优化阴影贴图效果
float ShadowCalculationWithBiasAndSample(vec4 frag_pos_light_space)
{
    vec3 projection_coords = frag_pos_light_space.xyz / frag_pos_light_space.w;
    projection_coords = projection_coords * 0.5 + 0.5;
    float closest_depth = texture(u_shadow_map, projection_coords.xy).r; 
    float currentDepth = projection_coords.z;

    // 使用了偏移量后，所有采样点都获得了比表面深度更小的深度值，这样整个表面就正确地被照亮，
    // 解决了大部分的阴影失真的问题，但是在有些表面坡度很大的情况下，仍然会产生阴影失真。更可
    // 靠的办法是根据表面朝向光线的角度更改偏移量：使用点乘:
    // 表面和光照法线的夹角较大者，其点积值就越小，得到的bias值就越大
    vec3 normal = normalize(fs_in.fragment_normal);
    vec3 light_direction = normalize(u_light_world_pos - fs_in.fragment_position);
    float bias = max(0.05 * (1.0 - dot(normal, light_direction)), 0.005);

    // 用percentage-closer filtering进行采样，使得阴影的边缘尽量柔和
    // textureSize库函数将返回当前LOD级别下的纹理的高宽
    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(u_shadow_map, 0);
    
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcf_depth = texture(u_shadow_map, projection_coords.xy + vec2(x, y) * texel_size).r; 
            shadow += currentDepth - bias > pcf_depth  ? 1.0 : 0.0;        
        }    
    }
    
    shadow /= 9.0;
    
    // 保持阴影不要超出far_plane，超出者，阴影值即硬性设置为0，即没阴影
    if(projection_coords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

vec4 CalculateLightAndShadow()
{
    vec3 color = texture(u_diffuse_texture, fs_in.fragment_texture_coords).rgb;
    vec3 normal = normalize(fs_in.fragment_normal);
    vec3 light_color = vec3(0.3);

    // 计算环境光分量
    vec3 ambient = light_color * color;
    
    // 计算漫反射光分量
    vec3 light_direction = normalize(u_light_world_pos - fs_in.fragment_position);
    float diff = max(dot(light_direction, normal), 0.0);
    vec3 diffuse = diff * light_color;
    
    // 计算镜面反射光分量，基于Blinn-Phong光照模型
    vec3 view_direction = normalize(u_camera_world_pos - fs_in.fragment_position);
    vec3 reflect_direction = reflect(-light_direction, normal);
    vec3 halfway_direction = normalize(light_direction + view_direction);
    float spec = pow(max(dot(normal, halfway_direction), 0.0), 64.0);
    vec3 specular = spec * light_color;

    // 计算阴影
    float shadow = 0.0;

    if (u_bias_and_sample == true)
        shadow = ShadowCalculationWithBiasAndSample(fs_in.fragment_pos_in_light_space);
    else
        shadow = ShadowCalculation(fs_in.fragment_pos_in_light_space);

    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    return vec4(lighting, 1.0);
}

void main()
{
    fragment_color = CalculateLightAndShadow();
}