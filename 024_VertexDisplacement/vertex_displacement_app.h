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
/*!
 * \file vertex_displacement_app.h
 * \date 2019/03/19 13:18
 *
 * \author www.xionggf.com
 * Contact: sun_of_lover@sina.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef vertex_displacement_h__
#define vertex_displacement_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_draw_mode.h"

class VertexDisplacementApp : public kgl::App
{
public:
	VertexDisplacementApp();
	virtual ~VertexDisplacementApp();
protected:
    virtual void InitModel() override;
    virtual void RenderScene() override ;
	virtual void InitMainCamera() override;
	virtual void RenderGUI() override;
private:
    kgl::GPUProgram*         gpu_program_ = nullptr;
    kgl::PrimitiveSPtr       rectangle_primitive_;
    kgl::SourceTextureSPtr   texture_1_;
	float					 current_angle_ = 0.f;
	float					 speed_of_ratation_ = 10.f;
	double					 last_frame_time_ = 0.0f;
	float					 flag_left_top_x_ = -4.f;
	float					 flag_left_top_y_ = 2.5f;
	bool					 wireframe_mode_ = false;
	kgl::RenderStateDrawMode draw_mode_;
};

#endif // vertex_displacement_h__