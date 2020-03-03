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
// 这是这个渲染过程的fragment shader
/*
out vec3 v2f_position; 
out vec3 v2f_normal; 
out vec3 v2f_camera_position; 
out float v2f_terrain_height;
*/
#version 330

in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCameraVector;
in vec3 fromLightVector;

out vec4 out_Color;

uniform sampler2D u_texture_reflection;
uniform sampler2D u_texture_refraction;
uniform sampler2D u_dudv_map;
uniform sampler2D u_normal_map;
uniform sampler2D u_depth_map;
uniform vec3 u_light_color;
uniform float u_move_factor;

const float waveStrength = 0.04;
const float shineDamper = 20.0;
const float reflectivity = 0.5;

const float near = 0.1;
const float far = 1000.0;


void main()
{
    vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
    vec2 reflectTexCoords = textureCoords; reflectTexCoords.y = 1 - reflectTexCoords.y;//vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
    vec4 reflectColor = texture(u_texture_reflection,reflectTexCoords);// reflectTexCoords);
    out_Color = reflectColor;// * vec4(1.0,1.0,1.0,0.3);
    /*
    vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
    vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);

    float depth = texture(u_depth_map, refractTexCoords).r;
    float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

    depth = gl_FragCoord.z;
    float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
    float waterDepth = floorDistance - waterDistance;

    vec2 distortedTexCoords = texture(u_dudv_map, vec2(textureCoords.x + u_move_factor, textureCoords.y)).rg * 0.1;
    distortedTexCoords = textureCoords + vec2(distortedTexCoords.x, distortedTexCoords.y + u_move_factor);
    vec2 totalDistortion = (texture(u_dudv_map, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength * clamp(waterDepth / 20.0, 0.0, 1.0);

    reflectTexCoords.x = clamp(reflectTexCoords.x + totalDistortion.x, 0.001, 0.999);
    reflectTexCoords.y = clamp(reflectTexCoords.y + totalDistortion.y, -0.999, -0.001);
    refractTexCoords = clamp(refractTexCoords + totalDistortion, 0.001, 0.999);

    vec4 reflectColor = texture(u_texture_reflection, reflectTexCoords);
    vec4 refractColor = texture(u_texture_refraction, refractTexCoords);

    vec4 normalMapColor = texture(u_normal_map, distortedTexCoords);
    vec3 normal = normalize(vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 3.0, normalMapColor.g * 2.0 - 1.0));

    vec3 viewVector = normalize(toCameraVector);
    float refractiveFactor = dot(viewVector, normal);
    refractiveFactor = pow(refractiveFactor, 0.5);
    refractiveFactor = clamp(refractiveFactor, 0.0, 1.0);

    vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
    float specular = max(dot(reflectedLight, viewVector), 0.0);
    specular = pow(specular, shineDamper);
    vec3 specularHighlights = u_light_color * specular * reflectivity * clamp(waterDepth / 5.0, 0.0, 1.0);

    out_Color = mix(reflectColor, refractColor, refractiveFactor);
    out_Color = mix(out_Color, vec4(0.0, 0.3, 0.5, 1.0), 0.2) + vec4(specularHighlights, 0.0);
    out_Color.a = clamp(waterDepth / 5.0, 0.0, 1.0);
    */
    // out_Color = vec4(1.0,1.0,1.0,1.0);
}
