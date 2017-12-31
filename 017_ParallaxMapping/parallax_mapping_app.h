/*!
 * \file parallax_mapping_app.h
 * \date 2017/12/31 15:25
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
#ifndef parallax_mapping_app_h__
#define parallax_mapping_app_h__

#include "../klib/kgl_lib_pch.h"
#include "../klib/kgl_app.h"
#include "../klib/kgl_gpu_program.h"
#include "../klib/kgl_primitive.h"
#include "../klib/kgl_source_texture.h"
#include "../klib/kgl_render_state_cull_mode.h"
#include "../klib/kgl_render_state_depth.h"
#include "../klib/kgl_sky_box.h"
#include "../klib/kgl_basic_static_mesh.h"
#include "../klib/kgl_light.h"

class ParallaxMappingApp : public kgl::App
{
public:
	ParallaxMappingApp();
	virtual ~ParallaxMappingApp();
	virtual void InitScene();
protected:
	virtual void RenderFrame() override;
	virtual void OnKeyAction(GLFWwindow* window, int key, int scancode, int action, int mode) override;
	virtual void OnMouseAction(GLFWwindow* window, double xpos, double ypos) override;
	virtual void OnScrollAction(GLFWwindow* window, double xoffset, double yoffset) override;
	virtual void ProcessInput() override;
	virtual void InitModel() override;
	virtual void InitShaders() override;
	virtual void InitMainCamera() override;
	virtual void InitLights() override;
	virtual void InitMaterials() override;
	void renderQuad();
	void InitNDCScreeenRectangle();
private:
	/// <summary>
	/// The model_
	/// </summary>
	kgl::BasicStaticMesh* model_ = nullptr;

	/// <summary>
	/// The model_shader_
	/// </summary>
	kgl::GPUProgramSPtr model_shader_;

	/// <summary>
	/// The rs_depth_
	/// </summary>
	kgl::RenderStateDepth rs_depth_;

	/// <summary>
	/// The directional_light_
	/// </summary>
	kgl::DirectionalLight directional_light_;

	/// <summary>
	/// The material_
	/// </summary>
	kgl::Material material_;


	kgl::SourceTextureSPtr diffuse_map_texture_;// = loadTexture("../../../resources/textures/bricks2.jpg");
	kgl::SourceTextureSPtr normal_map_texture_;// = loadTexture("../../../resources/textures/bricks2_normal.jpg");
	kgl::SourceTextureSPtr height_map_texture_;;// = loadTexture("../../../resources/textures/bricks2_disp.jpg");

	float height_scale = 0.1f;

	kgl::PrimitiveSPtr screen_quad_;
};


#endif // parallax_mapping_app_h__