// 灯光类定义
/*!
 * \file kgl_light.h
 * \date 2017/12/31 13:33
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
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
#ifndef kgl_light_h__
#define kgl_light_h__

namespace kgl
{
    struct DirectionalLight 
    {
        glm::vec3 Direction;
        glm::vec3 Ambient;
        glm::vec3 Diffuse;
        glm::vec3 Specular;
    };

    struct DirLightUniformLocation
    {
        GLint Direction;
        GLint Ambient;
        GLint Diffuse;
        GLint Specular;
    };

    struct PointLight
    {
        glm::vec3 Ambient;
        glm::vec3 Diffuse;
        glm::vec3 Specular;
        glm::vec3 Position;         // 点光源在世界空间中的坐标位置
        float AttenuationConstant;  // 常数衰减值
        float AttenuationLinear;    // 线性衰减值
        float AttenuationExp;       // 指数衰减值
    };

    struct PointLightUniformLocation
    {
        GLint Ambient;
        GLint Diffuse;
        GLint Specular;
        GLint Position;         // 点光源在世界空间中的坐标位置
        GLint AttenuationConstant;  // 常数衰减值
        GLint AttenuationLinear;    // 线性衰减值
        GLint AttenuationExp;       // 指数衰减值
    };

    struct SpotLight
    {
        glm::vec3 Ambient;
        glm::vec3 Diffuse;
        glm::vec3 Specular;
        glm::vec3 Position;         // 点光源在世界空间中的坐标位置
        glm::vec3 Direction;        // 探照灯的照射方向
        glm::vec4 CutOffAndAttenuation; // 4个分量分别是cut off ，常数衰减值，线性衰减值，指数衰减值
        float     Exponent;
    };

    struct SpotLightUniformLocation
    {
        GLint Ambient;
        GLint Diffuse;
        GLint Specular;
        GLint Position;         // 点光源在世界空间中的坐标位置
        GLint Direction;        // 探照灯的照射方向
        GLint CutOffAndAttenuation; // 4个分量分别是cut off ，常数衰减值，线性衰减值，指数衰减值
        GLint Exponent;
    };

    
    /// <summary>
    /// 用来渲染low polygon terrain的简单灯光
    /// </summary>
    struct LowPolyTerrainLight
    {
        glm::vec3 Direction;
        glm::vec3 Color;
        glm::vec2 Bias;
    };
}
#endif // kgl_light_h__