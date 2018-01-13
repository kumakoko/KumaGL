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

in vec2 v2f_tex_coords_1;

uniform sampler2D u_hdr_buffer; // 从离屏的frame buffer中读取到纹理
uniform bool u_use_hdr;
uniform float u_exposure;

void main()
{             
    const float gamma = 2.2;
    vec3 hdr_color = texture(u_hdr_buffer, v2f_tex_coords_1).rgb;

    if(u_use_hdr)
    {
        vec3 result = vec3(1.0) - exp(-hdr_color * u_exposure);
        result = pow(result, vec3(1.0 / gamma));
        fragment_color = vec4(result, 1.0);
    }
    else
    {
        vec3 result = pow(hdr_color, vec3(1.0 / gamma));
        fragment_color = vec4(result, 1.0);
    }
}