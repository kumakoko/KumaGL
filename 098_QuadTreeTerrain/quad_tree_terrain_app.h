/*!
 * \file quad_tree_terrain_app.h
 * \date 2017/12/31 14:38
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 渲染若干个的三角形      
 *
 * TODO:  演示如何使用基础图元类kgl::Primitve
 *        演示如何使用着色器类kgl::GPUProgram
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
#ifndef quad_tree_terrain_app_h__
#define quad_tree_terrain_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"

class QuadTreeTerrainApp : public kgl::App
{
public:
    QuadTreeTerrainApp();
    virtual ~QuadTreeTerrainApp();
protected:
    virtual void InitModel() override;
    virtual void RenderScene() override;
private:
    kgl::GPUProgram* gpu_program_;
    kgl::Primitive*  rectangle_primitive_;
    kgl::Primitive*  triangle_primitive_1_;
    kgl::Primitive*  triangle_primitive_2_;
    kgl::Primitive*  triangle_primitive_3_;
    kgl::Primitive*  triangle_primitive_4_;
    double time = 0, dt;// Current time and enlapsed time
    double turn = 0;    // Model turn counter
    double speed = 0.3; // Model rotation speed
    int wire = 0;       // Draw model in wireframe?
    std::array<float,3> bgColor;         // Background color 
    std::array<unsigned char, 4> cubeColor; // Model color (32bits RGBA)
};


#endif // quad_tree_terrain_app_h__