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
// 基于公告版技术的粒子系统的顶点着色器
#version 330 core

layout (location = 0) in vec3 vsin_position;     	// 顶点位置坐标，基于世界空间
layout (location = 1) in vec4 vsin_color; 			// 顶点法线
layout (location = 2) in vec4 vsin_tex_coord_1;		// 顶点纹理坐标，

out VSOutput 
{
	vec4 position;     	// 顶点位置坐标，基于世界空间
	vec4 color; 			// 顶点法线
	vec4 tex_coord_1;		// 顶点切线;
}vs_out;


uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform vec4 size;
uniform mat4 preRotatedQuad;//[4]

void main( )
{
	mat4 modelViewProj = projection_matrix * view_matrix;
    vec4 vCenter = float4(vsin_position.x, vsin_position.y, vsin_position.z, 1.0);
    vs_out.position = modelViewProj * (vCenter + preRotatedQuad[vsin_tex_coord_1.z] * vsin_tex_coord_1.w);//mul( modelViewProj, vCenter + preRotatedQuad[IN.texcoord0.z] * IN.texcoord0.w );
    vsin_tex_coord_1.z = 0.0f; // Remove the hidden information from the z component
    vsin_tex_coord_1.w = 1.0f; // Remove the hidden information from the w component
    vs_out.color0    = vsin_color;
    vs_out.tex_coord_1 = vsin_tex_coord_1;
}







/*
struct appin
{
    float4 position  : POSITION;
    float4 color0    : COLOR0;
    float4 texcoord0 : TEXCOORD0;
};

struct vertout
{
    float4 position  : POSITION;
    float4 color0    : COLOR0;
    float4 texcoord0 : TEXCOORD0;
};

//-----------------------------------------------------------------------------
// IN             - incoming per-vertex data to be processed
// modelViewProj  - combined world-view-projection matrix
// size           - size factor for billboard creation
// preRotatedQuad - a pre-rotated quad for billboarding purposes
//
// Note: Only the x and y components of texcoord0 are valid, the z and w 
//       components contain hidden information. The z component identifies
//       which vertex of the billboard's quad is currently being processed and 
//       the w component carries a scaling factor for sizing the billboard.
//-----------------------------------------------------------------------------

vertout main( appin IN,
              const uniform float4x4 modelViewProj, 
              const uniform float4   size,
              const uniform float4   preRotatedQuad[4] )

{
    vertout OUT;

    // Build-up a float4 position from a float3 position and then transform it.
    float4 vCenter = float4( IN.position.x, IN.position.y, IN.position.z, 1.0f );

    OUT.position = mul( modelViewProj, vCenter + preRotatedQuad[IN.texcoord0.z] * IN.texcoord0.w );

    IN.texcoord0.z = 0.0f; // Remove the hidden information from the z component
    IN.texcoord0.w = 1.0f; // Remove the hidden information from the w component

    OUT.color0    = IN.color0;
    OUT.texcoord0 = IN.texcoord0;

    return OUT;
}
*/






