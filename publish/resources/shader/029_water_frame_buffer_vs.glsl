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
// 把周围环境渲染到一个frame buffer上，然后把frame buffer贴到一个水面模型中去的
// 这是这个渲染过程的vertex shader
#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vsin_normal;
layout (location = 2) in vec2 texture_coord_1;


out vec4 clipSpace;
out vec2 textureCoords;
out vec3 toCameraVector;
out vec3 fromLightVector;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_world_matrix;
uniform vec3 u_camera_position;
uniform vec3 u_light_position;

const float tiling = 4.0;


void main(void) 
{
    vec4 worldPosition = u_world_matrix * vec4(position.x,0.0,position.y,1.0);
    clipSpace = u_projection_matrix * u_view_matrix * worldPosition;
    gl_Position = u_projection_matrix * u_view_matrix * u_world_matrix * vec4(position.x,position.y,position.z,1.0);// clipSpace;
    // textureCoords = (position.xy / 2.0 + 0.5) * tiling;
	textureCoords = texture_coord_1;
    toCameraVector = u_camera_position - worldPosition.xyz;
    fromLightVector = worldPosition.xyz - u_light_position;
}