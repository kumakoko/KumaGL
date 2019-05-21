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

#version 330 

in vec2  v2f_texture_coord_0; 
in vec3  v2f_normal;
// in vec3  v2f_caustic_normal;
in float v2f_height;

uniform sampler2D u_texture_sand; 
uniform sampler2D u_texture_grass; 
uniform sampler2D u_texture_rocks; 
uniform sampler2D u_texture_snow; 
uniform sampler2D u_texture_forest; 
uniform sampler2D u_texture_tundra; // 苔原纹理贴图

out vec4 Color; 

/************************************************************************************
vec3 normal_at  片元的法线
vec3 sun        第一个光源的入射光线
vec3 sun2       第二个光源的入射光线
************************************************************************************/
float getLightValue(vec3 normal_at, vec3 sun, vec3 sun2, float light_up) 
{
    // 计算片元对每个
    float light  = max(0,dot(normal_at, sun));
    float light1 = max(0,dot(normal_at, sun2));
    
    // 对两个光源的反射值进行混合
    return mix(min(1,light + light_up), min(1,light1 + light_up), 0.5);
}

vec4 calcTexChange(float height, float min, float max, vec4 tex1, vec4 tex2)
{
    float aspect = (height-min) / (max-min);
    return mix(tex1, tex2, aspect);
}

void main()
{
    float sandGrassFrom   = -3.0, sandGrassTo         = 3.0,
          grassForestFrom = 4.0,  grassForestTo   = 7.0,
          forestTundraFrom = 16.0, forestTundraTo = 25.0,
          tundraRocksFrom = 25.0, tundraRocksTo   = 30.0,
          rocksSnowFrom   = 40.0, rocksSnowTo     = 60.0;

    // 两个光源的照射方向想
    vec3 sun  = vec3(1,1,1);
    vec3 sun2 = vec3(-1,1,1);
    vec4 sand  = texture(u_texture_sand , v2f_texture_coord_0); 
    vec4 grass = texture(u_texture_grass, v2f_texture_coord_0*3.0); 
    vec4 forest = texture(u_texture_forest, v2f_texture_coord_0*3.0);
    vec4 rocks = texture(u_texture_rocks, v2f_texture_coord_0);
    vec4 tundra = texture(u_texture_tundra, v2f_texture_coord_0*5.0);
    vec4 snow = texture(u_texture_snow, v2f_texture_coord_0*5.0);
    vec4 color;

    // 跟踪不同的高度值，用不同的纹理进行混合计算，依据高度变化覆盖上不同地貌效果
    if (v2f_height < sandGrassFrom)
    {
        color = sand;
    }
    else if (v2f_height < sandGrassTo)
    {
        color = calcTexChange(v2f_height, sandGrassFrom, sandGrassTo, sand, grass);
    }
    else if (v2f_height < grassForestFrom)
    {
        color = grass;
    }
    else if (v2f_height < grassForestTo)
    {
        color = calcTexChange(v2f_height, grassForestFrom, grassForestTo, grass, forest);
    } 
    else if (v2f_height < forestTundraFrom)
    {
        color = forest;
    }
    else if (v2f_height < forestTundraTo)
    {
        color = calcTexChange(v2f_height, forestTundraFrom, forestTundraTo, forest, tundra);
    }
    else if (v2f_height < tundraRocksFrom)
    {
        color = tundra;
    }
    else if (v2f_height < tundraRocksTo)
    {
        color = calcTexChange(v2f_height, tundraRocksFrom, tundraRocksTo, tundra, rocks);
    }
    else if (v2f_height < rocksSnowFrom)
    {
        color = rocks;
    }
    else if (v2f_height < rocksSnowTo)
    {
        color = calcTexChange(v2f_height, rocksSnowFrom, rocksSnowTo, rocks, snow);
    }
    else
    {
        color = snow;
    }
        
    color = color * getLightValue(v2f_normal, sun, sun2, 0.7);
    
    // Caustics
    //color = color * getLightValue(v2f_caustic_normal, sun, sun2, 0.5);
    //float amount = dot(v2f_caustic_normal, sun);
    //vec4 white = vec4(1,1,1,1);
    //vec4 caustics = mix(color, white, amount);
    
    Color =  color;
}
