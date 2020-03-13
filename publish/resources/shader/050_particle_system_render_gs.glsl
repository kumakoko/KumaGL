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
// 粒子系统的几何着色器
#version 330

uniform struct Matrices
{
    mat4 mProj;
    mat4 mView;
} matrices;

uniform vec3 vQuad1, vQuad2;

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

in vec3 vColorPass[];
in float fLifeTimePass[];
in float fSizePass[];
in int iTypePass[];

smooth out vec2 vTexCoord;
flat out vec4 vColorPart;

void main()
{
  if(iTypePass[0] != 0)
  {
    vec3 vPosOld = gl_in[0].gl_Position.xyz;
    float fSize = fSizePass[0];
    mat4 mVP = matrices.mProj*matrices.mView;
    
    vColorPart = vec4(vColorPass[0], fLifeTimePass[0]);
       
    vec3 vPos = vPosOld+(-vQuad1-vQuad2)*fSize;
    vTexCoord = vec2(0.0, 0.0);
    gl_Position = mVP*vec4(vPos, 1.0);
    EmitVertex();
    
    vPos = vPosOld+(-vQuad1+vQuad2)*fSize;
    vTexCoord = vec2(0.0, 1.0);
    gl_Position = mVP*vec4(vPos, 1.0);
    EmitVertex();
    
    vPos = vPosOld+(vQuad1-vQuad2)*fSize;
    vTexCoord = vec2(1.0, 0.0);
    gl_Position = mVP*vec4(vPos, 1.0);
    EmitVertex();
    
    vPos = vPosOld+(vQuad1+vQuad2)*fSize;
    vTexCoord = vec2(1.0, 1.0);
    gl_Position = mVP*vec4(vPos, 1.0);
    EmitVertex();
      
    EndPrimitive();
  }
}