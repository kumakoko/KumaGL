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
// 光照计算
// #version 330 core 因为这里要include其他的代码，所以不用在这里指定版本了

in vec3 fs_normal;
in vec3 fs_frag_pos_world;  
uniform vec3                view_pos;           // 视点（摄像机）的位置，基于世界坐标
uniform LightingMaterial    material;           // 物体表面材质，在lighting_struct.glsl文件中定义
uniform DirectionalLight    directional_light;  // 从外部传入的有向平行光，在lighting_struct.glsl文件中定义
uniform PointLight			point_light;
uniform SpotLight			spot_light;

out vec4 color;

void main()
{
    // 传递进来的有向平行光执行光照计算
    color = PhongDirectionalLighting(fs_frag_pos_world,fs_normal,view_pos,directional_light,material);
	color += PhongPointLighting(fs_frag_pos_world,fs_normal,view_pos,point_light,material);
	color += PhongSpotLighting(fs_frag_pos_world,fs_normal,view_pos,spot_light,material);
}