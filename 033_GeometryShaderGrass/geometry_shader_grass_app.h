﻿/**************************************************************************************************************************
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
/*!
* \file frame_buffer_app.h
* \date 2017/12/31 15:01
*
* \author www.xionggf.com
* Contact: sun_of_lover@sina.com
*
* \brief
*
* TODO:
*
* \note
*/
#ifndef frame_buffer_app_h__
#define frame_buffer_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_rendered_texture.h"
#include "../klib/kgl_frame_buffer.h"
#include "../klib/kgl_vertex_buffer_object.h"

class GeometryShaderGrassApp : public kgl::App
{
public:
	GeometryShaderGrassApp();
	virtual ~GeometryShaderGrassApp();
	virtual void InitScene();
protected:
	virtual void PreRenderFrame() override;
	virtual void RenderScene() override;
	virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode) override;
	virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos) override;
	virtual void OnScrollAction(GLFWwindow* window, double xoffset, double yoffset) override;
	virtual void ProcessInput() override;
	virtual void InitMaterials() override;
	virtual void InitModel() override;
	virtual void InitShaders() override;
	virtual void InitLights() override;
	virtual void InitMainCamera() override;
	virtual void InitFont() override;
	void CreateGrassData();
	void RenderGrass();
private:
	kgl::GPUProgramSPtr			scene_shader_;
	kgl::GPUProgramSPtr			grass_shader_;
	kgl::PrimitiveSPtr			floor_; // 用来绘制地板的图元
	kgl::SourceTextureSPtr		floor_texture_; // 用来绘制地板的纹理
	kgl::SourceTextureSPtr		grass_texture_; // 用来绘制地板的纹理
	kgl::RenderStateDepth		draw_fb_to_screen_depth_;
	kgl::RenderStateCullMode	cull_mode_;
	kgl::RenderStateDepth		depth_test_mode_;
	kgl::RenderStateCullMode	draw_fb_to_scr_cull_;
	kgl::RenderStateDepth		draw_fb_to_scr_depth_;
	kgl::VertexBufferObject		vboGrassData;
	GLint						iNumGrassTriangles;
	GLuint						uiGrassVAO;
	float						fTimePassed;
	glm::vec3					vRenderScale;
	glm::vec3					floor_translation_;
};



#endif // frame_buffer_app_h__